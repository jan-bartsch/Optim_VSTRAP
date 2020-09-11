#include "optim_controller.h"

optim_controller::optim_controller() {
    logger::InitLog();
}

int optim_controller::start_optimizer(int argc, const char **argv)
{
    logger::Info("Starting optimizer...");

    std::string current_directory(get_current_dir_name());
    std::string input_directory;
    std::string pathToOptimInput;
    const char * input_xml_path;
    int return_flag_2;

    switch (argc) {
    case 1:
        input_directory = current_directory + "/Optim_input.xml";
        input_xml_path = input_directory.c_str();
        logger::Info("Starting optimizer with default values: " + input_directory);
        break;
    case 2:
        input_xml_path = argv[1];
        break;
    default:
        logger::Warning("Too many input parameters. Expected 2 but was " + std::to_string(argc));
        return 3;
        //throw std::runtime_error("Too many input parameters");
    }

    std::chrono::time_point<std::chrono::system_clock> start_optim = std::chrono::system_clock::now();

    int optim_flag = optim_controller::start_optimization_iteration(input_xml_path);

    std::chrono::time_point<std::chrono::system_clock> end_optim = std::chrono::system_clock::now();
    logger::Info("Optimization took: " + std::to_string(std::chrono::duration_cast<std::chrono::minutes>
                                                        (end_optim-start_optim).count()) + " minutes");

    if (optim_flag == 0) {
        logger::Info("Optimization ended without errors");
    } else {
        std::string return_string = "Optimization returned non-zero value: " + std::to_string(optim_flag);
        logger::Info(return_string);
    }

    return 0;

}

int optim_controller::start_optimization_iteration(const char * input_xml_path)
{
    data_provider data_provider_opt = data_provider(input_xml_path);
    gradient_calculator gradient_calculator_opt = gradient_calculator(input_xml_path);
    objective_calculator objective = objective_calculator(input_xml_path);
    output_control_update outController = output_control_update(input_xml_path);
    stepdirection_controller stepdir_contr = stepdirection_controller(input_xml_path);
    stepsize_controller stepsize_contr = stepsize_controller(input_xml_path);

    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(data_provider_opt);

    input input_control = input();
    input_control.setData_provider_optim(data_provider_opt);

    output_diagnostics outDiag = output_diagnostics();
    outDiag.setData_provider_optim(data_provider_opt);

    equation_solving_controller model_solver = equation_solving_controller();
    model_solver.setData_provider_optim(data_provider_opt);

    std::map<std::string, double> optimizationParameters = data_provider_opt.getOptimizationParameters();
    std::map<std::string, std::string> paths = data_provider_opt.getPaths();
    std::map<std::string,std::string> subroutines = data_provider_opt.getSubroutines();


    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    int zero_control = static_cast<int>(optimizationParameters.find("start_zero_control")->second);
    unsigned int calculation_functional = static_cast<unsigned int>(optimizationParameters.find("calculation_functional")->second);
    unsigned int calculation_wasserstein = static_cast<unsigned int>(optimizationParameters.find("calculation_wasserstein")->second);
    double fixed_gradient_descent_stepsize = static_cast<double>(optimizationParameters.find("fixed_gradient_descent_stepsize")->second);
    double fraction_of_optimal_control = static_cast<double>(optimizationParameters.find("fraction_of_optimal_control")->second);
    double fabs_tol_gp = static_cast<double>(optimizationParameters.find("fabs_tol_gp")->second);

    std::string BUILD_DIRECTORY_VSTRAP = paths.find("BUILD_DIRECTORY_VSTRAP")->second;
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string INPUT_FORWARD = paths.find("INPUT_FORWARD")->second;
    std::string INPUT_BACKWARD = paths.find("INPUT_BACKWARD")->second;

    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;

    /*
     * Check consistency of input files
     */
    //check_input_py(data_provider_opt, input_xml_path);

    std::string START_VSTRAP_FORWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + INPUT_FORWARD;
    int forward_return = 0;
    std::string START_VSTRAP_BACKWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + INPUT_BACKWARD;
    int backward_return = 0;

    double wasserstein_distance = 0.0;

    logger::Info("Reading paramters done");

    arma::mat control;
    if(zero_control == 0) {
        control = start_with_zero_control(input_xml_path);
    } else if (zero_control == 1) {
        control = start_with_given_control(input_xml_path);
        std::cout << control << std::endl;
    } else {
        logger::Info("Starting without control_field_cells");
    }
    generate_input_files(input_xml_path);

    /**
     * START OPTIMIZATION ITERATION
     */
    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::vector<particle>> forwardParticles_initialControl(ntimesteps_gp);
    std::unordered_map<coordinate_phase_space_time,double> forwardPDF_map;
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF;
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF_initial;

    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);
    std::unordered_map<coordinate_phase_space_time,double> backwardPDF_map;
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> backwardPDF;

    std::vector<std::unordered_map<coordinate_phase_space_time,double>> pdf_time(ntimesteps_gp);
    int assembling_flag;

    arma::mat gradient(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),3,arma::fill::zeros);
    arma::mat gradient_old(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),3,arma::fill::zeros);
    arma::mat stepDirection(static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second),3,arma::fill::zeros);
    double value_objective = 0.0;
    int stepsize_flag;
    double stepsize0 = fixed_gradient_descent_stepsize/std::max(1.0,arma::norm(gradient));
    double stepsize = stepsize0;
    double stepsize_before;
    double norm_Gradient = 0.0;

    std::chrono::time_point<std::chrono::system_clock> start, end;

    unsigned int optimizationIteration_max_gp = static_cast<unsigned int>(optimizationParameters.find("optimizationIteration_max_gp")->second);

    for(unsigned int r = 1; r <= optimizationIteration_max_gp; r++) {

        logger::Info("Starting VSTRAP (foward)... ");
        forward_return = model_solver.start_solving_forward(START_VSTRAP_FORWARD);
        if (forward_return != 0) {
            logger::Info("Forward VSTRAP returned non-zero value: " + std::to_string(forward_return));
            throw  std::system_error();
        }
        logger::Info("Finished VSTRAP... Reading particle files");
        input_control.read_plasma_state_forward(forwardParticles);

        //        forwardPDF = pdf_control.assemblingMultiDim_parallel(forwardParticles,0);
        //        value_objective = objective.calculate_objective_L2(forwardPDF,control);

        logger::Info("Finished reading files...");
        logger::Info("Starting VSTRAP (backward)...");

        backward_return = model_solver.start_solving_backward(START_VSTRAP_BACKWARD);
        if (backward_return != 0)  {
            logger::Info("Backward VSTRAP returned non-zero value: " + std::to_string(backward_return));
            throw std::system_error();
        }

        logger::Info("Reading particle files...");
        input_control.read_plasma_state_backward(backwardParticles);


        logger::Info("Assembling pdfs...");
        start = std::chrono::system_clock::now();
        assembling_flag = pdf_control.assemblingMultiDim_parallel(forwardParticles,0,pdf_time);
        forwardPDF = pdf_time;
        assembling_flag = pdf_control.assemblingMultiDim_parallel(backwardParticles,1,pdf_time);
        backwardPDF = pdf_time;
        end = std::chrono::system_clock::now();
        logger::Info("Assembling of pdfs took: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>
                                                                  (end-start).count()) + " second(s)");


        logger::Info("Building gradient...");
        //gradient = gradient_calculator_opt.calculateGradient_forceControl_space_L2(forwardPDF,backwardPDF,control);
        gradient = gradient_calculator_opt.calculateGradient_forceControl_space_Hm(forwardPDF,backwardPDF,control);
        outDiag.writeDoubleToFile(arma::norm(gradient,"fro"),"normGradientTrack");
        outDiag.writeGradientToFile(gradient,"gradient_"+std::to_string(r));
        norm_Gradient = arma::norm(gradient,"fro");

        if (r == 0) {
            //save plasma states using initial control
            //input_control.read_plasma_state_forward(forwardParticles_initialControl);
            forwardParticles_initialControl = forwardParticles;
            assembling_flag = pdf_control.assemblingMultiDim_parallel(forwardParticles,0,pdf_time);
            forwardPDF_initial = pdf_time;
        } else if (fmod(r,calculation_wasserstein) == 0.0) {
            wasserstein_distance = pdf_control.calculate_wasserstein_metric(forwardParticles_initialControl,forwardParticles);
            //wasserstein_distance = pdf_control.calculate_wasserstein_metric_histogramm(forwardPDF_initial,forwardPDF);
            std::cout << "Wasserstein distance: " << wasserstein_distance << std::endl;
            outDiag.writeDoubleToFile(wasserstein_distance,"wassersteinDistanceTrack");
        } else {
            std::cout << "No calculation of wasserstein distance" << std::endl;
        }

        if (calculation_functional == 0) {
            logger::Info("No calculation of functional");
        }
        else if (fmod(r,calculation_functional) == 0.0) {
            logger::Info("Calculating functional...");
            start = std::chrono::system_clock::now();
            value_objective = objective.calculate_objective_L2(forwardPDF,control);
            end = std::chrono::system_clock::now();
            logger::Info("Calculation of functional took: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>
                                                                             (end-start).count()) + " second(s)");
            outDiag.writeDoubleToFile(value_objective,"objectiveTrack");
        } else {
            std::cout << "No calculation of functional" << std::endl;
        }



        logger::Info("Updating the control...");
        stepDirection = stepdir_contr.get_stepdirection(gradient,gradient_old,stepDirection,r);
        gradient_old = gradient;
        stepsize_before = stepsize;
        stepsize_flag = stepsize_contr.calculate_stepsize(gradient,value_objective,control,
                                                          stepDirection,forwardParticles[0],stepsize);
        if (std::fabs(stepsize - stepsize_before) < fabs_tol_gp ) {
            stepsize = 2.0*stepsize; //Increase stepsize if too small (orthwise start with last stepsize)
        }

        std::cout << "Control in iteration " << r << std::endl;
        std::cout << control << std::endl;
        outDiag.writeGradientToFile(control,"control_"+std::to_string(r));

        if (stepsize_flag == 1) {
            std::string small_stepsize = "Linesearch returned too small stepsize; Found minimum after " + std::to_string(r+1) + " iterations";
            logger::Info(small_stepsize);
            return 0;
        } else if (stepsize_flag == 2) {
            std::string iteration_depth_reached = "Linesearch reached maximum iteration depth ("
                    + std::to_string(optimizationIteration_max_gp) + "), try to increase tolerance_gp";
            logger::Info(iteration_depth_reached);
            return 1;
        }

        outController.writeControl_XML(control);
        outDiag.writeDoubleToFile(arma::norm(control,"fro"),"normControlTrack");
        interpolate_control(data_provider_opt);

        logger::Info("Starting " + std::to_string(r+1) + " iteration");
    }

    return 0;
}

int optim_controller::check_input_py(data_provider provider, const char *filePathOptimInput)
{
    std::map<std::string, std::string> paths = provider.getPaths();
    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string INPUT_FORWARD = paths.find("INPUT_FORWARD")->second;
    std::string INPUT_BACKWARD = paths.find("INPUT_BACKWARD")->second;

    std::string CHECK_INPUT_PYHTON = "python3 " + DIRECTORY_TOOLSET + "check_input.py "
            + PATH_TO_SHARED_FILES + INPUT_FORWARD + " " + PATH_TO_SHARED_FILES
            + INPUT_BACKWARD + " " + &filePathOptimInput[0];

    int check_input_flag = system(&CHECK_INPUT_PYHTON[0]);

    //output is in 16 bit value
    if(check_input_flag == 512) {
        throw std::runtime_error("File not found in Python check");
    } else if (check_input_flag == 256) {
        throw std::runtime_error("Encountered inconsitency in input files");
    } else {
        logger::Info("Python check: **ALL CLEAR**");
    }

    return check_input_flag;
}

int optim_controller::interpolate_control(data_provider provider)
{
    std::map<std::string, std::string> paths = provider.getPaths();
    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;
    std::string CHECK_INPUT_PYHTON = "python3 " + DIRECTORY_TOOLSET + "check_input.py " + PATH_TO_SHARED_FILES;

    std::string BGF_CONTROL = paths.find("BGF_CONTROL")->second;
    std::string CONTROL_FIELD_CELLS_NAME = paths.find("CONTROL_FIELD_CELLS_NAME")->second;
    std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " +  PATH_TO_SHARED_FILES + DOMAIN_MESH +
            " " + PATH_TO_SHARED_FILES + CONTROL_FIELD_CELLS_NAME + " " + PATH_TO_SHARED_FILES + BGF_CONTROL;

    int interpolating_flag = system(&interpolating_control_python[0]);
    if(interpolating_flag == 512) {
        throw std::runtime_error("File not found in control interpolating");
    } else if (interpolating_flag == 256) {
        throw std::runtime_error("Interpolating returned error value");
    }

    return interpolating_flag;

}

arma::mat optim_controller::start_with_zero_control(const char *input_xml_path)
{
    data_provider data_provider_opt = data_provider(input_xml_path);
    output_control_update outController = output_control_update(input_xml_path);

    std::map<std::string, double> optimizationParameters = data_provider_opt.getOptimizationParameters();
    std::map<std::string, std::string> paths = data_provider_opt.getPaths();
    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;
    std::string BGF_CONTROL = paths.find("BGF_CONTROL")->second;
    std::string CONTROL_FIELD_CELLS_NAME = paths.find("CONTROL_FIELD_CELLS_NAME")->second;

    unsigned int dimensionOfControl = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);

    arma::mat control(dimensionOfControl,3,arma::fill::zeros);

    logger::Info("Deleting old files");
    std::string COMMAND_RM_RESULTS = "rm -r results";
    system(&COMMAND_RM_RESULTS[0]);
    std::string COMMAND_MKDIR_RESULTS = "mkdir results";
    system(&COMMAND_MKDIR_RESULTS[0]);
    logger::Info("Starting with zero control");
    outController.writeControl_XML(control);
    std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + PATH_TO_SHARED_FILES +  DOMAIN_MESH +
            " " + PATH_TO_SHARED_FILES + CONTROL_FIELD_CELLS_NAME + " " + PATH_TO_SHARED_FILES + BGF_CONTROL;
    system(&interpolating_control_python[0]);

    return control;
}

arma::mat optim_controller::start_with_given_control(const char *input_xml_path)
{
    data_provider data_provider_opt = data_provider(input_xml_path);
    output_control_update outController = output_control_update(input_xml_path);

    std::map<std::string, double> optimizationParameters = data_provider_opt.getOptimizationParameters();
    std::map<std::string, std::string> paths = data_provider_opt.getPaths();
    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;
    std::string BGF_CONTROL = paths.find("BGF_CONTROL")->second;
    std::string CONTROL_FIELD_CELLS_NAME = paths.find("CONTROL_FIELD_CELLS_NAME")->second;
    double fraction_of_optimal_control = static_cast<double>(optimizationParameters.find("fraction_of_optimal_control")->second);

    logger::Info("Deleting old files");
    std::string COMMAND_RM_RESULTS = "rm -rf results";
    system(&COMMAND_RM_RESULTS[0]);
    std::string COMMAND_MKDIR_RESULTS = "mkdir results";
    system(&COMMAND_MKDIR_RESULTS[0]);

    logger::Info("Starting with existing control (multiplied by a positive constant)");
    std::string READ_CONTROL = PATH_TO_SHARED_FILES + CONTROL_FIELD_CELLS_NAME;
    arma::mat control = input::readControl(&READ_CONTROL[0]);
    outController.writeControl_XML(fraction_of_optimal_control*control);
    std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + DOMAIN_MESH +
            " " + PATH_TO_SHARED_FILES + CONTROL_FIELD_CELLS_NAME + " " + PATH_TO_SHARED_FILES + BGF_CONTROL;
    system(&interpolating_control_python[0]);

    return control;
}

int optim_controller::generate_input_files(const char *input_xml_path)
{
    data_provider data_provider_opt = data_provider(input_xml_path);

    std::map<std::string, std::string> paths = data_provider_opt.getPaths();
    std::string PATH_TO_SHARED_FILES_ABSOLUTE = paths.find("PATH_TO_SHARED_FILES_ABSOLUTE")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;

    std::string generation_string = "python3 " + DIRECTORY_TOOLSET + "generate_forward_input.py" + " " + &input_xml_path[0];
    generation_string += " && python3 " + DIRECTORY_TOOLSET + "generate_backward_input.py" + " " + &input_xml_path[0];
    generation_string += " && python3 " + DIRECTORY_TOOLSET + "generate_adjoint_particle_creation.py" + " " + &input_xml_path[0] + " " + PATH_TO_SHARED_FILES_ABSOLUTE;
    logger::Info("Running " + generation_string);
    system(&generation_string[0]);

    return 0;
}
