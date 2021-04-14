#include "stepsizecontroller.h"

StepsizeController::StepsizeController(
    std::shared_ptr<MOTIONS::InputData> &input_data) {
  this->setInput_data(input_data);
}

int StepsizeController::CalculateStepsize(
    arma::mat &gradient, double J0, arma::mat &control,
    arma::mat &stepdirection, std::vector<Particle> &Input_particles,
    double &stepsize0) {

  std::string control_update = "";
  try {
    control_update = input_data_->control_update;
  } catch (std::exception e) {
    logger::Info("No control_update found");
    logger::Info(e.what());
  }

  if (control_update.compare("armijo_linesearch") == 0) {
    logger::Info("Updating control using stepsize-selection strategy: Armijo "
                 "lineasearch");
    return ArmijoLinesearch(gradient, J0, control, stepdirection,
                            Input_particles, stepsize0);
  } else if (control_update.compare("gradient_descent") == 0) {
    logger::Info(
        "Updating control using stepsize-selection strategy: gradient descent");
    return GradientDescent(control, stepdirection, Input_particles, stepsize0);
  } else if (control_update.compare("successive_approximation") == 0) {
    logger::Info("Updating control using stepsize-selection strategy: "
                 "successive approximation");
    return SuccessiveApproximation(gradient, J0, control, stepdirection,
                                   Input_particles, stepsize0);
  } else {
    std::invalid_argument("No such control update subroutine");
    return 1;
  }
}

int StepsizeController::ArmijoLinesearch(arma::mat &gradient, double J0,
                                         arma::mat &control,
                                         arma::mat &stepdirection,
                                         std::vector<Particle> &Input_particles,
                                         double &stepsize0) {
  int return_flag = 0;
  std::ostringstream streamObj;

  OutputControlUpdate outController = OutputControlUpdate(input_data_);
  ObjectiveCalculator objective = ObjectiveCalculator(input_data_);
  OutputDiagnostics outDiag = OutputDiagnostics(input_data_);
  PdfController pdf_control = PdfController(input_data_);
  Input Input_control = Input(input_data_);

  double small_discr_sidelength = input_data_->small_discr_sidelength;

  std::string start_vstrap_forward =
      input_data_->build_directory_vstrap + "vstrap" + " " +
      input_data_->path_to_shared_files + input_data_->input_forward;

  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> pdf_time(
      input_data_->ntimesteps_gp);
  int assembling_flag;

  double scalarProduct = (arma::dot(gradient.col(0), stepdirection.col(0)) +
                          arma::dot(gradient.col(1), stepdirection.col(1)) +
                          arma::dot(gradient.col(2), stepdirection.col(2))) *
                         small_discr_sidelength;

  double scalarProduct_Stepdirection =
      (arma::dot(stepdirection.col(0), stepdirection.col(0)) +
       arma::dot(stepdirection.col(1), stepdirection.col(1)) +
       arma::dot(stepdirection.col(2), stepdirection.col(2))) *
      small_discr_sidelength;

  InnerProducts pro = InnerProducts(input_data_);
  double scalarProduct_Hm = pro.H1InnerProduct(gradient, stepdirection);

  std::cout << "scalarProduct: " << scalarProduct << std::endl;
  std::cout << "Stepdirection: " << stepdirection << std::endl;

  if (scalarProduct_Hm > 0) {
    std::cout << "No descent direction!" << std::endl;
    // throw std::runtime_error("No descent-direction");
  }

  unsigned int counter = 0;
  double Jtemp = std::numeric_limits<double>::max();

  double alpha = stepsize0; // stepsize
  arma::mat control0 = control;

  std::vector<std::vector<Particle>> forwardParticles(
      input_data_->ntimesteps_gp);
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
      forwardPDF_time;

  std::string bgf_control = input_data_->bgf_control;
  std::string control_field_cells_name = input_data_->control_field_cells_name;

  int forward_return;

  /*
   * Generate new control
   */
  control = control0 + alpha * stepdirection;
  outController.WritecontrolXml(control);
  outController.InterpolateControl(input_data_);

  forward_return = system(&start_vstrap_forward[0]);

  if (forward_return == 0) {
    Input_control.ReadPlasmaStateForward(
        forwardParticles, "plasma_state_batch_1_forward_particles_CPU_");
    assembling_flag =
        pdf_control.AssemblingMultidimParallel(forwardParticles, 0, pdf_time);
    forwardPDF_time = pdf_time;
    if (assembling_flag == 0) {
      Jtemp = objective.CalculateObjective(forwardPDF_time,
                                           control0 + alpha * stepdirection);
    }
  } else {
    logger::Info("Forward VSTRAP returned non-zero value: " +
                 std::to_string(forward_return));
    throw std::system_error();
  }

  while (Jtemp > J0 + alpha * scalarProduct_Hm *
                          input_data_->armijo_descent_fraction &&
         alpha > input_data_->tolerance_gp &&
         counter <= input_data_->linesearchIteration_max_gp) {
    // while (Jtemp > J0 -
    // alpha*scalarProduct_Stepdirection*armijo_descent_fraction && alpha >
    // tolerance
    //      && counter <= linesearchIteration_max_gp) {
    alpha = pow(input_data_->armijio_base_exp, counter) *
            input_data_->armijo_iterative_exp * alpha;

    control = control0 + alpha * stepdirection;
    outController.WritecontrolXml(control0 + alpha * stepdirection);
    outController.InterpolateControl(input_data_);

    forward_return = system(&start_vstrap_forward[0]);

    if (forward_return == 0) {
      Input_control.ReadPlasmaStateForward(
          forwardParticles, "plasma_state_batch_1_forward_particles_CPU_");
      assembling_flag =
          pdf_control.AssemblingMultidimParallel(forwardParticles, 0, pdf_time);
      forwardPDF_time = pdf_time;
      if (assembling_flag == 0) {
        Jtemp = objective.CalculateObjective(forwardPDF_time,
                                             control0 + alpha * stepdirection);
      }
    } else {
      logger::Info("Forward VSTRAP returned non-zero value: " +
                   std::to_string(forward_return));
      throw std::system_error();
    }

    std::cout << "Armijo: "
              << "Jtemp = " << Jtemp << std::endl
              << "J0 + scalarProduct*armijo_descent_fraction = "
              << J0 + alpha * scalarProduct *
                          input_data_->armijo_descent_fraction
              << std::endl
              << "Stepsize " << alpha << " in " << counter << ". iteration"
              << std::endl;

    counter++;
  }

  if (alpha < input_data_->tolerance_gp) {
    // Calculate with old control
    control = control0;
    outController.WritecontrolXml(control0);
    outController.InterpolateControl(input_data_);

    forward_return = system(&start_vstrap_forward[0]);
    streamObj << input_data_->tolerance_gp;
    logger::Info("Minimum already reached. You may want to decrease your "
                 "tolerance? (Was " +
                 streamObj.str() + ")");
    return_flag = 1;
    return return_flag;
  } else if (counter > input_data_->linesearchIteration_max_gp) {
    control = control0;
    outController.WritecontrolXml(control0);
    outController.InterpolateControl(input_data_);
    logger::Info("Maximum interation depth ( " +
                 std::to_string(input_data_->linesearchIteration_max_gp) +
                 " ) reached without decrease!");
    return_flag = 2;
    return return_flag;
  } else {
    streamObj << alpha;
    logger::Info("Armijo-linesearch found stepsize " + streamObj.str() +
                 " after " + std::to_string(counter - 1) + " iterations.");
  }

  outDiag.WriteDoubleToFile(alpha, "stepsizeTrack");
  stepsize0 = alpha;

  return return_flag;
}

int StepsizeController::GradientDescent(arma::mat &control,
                                        arma::mat &stepdirection,
                                        std::vector<Particle> &Input_particles,
                                        double &stepsize) {

  int return_flag = 0;

  OutputControlUpdate outController = OutputControlUpdate(input_data_);
  PdfController pdf_control = PdfController(input_data_);
  Input Input_control = Input(input_data_);

  std::string start_vstrap_forward =
      input_data_->build_directory_vstrap + "vstrap" + " " +
      input_data_->path_to_shared_files + input_data_->input_forward;

  int assembling_flag = 1;
  double alpha = 1.0;
  arma::mat control0 = control;

  double counter = 1.0;
  std::vector<std::vector<Particle>> forwardParticles(
      input_data_->ntimesteps_gp);
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
      forwardPDF_time;
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> pdf_time(
      input_data_->ntimesteps_gp);

  while (assembling_flag == 1 &&
         counter <= input_data_->linesearchIteration_max_gp) {
    int forward_return;
    alpha = alpha / 2.0;

    /*
     * Generate new control
     */
    control = control0 + alpha / counter *
                             input_data_->fixed_gradient_descent_stepsize *
                             stepdirection / arma::norm(stepdirection);
    outController.WritecontrolXml(control);
    outController.InterpolateControl(input_data_);

    forward_return = system(&start_vstrap_forward[0]);

    if (forward_return == 0) {
      Input_control.ReadPlasmaStateForward(
          forwardParticles, "plasma_state_batch_1_forward_particles_CPU_");
      assembling_flag =
          pdf_control.AssemblingMultidimParallel(forwardParticles, 0, pdf_time);
    }
    counter = counter + 1.0;
  }

  return return_flag;
}

int StepsizeController::SuccessiveApproximation(
    arma::mat &gradient, double J0, arma::mat &control,
    arma::mat &stepdirection, std::vector<Particle> &Input_particles,
    double &stepsize0) {

  int flag = 0;

  control = -gradient;

  return flag;
}
