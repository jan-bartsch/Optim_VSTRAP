#include "optimcontroller.h"

OptimController::OptimController() {
    logger::InitLog();
}

int OptimController::StartOptimizer(int argc, const char **argv)
{
    logger::Info("Starting optimizer...");

    std::string current_directory(get_current_dir_name());
    std::string Input_directory;
    std::string pathToOptimInput;
    const char * Input_xml_path;
    int return_flag_2;

    switch (argc) {
    case 1:
        Input_directory = current_directory + "/Optim_Input.xml";
        Input_xml_path = Input_directory.c_str();
        logger::Info("Starting optimizer with default values: " + Input_directory);
        break;
    case 2:
        Input_xml_path = argv[1];
        break;
    default:
        logger::Warning("Too many Input parameters. Expected 2 but was " + std::to_string(argc));
        return 3;
        //throw std::runtime_error("Too many Input parameters");
    }

    std::chrono::time_point<std::chrono::system_clock> start_optim = std::chrono::system_clock::now();

    int optim_flag = OptimController::MainOptimizationAlgorithm(Input_xml_path);

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

int OptimController::MainOptimizationAlgorithm(const char * Input_xml_path)
{
    DataProvider data_provider_opt = DataProvider(Input_xml_path);
    GradientCalculator gradient_calculator_opt = GradientCalculator(Input_xml_path);
    ObjectiveCalculator objective = ObjectiveCalculator(Input_xml_path);
    OutputControlUpdate outController = OutputControlUpdate(Input_xml_path);
    StepdirectionController stepdir_contr = StepdirectionController(Input_xml_path);
    StepsizeController stepsize_contr = StepsizeController(Input_xml_path);

    PdfController pdf_control = PdfController();
    pdf_control.set_DataProviderOptim(data_provider_opt);

    Input Input_control = Input();
    Input_control.set_DataProviderOptim(data_provider_opt);

    OutputDiagnostics outDiag = OutputDiagnostics();
    outDiag.set_DataProviderOptim(data_provider_opt);

    EquationSolvingController model_solver = EquationSolvingController();
    model_solver.set_DataProviderOptim(data_provider_opt);

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
    std::string Input_FORWARD = paths.find("Input_FORWARD")->second;
    std::string Input_BACKWARD = paths.find("Input_BACKWARD")->second;

    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;


    std::string START_VSTRAP_FORWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + Input_FORWARD;
    int forward_return = 0;
    std::string START_VSTRAP_BACKWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + Input_BACKWARD;
    int backward_return = 0;

    double wasserstein_distance = 0.0;

    logger::Info("Reading paramters done");

    arma::mat control(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),3,arma::fill::zeros);
    if(zero_control == 1) {
        control = StartWithZeroControl(Input_xml_path);
        GenerateInputFiles(Input_xml_path);
    } else if (zero_control == 0) {
        control = StartWithGivenControl(Input_xml_path);
        std::cout << control << std::endl;
    } else if (zero_control == 2) {
        logger::Info("Starting with zero control but not deleting old files");
        outController.InterpolateControl(data_provider_opt);
        outController.WritecontrolXml(control);
    }else {
        logger::Info("Starting without control_field_cells");
    }

    /*
     * Check consistency and sanity of Input files
     */
    ParameterSanity ps = ParameterSanity();
    ps.CheckAdjointVelocity(data_provider_opt);
    ps.CheckAdjointVelocity(data_provider_opt);

    Calculus calc = Calculus();
    calc.set_DataProviderOptim(data_provider_opt);
    calc.DivergenceVector(control);



    /**
     * START OPTIMIZATION ITERATION
     */
    std::vector<std::vector<Particle>> forward_particles(ntimesteps_gp);
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double>> forwardPDF;
    std::vector<std::vector<Particle>> forward_particles_electrons(ntimesteps_gp);
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double>> forwardPDF_electrons;

    std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double>> forwardPDF_initial; //Needed for Wasserstein
    std::vector<std::vector<Particle>> forward_particles_initialControl(ntimesteps_gp); //Needed for calculation of Wasserstein distance

    std::vector<std::vector<Particle>> backward_particles(ntimesteps_gp);
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double>> backwardPDF;
    std::vector<std::vector<Particle>> backward_particles_electrons(ntimesteps_gp);
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double>> backwardPDF_electrons;

    std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double>> pdf_time(ntimesteps_gp);
    int assembling_flag;

    arma::mat gradient(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),3,arma::fill::zeros);
    arma::mat gradient_old(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),3,arma::fill::zeros);
    arma::mat stepDirection(static_cast<unsigned int>(optimizationParameters.find("number_cells_position")->second),3,arma::fill::zeros);
    arma::mat stepDirection_old(static_cast<unsigned int>(optimizationParameters.find("number_cells_position")->second),3,arma::fill::zeros);
    double value_objective = 0.0;
    int stepsize_flag;
    double stepsize = fixed_gradient_descent_stepsize;
    double stepsize_before;
    double norm_Gradient = 0.0;

    std::chrono::time_point<std::chrono::system_clock> start, end;

    unsigned int optimizationIteration_max_gp = static_cast<unsigned int>(optimizationParameters.find("optimizationIteration_max_gp")->second);

    for(unsigned int r = 1; r <= optimizationIteration_max_gp; r++) {
        logger::Info("Starting VSTRAP (foward)... ");
        forward_return = model_solver.StartSolvingForward(START_VSTRAP_FORWARD);
        if (forward_return != 0) {
            logger::Info("Forward VSTRAP returned non-zero value: " + std::to_string(forward_return));
            throw  std::system_error();
        }
        logger::Info("Finished VSTRAP");

        logger::Info("Reading particle files");
        Input_control.ReadPlasmaStateForward(forward_particles,"plasma_state_batch_1_forward_particles_CPU_");
        if (simulating_plasma==1) {
            Input_control.ReadPlasmaStateForward(forward_particles_electrons,"plasma_state_batch_e_forward_particles_electrons_CPU_");
        }

        logger::Info("Finished reading files...");
        logger::Info("Starting VSTRAP (backward)...");



        backward_return = model_solver.StartSolvingBackward(START_VSTRAP_BACKWARD);
        if (backward_return != 0)  {
            logger::Info("Backward VSTRAP returned non-zero value: " + std::to_string(backward_return));
            throw std::system_error();
        }
        logger::Info("Finished backward VSTRAP");



        logger::Info("Reading particle files...");
        Input_control.ReadPlasmaStateBackward(backward_particles,"plasma_state_batch_1_adjoint_particles_CPU_");
        if (simulating_plasma == 1) {
            Input_control.ReadPlasmaStateBackward(backward_particles_electrons,"plasma_state_batch_e_adjoint_particles_electrons_CPU_");
        }


        logger::Info("Assembling pdfs...");
        start = std::chrono::system_clock::now();
        assembling_flag = pdf_control.AssemblingMultidimParallel(forward_particles,0,pdf_time);
        forwardPDF = pdf_time;
        assembling_flag = pdf_control.AssemblingMultidimParallel(backward_particles,1,pdf_time);
        backwardPDF = pdf_time;

        if (simulating_plasma == 1) {
            assembling_flag = pdf_control.AssemblingMultidimParallel(forward_particles_electrons,0,pdf_time);
            forwardPDF_electrons = pdf_time;
            assembling_flag = pdf_control.AssemblingMultidimParallel(backward_particles_electrons,1,pdf_time);
            backwardPDF_electrons = pdf_time;
        }
        end = std::chrono::system_clock::now();
        logger::Info("Assembling of pdfs took: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>
                                                                  (end-start).count()) + " second(s)");



        logger::Info("Building gradient...");
        if (simulating_plasma == 1) {
            gradient = gradient_calculator_opt.CalculategradientForcecontrolSpaceHmPlasma(forwardPDF,backwardPDF,forwardPDF_electrons,backwardPDF_electrons,control);
        } else {
            gradient = gradient_calculator_opt.CalculategradientForcecontrolSpaceHm(forwardPDF,backwardPDF,control);
        }
        outDiag.WriteDoubleToFile(arma::norm(gradient,"fro"),"normGradientTrack");
        outDiag.WriteGradientMatrixToFile(gradient,"gradient_"+std::to_string(r));
        norm_Gradient = arma::norm(gradient,"fro");

        if (r == 0) {
            //save plasma states using initial control
            //Input_control.ReadPlasmaStateForward(forward_particles_initialControl);
            forward_particles_initialControl = forward_particles;
            assembling_flag = pdf_control.AssemblingMultidimParallel(forward_particles,0,pdf_time);
            forwardPDF_initial = pdf_time;
        } else if (fmod(r,calculation_wasserstein) == 0.0) {
            wasserstein_distance = pdf_control.CalculateWassersteinMetric(forward_particles_initialControl,forward_particles);
            //wasserstein_distance = pdf_control.calculate_wasserstein_metric_histogramm(forwardPDF_initial,forwardPDF);
            std::cout << "Wasserstein distance: " << wasserstein_distance << std::endl;
            outDiag.WriteDoubleToFile(wasserstein_distance,"wassersteinDistanceTrack");
        } else {
            std::cout << "No calculation of wasserstein distance" << std::endl;
        }

        if (calculation_functional == 0) {
            logger::Info("No calculation of functional");
        }
        else if (fmod(r,calculation_functional) == 0.0) {
            logger::Info("Calculating functional...");
            start = std::chrono::system_clock::now();
            value_objective = objective.CalculateObjective(forwardPDF,control);
            end = std::chrono::system_clock::now();
            logger::Info("Calculation of functional took: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>
                                                                             (end-start).count()) + " second(s)");
            outDiag.WriteDoubleToFile(value_objective,"objectiveTrack");
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
        stepDirection = stepdir_contr.GetStepdirection(gradient,gradient_old,stepDirection_old,r);
        stepDirection_old = stepDirection;
        gradient_old = gradient;
        stepsize_before = stepsize;
        stepsize_flag = stepsize_contr.CalculateStepsize(gradient,value_objective,control,
                                                          stepDirection,forward_particles[0],stepsize);
        if (std::fabs(stepsize - stepsize_before) < fabs_tol_gp ) {
            stepsize = 2.0*stepsize; //Increase stepsize if too small (orthwise start with last stepsize)
        }

        std::cout << "Control in iteration " << r << std::endl;
        std::cout << control << std::endl;
        outDiag.WriteGradientMatrixToFile(control,"control_"+std::to_string(r));

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

        outController.WritecontrolXml(control);
        outDiag.WriteDoubleToFile(arma::norm(control,"fro"),"normControlTrack");
        outController.InterpolateControl(data_provider_opt);

        logger::Info("Starting post_processing");
        PostProcessingConvergence(data_provider_opt);
        VisualizeControl(data_provider_opt);
        ParaviewPlotForward(data_provider_opt);

        logger::Info("Starting " + std::to_string(r+1) + " iteration");
    }

    return 0;
}

/*int optim_controller::check_Input_py(data_provider provider, const char *filePathOptimInput)
{
    std::map<std::string, std::string> paths = provider.getPaths();
    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string Input_FORWARD = paths.find("Input_FORWARD")->second;
    std::string Input_BACKWARD = paths.find("Input_BACKWARD")->second;

    std::string CHECK_Input_PYHTON = "python3 " + DIRECTORY_TOOLSET + "check_Input.py "
            + PATH_TO_SHARED_FILES + Input_FORWARD + " " + PATH_TO_SHARED_FILES
            + Input_BACKWARD + " " + &filePathOptimInput[0];

    int check_Input_flag = system(&CHECK_Input_PYHTON[0]);

    //output is in 16 bit value
    if(check_Input_flag == 512) {
        throw std::runtime_error("File not found in Python check");
    } else if (check_Input_flag == 256) {
        throw std::runtime_error("Encountered inconsitency in Input files OR File not found");
    } else {
        logger::Info("Python check: **ALL CLEAR**");
    }

    return check_Input_flag;
}*/

arma::mat OptimController::StartWithZeroControl(const char *Input_xml_path)
{
    DataProvider data_provider_opt = DataProvider(Input_xml_path);
    OutputControlUpdate outController = OutputControlUpdate(Input_xml_path);

    std::map<std::string, double> optimizationParameters = data_provider_opt.getOptimizationParameters();
    std::map<std::string, std::string> paths = data_provider_opt.getPaths();
    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;
    std::string BGF_CONTROL = paths.find("BGF_CONTROL")->second;
    std::string CONTROL_FIELD_CELLS_NAME = paths.find("CONTROL_FIELD_CELLS_NAME")->second;

    unsigned int number_cells_position = static_cast<unsigned int>(optimizationParameters.find("number_cells_position")->second);

    arma::mat control(number_cells_position,3,arma::fill::zeros);

    logger::Info("Deleting old files");
    std::string COMMAND_RM_RESULTS = "rm -r results/";
    system(&COMMAND_RM_RESULTS[0]);
    std::string COMMAND_MKDIR_RESULTS = "mkdir results && mkdir -p results/" + paths.find("mesh_2d_path")->second +
            " && mkdir -p results/"+ paths.find("mesh_3d_path")->second;
    system(&COMMAND_MKDIR_RESULTS[0]);
    logger::Info("Starting with zero control");
    outController.WritecontrolXml(control);
    outController.InterpolateControl(data_provider_opt);

    return control;
}

arma::mat OptimController::StartWithGivenControl(const char *Input_xml_path)
{
    DataProvider data_provider_opt = DataProvider(Input_xml_path);
    OutputControlUpdate outController = OutputControlUpdate(Input_xml_path);

    Input in = Input();
    in.set_DataProviderOptim(data_provider_opt);


    std::map<std::string, double> optimizationParameters = data_provider_opt.getOptimizationParameters();
    std::map<std::string, std::string> paths = data_provider_opt.getPaths();
    std::string START_WITH_EXISTING_CONTROL = paths.find("START_WITH_EXISTING_CONTROL")->second;
    double fraction_of_optimal_control = static_cast<double>(optimizationParameters.find("fraction_of_optimal_control")->second);
     int number_cells_position = static_cast<int>(optimizationParameters.find("number_cells_position")->second);

    logger::Info("Deleting old .txt and .csv files");
    std::string COMMAND_RM_RESULTS = "rm *.csv && rm *.txt";
    system(&COMMAND_RM_RESULTS[0]);

    logger::Info("Starting with existing control (multiplied by a positive constant)");
    std::string READ_CONTROL = START_WITH_EXISTING_CONTROL;
    arma::mat control = in.ReadControl(&READ_CONTROL[0],number_cells_position);
    outController.WritecontrolXml(fraction_of_optimal_control*control);
    outController.InterpolateControl(data_provider_opt);

    return control;
}

int OptimController::GenerateInputFiles(const char *Input_xml_path)
{
    DataProvider data_provider_opt = DataProvider(Input_xml_path);

    std::map<std::string, std::string> paths = data_provider_opt.getPaths();
    std::string PATH_TO_SHARED_FILES_ABSOLUTE = paths.find("PATH_TO_SHARED_FILES_ABSOLUTE")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;

    std::string generation_string = "python3 " + DIRECTORY_TOOLSET + "generate_forward_Input.py" + " " + &Input_xml_path[0];
    generation_string += " && python3 " + DIRECTORY_TOOLSET + "generate_backward_Input.py" + " " + &Input_xml_path[0];
    generation_string += " && python3 " + DIRECTORY_TOOLSET + "generate_adjoint_particle_creation.py" + " " + &Input_xml_path[0] + " " + PATH_TO_SHARED_FILES_ABSOLUTE;
    logger::Info("Running " + generation_string);
    system(&generation_string[0]);

    return 0;
}

int OptimController::PostProcessingConvergence(DataProvider provider)
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

int OptimController::VisualizeControl(DataProvider provider)
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
            + std::to_string(parameters.find("position_max_gp")->second);

    logger::Info("Visualize control ... using command " + VISUALIZING_STRING);
    try {
        system(&VISUALIZING_STRING[0]);
    } catch (std::exception e) {
        throw std::runtime_error(e.what());
    }
    return 0;
}

int OptimController::ParaviewPlotForward(DataProvider provider)
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
