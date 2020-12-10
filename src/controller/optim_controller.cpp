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

    int simulating_plasma = static_cast<int>(optimizationParameters.find("simulating_plasma")->second);

    std::string BUILD_DIRECTORY_VSTRAP = paths.find("BUILD_DIRECTORY_VSTRAP")->second;
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string INPUT_FORWARD = paths.find("INPUT_FORWARD")->second;
    std::string INPUT_BACKWARD = paths.find("INPUT_BACKWARD")->second;

    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;


    std::string START_VSTRAP_FORWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + INPUT_FORWARD;
    int forward_return = 0;
    std::string START_VSTRAP_BACKWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + INPUT_BACKWARD;
    int backward_return = 0;

    double wasserstein_distance = 0.0;

    logger::Info("Reading paramters done");

    arma::mat control(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),3,arma::fill::zeros);
    if(zero_control == 0) {
        control = start_with_zero_control(input_xml_path);
        generate_input_files(input_xml_path);
    } else if (zero_control == 1) {
        control = start_with_given_control(input_xml_path);
        std::cout << control << std::endl;
    } else if (zero_control == 2) {
        logger::Info("Starting with zero control but not deleting old files");
        outController.writeControl_XML(control);
        outController.interpolate_control(data_provider_opt);
    }else {
        logger::Info("Starting without control_field_cells");
    }

    /*
     * Check consistency and sanity of input files
     */
    //check_input_py(data_provider_opt, input_xml_path);
    parameter_sanity ps = parameter_sanity();
    ps.check_adjoint_velocity(data_provider_opt);



    /**
     * START OPTIMIZATION ITERATION
     */
    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF;
    std::vector<std::vector<particle>> forwardParticles_electrons(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF_electrons;

    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF_initial; //Needed for Wasserstein
    std::vector<std::vector<particle>> forwardParticles_initialControl(ntimesteps_gp); //Needed for calculation of Wasserstein distance

    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> backwardPDF;
    std::vector<std::vector<particle>> backwardParticles_electrons(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> backwardPDF_electrons;

    std::vector<std::unordered_map<coordinate_phase_space_time,double>> pdf_time(ntimesteps_gp);
    int assembling_flag;

    arma::mat gradient(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),3,arma::fill::zeros);
    arma::mat gradient_old(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),3,arma::fill::zeros);
    arma::mat stepDirection(static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second),3,arma::fill::zeros);
    double value_objective = 0.0;
    int stepsize_flag;
    double stepsize = fixed_gradient_descent_stepsize;
    double stepsize_before;
    double norm_Gradient = 0.0;

    std::chrono::time_point<std::chrono::system_clock> start, end;

    unsigned int optimizationIteration_max_gp = static_cast<unsigned int>(optimizationParameters.find("optimizationIteration_max_gp")->second);

    input_control.readBrockettFile(PATH_TO_SHARED_FILES+"brockett.csv",",",ntimesteps_gp);

    for(unsigned int r = 1; r <= optimizationIteration_max_gp; r++) {

        logger::Info("Starting VSTRAP (foward)... ");
        forward_return = model_solver.start_solving_forward(START_VSTRAP_FORWARD);
        if (forward_return != 0) {
            logger::Info("Forward VSTRAP returned non-zero value: " + std::to_string(forward_return));
            throw  std::system_error();
        }
        logger::Info("Finished VSTRAP");

        logger::Info("Reading particle files");
        input_control.read_plasma_state_forward(forwardParticles,"plasma_state_batch_1_forward_particles_CPU_");
        if (simulating_plasma == 0) {
            input_control.read_plasma_state_forward(forwardParticles_electrons,"plasma_state_batch_e_forward_particles_electrons_CPU_");
        }


        //        forwardPDF = pdf_control.assemblingMultiDim_parallel(forwardParticles,0);
        //        value_objective = objective.calculate_objective_L2(forwardPDF,control);

        logger::Info("Finished reading files...");
        logger::Info("Starting VSTRAP (backward)...");

        backward_return = model_solver.start_solving_backward(START_VSTRAP_BACKWARD);
        if (backward_return != 0)  {
            logger::Info("Backward VSTRAP returned non-zero value: " + std::to_string(backward_return));
            throw std::system_error();
        }
        logger::Info("Finished backward VSTRAP");



        logger::Info("Reading particle files...");
        input_control.read_plasma_state_backward(backwardParticles,"plasma_state_batch_1_adjoint_particles_CPU_");
        if (simulating_plasma == 0) {
            input_control.read_plasma_state_backward(backwardParticles_electrons,"plasma_state_batch_e_adjoint_particles_electrons_CPU_");
        }


        logger::Info("Assembling pdfs...");
        start = std::chrono::system_clock::now();
        assembling_flag = pdf_control.assemblingMultiDim_parallel(forwardParticles,0,pdf_time);
        forwardPDF = pdf_time;
        assembling_flag = pdf_control.assemblingMultiDim_parallel(backwardParticles,1,pdf_time);
        backwardPDF = pdf_time;

        if (simulating_plasma == 0) {
            assembling_flag = pdf_control.assemblingMultiDim_parallel(forwardParticles_electrons,0,pdf_time);
            forwardPDF_electrons = pdf_time;
            assembling_flag = pdf_control.assemblingMultiDim_parallel(backwardParticles_electrons,1,pdf_time);
            backwardPDF_electrons = pdf_time;
        }
        end = std::chrono::system_clock::now();
        logger::Info("Assembling of pdfs took: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>
                                                                  (end-start).count()) + " second(s)");



        logger::Info("Building gradient...");
        if (simulating_plasma == 0) {
            gradient = gradient_calculator_opt.calculateGradient_forceControl_space_Hm_plasma(forwardPDF,backwardPDF,forwardPDF_electrons,backwardPDF_electrons,control);
        } else {
            gradient = gradient_calculator_opt.calculateGradient_forceControl_space_Hm(forwardPDF,backwardPDF,control);
        }
        outDiag.writeDoubleToFile(arma::norm(gradient,"fro"),"normGradientTrack");
        outDiag.writeArmaMatrixToFile(gradient,"gradient_"+std::to_string(r));
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
            value_objective = objective.calculate_objective(forwardPDF,control);
            end = std::chrono::system_clock::now();
            logger::Info("Calculation of functional took: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>
                                                                             (end-start).count()) + " second(s)");
            outDiag.writeDoubleToFile(value_objective,"objectiveTrack");
        } else {
            std::cout << "No calculation of functional" << std::endl;
        }

        /**
         * first stepsize guess, scaled with norm of gradient
         */

        if (r == 1) {
            double norm_gradient = arma::norm(gradient);
            std::cout << "Norm of gradient: " + std::to_string(arma::norm(gradient)) << std::endl;
            if (arma::norm(gradient) < optimizationParameters.find("tolerance_gp")->second) {
                throw std::runtime_error("Too small first gradient. Norm was smaller than tolerance_gp <" + std::to_string(optimizationParameters.find("tolerance_gp")->second) +">");
            }
            stepsize = fixed_gradient_descent_stepsize/norm_gradient;
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
        outDiag.writeArmaMatrixToFile(control,"control_"+std::to_string(r));

        if (stepsize_flag == 1) {
            std::string small_stepsize = "Linesearch returned too small stepsize; Found minimum after " + std::to_string(r) + " iterations";
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
        outController.interpolate_control(data_provider_opt);

        logger::Info("Starting post_processing");
        post_processing_convergence(data_provider_opt);
        visualize_control(data_provider_opt);
        paraview_plot_forward(data_provider_opt);

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
        throw std::runtime_error("Encountered inconsitency in input files OR File not found");
    } else {
        logger::Info("Python check: **ALL CLEAR**");
    }

    return check_input_flag;
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

    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);

    arma::mat control(pcell_gp,3,arma::fill::zeros);

    logger::Info("Deleting old files");
    std::string COMMAND_RM_RESULTS = "rm -r results/";
    system(&COMMAND_RM_RESULTS[0]);
    std::string COMMAND_MKDIR_RESULTS = "mkdir results && mkdir -p results/" + paths.find("mesh_2d_path")->second +
            " && mkdir -p results/"+ paths.find("mesh_3d_path")->second;
    system(&COMMAND_MKDIR_RESULTS[0]);
    logger::Info("Starting with zero control");
    outController.writeControl_XML(control);
    outController.interpolate_control(data_provider_opt);

    return control;
}

arma::mat optim_controller::start_with_given_control(const char *input_xml_path)
{
    data_provider data_provider_opt = data_provider(input_xml_path);
    output_control_update outController = output_control_update(input_xml_path);

    input in = input();
    in.setData_provider_optim(data_provider_opt);


    std::map<std::string, double> optimizationParameters = data_provider_opt.getOptimizationParameters();
    std::map<std::string, std::string> paths = data_provider_opt.getPaths();
    std::string START_WITH_EXISTING_CONTROL = paths.find("START_WITH_EXISTING_CONTROL")->second;
    double fraction_of_optimal_control = static_cast<double>(optimizationParameters.find("fraction_of_optimal_control")->second);
     int pcell_gp = static_cast<int>(optimizationParameters.find("pcell_gp")->second);

    logger::Info("Deleting old .txt and .csv files");
    std::string COMMAND_RM_RESULTS = "rm *.csv && rm *.txt";
    system(&COMMAND_RM_RESULTS[0]);

    logger::Info("Starting with existing control (multiplied by a positive constant)");
    std::string READ_CONTROL = START_WITH_EXISTING_CONTROL;
    arma::mat control = in.readControl(&READ_CONTROL[0],pcell_gp);
    outController.writeControl_XML(fraction_of_optimal_control*control);
    outController.interpolate_control(data_provider_opt);

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

int optim_controller::post_processing_convergence(data_provider provider)
{
    std::map<std::string, std::string> paths = provider.getPaths();
    std::string PATH_TO_SHARED_FILES_ABSOLUTE = paths.find("PATH_TO_SHARED_FILES_ABSOLUTE")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;

    std::string POSTPROCESSING_STRING = "python3 " + DIRECTORY_TOOLSET + "post_processing_convergence.py " + PATH_TO_SHARED_FILES_ABSOLUTE;

    logger::Info("Postprocessing ... using command " + POSTPROCESSING_STRING);
    try {
        system(&POSTPROCESSING_STRING[0]);
    } catch (std::exception e) {
        throw std::runtime_error(e.what());
    }
    return 0;
}

int optim_controller::visualize_control(data_provider provider)
{
    std::map<std::string, std::string> paths = provider.getPaths();
    std::map<std::string, double> parameters = provider.getOptimizationParameters();

    std::string PATH_TO_SHARED_FILES_ABSOLUTE = paths.find("PATH_TO_SHARED_FILES_ABSOLUTE")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string BGF_CONTROL = paths.find("BGF_CONTROL")->second;
    std::string DOMAIN_MESH_FILE = paths.find("DOMAIN_MESH_FILE")->second;

    double visalization_scaling = parameters.find("visalization_scaling")->second;

    std::string VISUALIZING_STRING = "python3 " + DIRECTORY_TOOLSET + "visualize_control.py "
            + PATH_TO_SHARED_FILES_ABSOLUTE + BGF_CONTROL + " ../../Optim_VSTRAP/data/global/"
            + DOMAIN_MESH_FILE + " " + std::to_string(visalization_scaling) + " " + PATH_TO_SHARED_FILES_ABSOLUTE + " "
            + std::to_string(parameters.find("pmax_gp")->second);

    logger::Info("Visualize control ... using command " + VISUALIZING_STRING);
    try {
        system(&VISUALIZING_STRING[0]);
    } catch (std::exception e) {
        throw std::runtime_error(e.what());
    }
    return 0;
}

int optim_controller::paraview_plot_forward(data_provider provider)
{
    std::map<std::string, std::string> paths = provider.getPaths();
    std::string PATH_TO_SHARED_FILES_ABSOLUTE = paths.find("PATH_TO_SHARED_FILES_ABSOLUTE")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;

    std::string PVPYTHON_ABSOLUTE_DIR = paths.find("PVPYTHON_ABSOLUTE_DIR")->second;

    std::string PARAVIEW_ANIMATION = "cd results && mkdir -p animation && cd animation && " + PVPYTHON_ABSOLUTE_DIR + " ../../" + DIRECTORY_TOOLSET
            + "python_current_iteration_forward_plasma_state.py" + " " + PATH_TO_SHARED_FILES_ABSOLUTE;

    try{
        system(&PARAVIEW_ANIMATION[0]);
    } catch(std::exception e) {
        logger::Info("Exception final postprocessing");
        logger::Warning(e.what());
        throw std::runtime_error("Final Postprocessing not working");
    }

    return 0;
}
