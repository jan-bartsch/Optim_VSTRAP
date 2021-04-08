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
    Input Input_control = Input();
    OutputDiagnostics outDiag = OutputDiagnostics();

    EquationSolvingController model_solver = EquationSolvingController();

    //parameters used often
    std::string build_directory_vstrap = MOTIONS::paths::build_directory_vstrap;
    std::string path_to_shared_files = MOTIONS::paths::path_to_shared_files;

    unsigned int ntimesteps_gp = MOTIONS::params::ntimesteps_gp;
    unsigned int dimension_control = MOTIONS::params::dimension_control;
    unsigned int number_cells_position = MOTIONS::params::number_cells_position;
    int simulating_plasma = MOTIONS::params::simulating_plasma;
    int calculation_functional = MOTIONS::params::calculation_functional;


    std::string start_vstrap_forward = build_directory_vstrap + "vstrap" + " " + path_to_shared_files + MOTIONS::paths::input_forward;
    int forward_return = 0;
    std::string START_VSTRAP_BACKWARD = build_directory_vstrap + "vstrap" + " " + path_to_shared_files + MOTIONS::paths::input_backward;
    int backward_return = 0;

    double wasserstein_distance = 0.0;

    logger::Info("Reading paramters done");

    arma::mat control(dimension_control,3,arma::fill::zeros);

    OptimController::Initialize(Input_xml_path,control);



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

    arma::mat gradient(dimension_control,3,arma::fill::zeros);
    arma::mat gradient_old(dimension_control,3,arma::fill::zeros);
    arma::mat stepDirection(number_cells_position,3,arma::fill::zeros);
    arma::mat stepDirection_old(number_cells_position,3,arma::fill::zeros);
    double value_objective = 0.0;
    int stepsize_flag;
    double stepsize = MOTIONS::params::fixed_gradient_descent_stepsize;
    double stepsize_before;
    double norm_Gradient = 0.0;

    std::chrono::time_point<std::chrono::system_clock> start, end;


    for(unsigned int r = 1; r <= MOTIONS::params::optimization_iteration_max; r++) {
        logger::Info("Starting VSTRAP (foward)... ");
        forward_return = model_solver.StartSolvingForward(start_vstrap_forward);
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
        outDiag.writeGradientMatrixToFile(gradient,"gradient_"+std::to_string(r));
        norm_Gradient = arma::norm(gradient,"fro");

        if (r == 0) {
            //save plasma states using initial control
            //Input_control.ReadPlasmaStateForward(forward_particles_initialControl);
            forward_particles_initialControl = forward_particles;
            assembling_flag = pdf_control.AssemblingMultidimParallel(forward_particles,0,pdf_time);
            forwardPDF_initial = pdf_time;
        } else if (fmod(r,MOTIONS::params::calculation_wasserstein) == 0.0) {
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
            if (arma::norm(gradient) < MOTIONS::params::tolerance_gp) {
                throw std::runtime_error("Too small first gradient. Norm was smaller than tolerance_gp <" + std::to_string(MOTIONS::params::tolerance_gp) +">");
            }
            stepsize = MOTIONS::params::fixed_gradient_descent_stepsize/norm_gradient;
        }

        logger::Info("Updating the control...");
        stepDirection = stepdir_contr.GetStepdirection(gradient,gradient_old,stepDirection_old,r);
        stepDirection_old = stepDirection;
        gradient_old = gradient;
        stepsize_before = stepsize;
        stepsize_flag = stepsize_contr.CalculateStepsize(gradient,value_objective,control,
                                                         stepDirection,forward_particles[0],stepsize);
        if (std::fabs(stepsize - stepsize_before) < MOTIONS::params::fabs_tol_gp ) {
            stepsize = 2.0*stepsize; //Increase stepsize if too small (orthwise start with last stepsize)
        }

        std::cout << "Control in iteration " << r << std::endl;
        std::cout << control << std::endl;
        outDiag.writeGradientMatrixToFile(control,"control_"+std::to_string(r));

        if (stepsize_flag == 1) {
            std::string small_stepsize = "Linesearch returned too small stepsize; Found minimum after " + std::to_string(r) + " iterations";
            logger::Info(small_stepsize);
            return 0;
        } else if (stepsize_flag == 2) {
            std::string iteration_depth_reached = "Linesearch reached maximum iteration depth ("
                    + std::to_string(MOTIONS::params::optimization_iteration_max) + "), try to increase tolerance_gp";
            logger::Info(iteration_depth_reached);
            return 1;
        }

        outController.WritecontrolXml(control);
        outDiag.WriteDoubleToFile(arma::norm(control,"fro"),"normControlTrack");
        outController.InterpolateControl();

        logger::Info("Starting post_processing");
        PostProcessingConvergence();
        VisualizeControl();
        ParaviewPlotForward();

        logger::Info("Starting " + std::to_string(r+1) + " iteration");
    }

    return 0;
}

int OptimController::Initialize(const char * Input_xml_path, arma::mat &control)
{
    int zero_control = MOTIONS::params::start_zero_control;
    OutputControlUpdate outController = OutputControlUpdate(Input_xml_path);

    if(zero_control == 1) {
        control = StartWithZeroControl(Input_xml_path);
        GenerateInputFiles(Input_xml_path);
    } else if (zero_control == 0) {
        control = StartWithGivenControl(Input_xml_path);
        std::cout << control << std::endl;
    } else if (zero_control == 2) {
        logger::Info("Starting with zero control but not deleting old files");
        outController.InterpolateControl();
        outController.WritecontrolXml(control);
    }else {
        logger::Info("Starting without control_field_cells");
    }

    /*
     * Check consistency and sanity of Input files
     */
    ParameterSanity ps = ParameterSanity();
    ps.CheckAdjointVelocity();
    ps.CheckAdjointVelocity();

    Calculus calc = Calculus();
    calc.DivergenceVector(control);

    return 0;
}

arma::mat OptimController::StartWithZeroControl(const char *Input_xml_path)
{
    DataProvider data_provider_opt = DataProvider(Input_xml_path);
    OutputControlUpdate outController = OutputControlUpdate(Input_xml_path);

    /*std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;
    std::string BGF_CONTROL = paths.find("BGF_CONTROL")->second;
    std::string CONTROL_FIELD_CELLS_NAME = paths.find("CONTROL_FIELD_CELLS_NAME")->second;
    */


    arma::mat control(MOTIONS::params::number_cells_position,3,arma::fill::zeros);

    logger::Info("Deleting old files");
    std::string COMMAND_RM_RESULTS = "rm -r results/";
    system(&COMMAND_RM_RESULTS[0]);
    std::string COMMAND_MKDIR_RESULTS = "mkdir results && mkdir -p results/" + MOTIONS::paths::mesh_2d_path +
            " && mkdir -p results/"+ MOTIONS::paths::mesh_3d_path;
    system(&COMMAND_MKDIR_RESULTS[0]);
    logger::Info("Starting with zero control");
    outController.WritecontrolXml(control);
    outController.InterpolateControl();

    return control;
}

arma::mat OptimController::StartWithGivenControl(const char *Input_xml_path)
{
    DataProvider data_provider_opt = DataProvider(Input_xml_path);
    OutputControlUpdate outController = OutputControlUpdate(Input_xml_path);

    Input in = Input();
    in.set_DataProviderOptim(data_provider_opt);

    logger::Info("Deleting old .txt and .csv files");
    std::string command_rm_results = "rm *.csv && rm *.txt";
    system(&command_rm_results[0]);

    logger::Info("Starting with existing control (multiplied by a positive constant)");
    std::string read_control = MOTIONS::paths::start_with_existing_control;
    arma::mat control = in.ReadControl(&read_control[0],MOTIONS::params::number_cells_position);
    outController.WritecontrolXml(MOTIONS::params::fraction_of_optimal_control*control);
    outController.InterpolateControl();

    return control;
}

int OptimController::GenerateInputFiles(const char *Input_xml_path)
{
    std::string path_to_shared_files_absolute = MOTIONS::paths::path_to_shared_files_absolute;
    std::string directory_toolset = MOTIONS::paths::directory_toolset;

    std::string generation_string = "python3 " + directory_toolset + "generate_forward_Input.py" + " " + &Input_xml_path[0];
    generation_string += " && python3 " + directory_toolset + "generate_backward_Input.py" + " " + &Input_xml_path[0];
    generation_string += " && python3 " + directory_toolset + "generate_adjoint_particle_creation.py" + " " + &Input_xml_path[0] + " " + path_to_shared_files_absolute;
    logger::Info("Running " + generation_string);
    system(&generation_string[0]);

    return 0;
}

int OptimController::PostProcessingConvergence()
{
    std::string postprocessing_string = "python3 " + MOTIONS::paths::directory_toolset
            + "post_processing_convergence.py " + MOTIONS::paths::path_to_shared_files_absolute;

    logger::Info("Postprocessing ... using command " + postprocessing_string);
    try {
        system(&postprocessing_string[0]);
    } catch (std::exception e) {
        throw std::runtime_error(e.what());
    }
    return 0;
}

int OptimController::VisualizeControl()
{

    std::string path_to_shared_files_absolute = MOTIONS::paths::path_to_shared_files_absolute;
    std::string directory_toolset = MOTIONS::paths::directory_toolset;

    std::string bgf_control = MOTIONS::paths::bgf_control;
    std::string domain_mesh_file = MOTIONS::paths::domain_mesh_file;

    double visalization_scaling = MOTIONS::params::visalization_scaling;

    std::string visualizing_string = "python3 " + directory_toolset + "visualize_control.py "
            + path_to_shared_files_absolute + bgf_control + " ../../Optim_VSTRAP/data/global/"
            + domain_mesh_file + " " + std::to_string(visalization_scaling) + " " + path_to_shared_files_absolute + " "
            + std::to_string(MOTIONS::params::position_max_gp);

    logger::Info("Visualize control ... using command " + visualizing_string);
    try {
        system(&visualizing_string[0]);
    } catch (std::exception e) {
        throw std::runtime_error(e.what());
    }
    return 0;
}

int OptimController::ParaviewPlotForward()
{
    std::string paraview_animation = "cd results && mkdir -p animation && cd animation && "
            + MOTIONS::paths::pvpython_absolute_dir + " ../../" + MOTIONS::paths::directory_toolset
            + "python_current_iteration_forward_plasma_state.py" + " " + MOTIONS::paths::path_to_shared_files_absolute;

    try{
        system(&paraview_animation[0]);
    } catch(std::exception e) {
        logger::Info("Exception final postprocessing");
        logger::Warning(e.what());
        throw std::runtime_error("Final Postprocessing not working");
    }

    return 0;
}
