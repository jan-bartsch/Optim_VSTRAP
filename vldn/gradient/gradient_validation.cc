#include "gradient_validation.h"

gradient_validation::gradient_validation() {}

int gradient_validation::landau_validation(int argc, char **argv) {
  DataProvider validation_provider = DataProvider(argv[1]);
  std::map<std::string, std::string> validation_paths =
      validation_provider.getPaths();

  std::string directory_optim_input =
      validation_paths.at("directory_optim_input");
  DataProvider optimization_provider =
      DataProvider(directory_optim_input.c_str());

  auto shared_optim_input_data = std::make_shared<MOTIONS::InputData>(
      MOTIONS::InitializeMotions::Load_MOTIONS(optimization_provider));

  Input in = Input(shared_optim_input_data);
  PdfController pdf_control = PdfController(shared_optim_input_data);
  OutputDiagnostics outDiag = OutputDiagnostics(shared_optim_input_data);
  EquationSolvingController model_solver =
      EquationSolvingController(shared_optim_input_data);
  InnerProducts product = InnerProducts(shared_optim_input_data);
  GradientCalculator gradient_calculator_opt =
      GradientCalculator(shared_optim_input_data);
  ObjectiveCalculator objective = ObjectiveCalculator(shared_optim_input_data);
  OutputControlUpdate outController =
      OutputControlUpdate(shared_optim_input_data);

  arma::mat control(32, 3, arma::fill::ones);

  std::cout << model_solver.Laplacian3D() << std::endl;

  unsigned int ntimesteps_gp = shared_optim_input_data->ntimesteps_gp;
  double number_cells_position = shared_optim_input_data->number_cells_position;

  std::vector<std::vector<Particle>> forwardParticles(ntimesteps_gp);
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> forwardPDF;

  std::vector<std::vector<Particle>> backwardParticles(ntimesteps_gp);
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> backwardPDF;

  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> pdf_time(
      ntimesteps_gp);
  int assembling_flag;

  std::string build_directory_vstrap =
      shared_optim_input_data->build_directory_vstrap;
  std::string path_to_shared_files =
      shared_optim_input_data->path_to_shared_files;

  std::string start_vstrap_forward = build_directory_vstrap + "vstrap" + " " +
                                     path_to_shared_files +
                                     shared_optim_input_data->input_forward;
  int forward_return = 0;
  std::string start_vstrap_backward = build_directory_vstrap + "vstrap" + " " +
                                      path_to_shared_files +
                                      shared_optim_input_data->input_backward;
  int backward_return = 0;

  arma::mat control0(number_cells_position, 3, arma::fill::zeros);
  //    control0 = -10.0*control0;
  arma::mat gradient(number_cells_position, 3, arma::fill::zeros);

  outController.WritecontrolXml(control0);
  outDiag.WriteDoubleToFile(arma::norm(control0, "fro"), "normControlTrack");
  outController.InterpolateControl(shared_optim_input_data);

  /*
   * calculat J^(u0)
   */
  logger::Info("Starting VSTRAP (foward)... ");
  forward_return = model_solver.StartSolvingForward(start_vstrap_forward);
  if (forward_return != 0) {
    logger::Info("Forward VSTRAP returned non-zero value: " +
                 std::to_string(forward_return));
    throw std::system_error();
  }
  logger::Info("Finished VSTRAP... Reading particle files");
  in.ReadPlasmaStateForward(forwardParticles,
                            "plasma_state_batch_1_forward_particles_CPU_");

  logger::Info("Finished reading files...");
  logger::Info("Starting VSTRAP (backward)...");

  backward_return = model_solver.StartSolvingBackward(start_vstrap_backward);
  if (backward_return != 0) {
    logger::Info("Backward VSTRAP returned non-zero value: " +
                 std::to_string(backward_return));
    throw std::system_error();
  }

  logger::Info("Reading particle files...");
  in.ReadPlasmaStateBackward(backwardParticles,
                             "plasma_state_batch_1_adjoint_particles_CPU_");

  logger::Info("Assembling pdfs...");

  assembling_flag =
      pdf_control.AssemblingMultidimParallel(forwardParticles, 0, pdf_time);
  forwardPDF = pdf_time;
  assembling_flag =
      pdf_control.AssemblingMultidimParallel(backwardParticles, 1, pdf_time);
  backwardPDF = pdf_time;

  if (assembling_flag != 0) {
    throw std::runtime_error("Too many too fast particles!");
  }

  double functional_value0 = objective.CalculateObjective(forwardPDF, control0);

  logger::Info("Building gradient...");
  arma::mat gradient0 =
      gradient_calculator_opt.CalculategradientForcecontrolSpaceHm(
          forwardPDF, backwardPDF, control0);
  outDiag.WriteDoubleToFile(arma::norm(gradient, "fro"), "normGradientTrack");

  arma::mat delta_control(number_cells_position, 3, arma::fill::ones);
  double t = -10.0;
  delta_control = t * delta_control;

  unsigned long iteration_number = 10;
  double reducing_factor = 0.5;

  std::vector<double> functional_values(iteration_number, 0.0);
  std::vector<double> difference(iteration_number, 0.0);
  std::vector<double> difference_Landau(iteration_number, 0.0);
  arma::mat control_temp;

  for (unsigned int i = 0; i < static_cast<unsigned long>(iteration_number);
       i++) {
    control_temp = control0 + pow(reducing_factor, i) * delta_control;
    outController.WritecontrolXml(control_temp);
    outController.InterpolateControl(shared_optim_input_data);

    logger::Info("Starting VSTRAP (foward)... ");
    forward_return = model_solver.StartSolvingForward(start_vstrap_forward);
    if (forward_return != 0) {
      logger::Info("Forward VSTRAP returned non-zero value: " +
                   std::to_string(forward_return));
      throw std::system_error();
    }
    logger::Info("Finished VSTRAP... Reading particle files");
    in.ReadPlasmaStateForward(forwardParticles,
                              "plasma_state_batch_1_forward_particles_CPU_");

    assembling_flag =
        pdf_control.AssemblingMultidimParallel(forwardParticles, 0, pdf_time);
    forwardPDF = pdf_time;

    if (assembling_flag != 0) {
      throw std::runtime_error("Too many too fast particles!");
    }

    functional_values[i] =
        objective.CalculateObjective(forwardPDF, control_temp);
    std::cout << std::to_string(functional_values[i]) << std::endl;
    std::cout << "Stepsize: " << pow(reducing_factor, i) << std::endl;
    difference[i] = (functional_values[i] - functional_value0) -
                    (pow(reducing_factor, i)) *
                        product.H2InnerProduct(gradient0, delta_control);

    difference_Landau[i] =
        std::abs(difference[i]) / (pow(pow(reducing_factor, i), 2));

    outDiag.WriteDoubleVectorToFile(difference, "Difference");
    outDiag.WriteDoubleVectorToFile(difference_Landau, "Difference_Landau");
    outDiag.WriteDoubleVectorToFile(functional_values, "FunctionalValues");
  }

  std::cout << "Difference: " << std::endl;
  for (int i = 0; i < iteration_number; i++) {
    std::cout << std::to_string(difference[i]) << std::endl;
  }
  std::cout << "Difference Landau: " << std::endl;
  for (int i = 0; i < iteration_number; i++) {
    std::cout << std::to_string(difference_Landau[i]) << std::endl;
  }
}
