#include "gradient_validation.h"

gradient_validation::gradient_validation() {}

int gradient_validation::landau_validation(int argc, char **argv) {
  DataProvider validation_provider = DataProvider(argv[1]);
  std::map<std::string, std::string> validation_paths =
      validation_provider.getPaths();
  std::map<std::string, double> validation_params =
      validation_provider.getOptimizationParameters();

  std::string DIRECTORY_OPTIM_Input =
      validation_paths.find("DIRECTORY_OPTIM_Input")->second;
  DataProvider optimization_provider =
      DataProvider(DIRECTORY_OPTIM_Input.c_str());

  Input in = Input();
  in.set_DataProviderOptim(optimization_provider);

  PdfController pdf_control = PdfController();
  pdf_control.set_DataProviderOptim(optimization_provider);

  OutputDiagnostics outDiag = OutputDiagnostics();
  outDiag.set_DataProviderOptim(optimization_provider);

  EquationSolvingController model_solver = EquationSolvingController();
  model_solver.set_DataProviderOptim(optimization_provider);

  arma::mat control(32, 3, arma::fill::ones);

  std::cout << model_solver.Laplacian3D() << std::endl;

  OptimController contr = OptimController();
  contr.set_DataProviderOptim(optimization_provider);

  InnerProducts product = InnerProducts();
  product.set_DataProviderOptim(optimization_provider);

  // contr.generate_Input_files(DIRECTORY_OPTIM_Input.c_str());

  GradientCalculator gradient_calculator_opt =
      GradientCalculator(DIRECTORY_OPTIM_Input.c_str());
  ObjectiveCalculator objective =
      ObjectiveCalculator(DIRECTORY_OPTIM_Input.c_str());
  OutputControlUpdate outController =
      OutputControlUpdate(DIRECTORY_OPTIM_Input.c_str());

  std::map<std::string, double> optimizationParameters =
      optimization_provider.getOptimizationParameters();
  std::map<std::string, std::string> paths = optimization_provider.getPaths();

  unsigned int dimensionOfControl_gp = static_cast<unsigned int>(
      optimizationParameters.find("dimensionOfControl_gp")->second);
  unsigned int ntimesteps_gp = static_cast<unsigned int>(
      optimizationParameters.find("ntimesteps_gp")->second);
  double number_cells_position = static_cast<unsigned int>(
      optimizationParameters.find("number_cells_position")->second);

  std::vector<std::vector<Particle>> forwardParticles(ntimesteps_gp);
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> forwardPDF;

  std::vector<std::vector<Particle>> backwardParticles(ntimesteps_gp);
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> backwardPDF;

  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> pdf_time(
      ntimesteps_gp);
  int assembling_flag;

  std::string BUILD_DIRECTORY_VSTRAP =
      paths.find("BUILD_DIRECTORY_VSTRAP")->second;
  std::string BUILD_DIRECTORY_OPTIM =
      paths.find("BUILD_DIRECTORY_OPTIM")->second;
  std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
  std::string Input_FORWARD = paths.find("Input_FORWARD")->second;
  std::string Input_BACKWARD = paths.find("Input_BACKWARD")->second;

  std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
  std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;

  std::string START_VSTRAP_FORWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " +
                                     PATH_TO_SHARED_FILES + Input_FORWARD;
  int forward_return = 0;
  std::string START_VSTRAP_BACKWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " +
                                      PATH_TO_SHARED_FILES + Input_BACKWARD;
  int backward_return = 0;

  arma::mat control0(number_cells_position, 3, arma::fill::zeros);
  //    control0 = -10.0*control0;
  arma::mat gradient(number_cells_position, 3, arma::fill::zeros);

  outController.WritecontrolXml(control0);
  outDiag.WriteDoubleToFile(arma::norm(control0, "fro"), "normControlTrack");
  outController.InterpolateControl(optimization_provider);

  /*
   * calculat J^(u0)
   */
  logger::Info("Starting VSTRAP (foward)... ");
  forward_return = model_solver.StartSolvingForward(START_VSTRAP_FORWARD);
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

  backward_return = model_solver.StartSolvingBackward(START_VSTRAP_BACKWARD);
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
  double small_discr_volume = static_cast<double>(
      optimizationParameters.find("small_discr_volume")->second);

  std::vector<double> functional_values(iteration_number, 0.0);
  std::vector<double> difference(iteration_number, 0.0);
  std::vector<double> difference_Landau(iteration_number, 0.0);
  arma::mat control_temp;

  for (unsigned int i = 0; i < static_cast<unsigned long>(iteration_number);
       i++) {
    control_temp = control0 + pow(reducing_factor, i) * delta_control;
    outController.WritecontrolXml(control_temp);
    outController.InterpolateControl(optimization_provider);

    logger::Info("Starting VSTRAP (foward)... ");
    forward_return = model_solver.StartSolvingForward(START_VSTRAP_FORWARD);
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
