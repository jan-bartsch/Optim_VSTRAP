#include "optim_controller.h"



optim_controller::optim_controller() {
    std::cout << "Initialising optim_controller" << std::endl;
}

void optim_controller::start_optimizer(int argc, const char **argv)
{


    logger::Info("Starting optimizer...");

    arma::mat control(64,3,arma::fill::zeros);
    std::string current_directory(get_current_dir_name());
    std::string input_directory;
    const char * input_xml_path;

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
        throw std::runtime_error("Too many input parameters");
    }

    std::chrono::time_point<std::chrono::system_clock> start_optim = std::chrono::system_clock::now();

    int optim_flag = optim_controller::start_optimization_iteration(control,input_xml_path);

    std::chrono::time_point<std::chrono::system_clock> end_optim = std::chrono::system_clock::now();
    logger::Info("Optimization took: " + std::to_string(std::chrono::duration_cast<std::chrono::minutes>
                                                        (end_optim-start_optim).count()) + " minutes");

    if (optim_flag == 0) {
        logger::Info("Optimization ended without errors");
    } else {
        std::string return_string = "Optimization returned non-zero value: " + std::to_string(optim_flag);
        logger::Info(return_string);
    }

}

int optim_controller::start_optimization_iteration(arma::mat &control, const char * input_xml_path)
{

    data_provider data_provider_opt = data_provider(input_xml_path);
    gradient_calculator gradient_calculator_opt = gradient_calculator(input_xml_path);
    objective_calculator objective = objective_calculator(input_xml_path);
    output_control_update outController = output_control_update(input_xml_path);
    stepdirection_controller stepdir_contr = stepdirection_controller(input_xml_path);
    stepsize_controller stepsize_contr = stepsize_controller(input_xml_path);

    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(data_provider_opt);

    output_diagnostics outDiag = output_diagnostics();


    std::map<std::string, double> optimizationParameters = data_provider_opt.getOptimizationParameters();
    std::map<std::string, std::string> paths = data_provider_opt.getPaths();
    std::map<std::string,std::string> subroutines = data_provider_opt.getSubroutines();

    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    bool zero_control = static_cast<bool>(optimizationParameters.find("start_zero_control")->second);
    unsigned int calculation_functional = static_cast<unsigned int>(optimizationParameters.find("calculation_functional")->second);
    double fixed_gradient_descent_stepsize = static_cast<double>(optimizationParameters.find("fixed_gradient_descent_stepsize")->second);
    double fraction_of_optimal_control = static_cast<double>(optimizationParameters.find("fraction_of_optimal_control")->second);


    std::string BUILD_DIRECTORY_VSTRAP = paths.find("BUILD_DIRECTORY_VSTRAP")->second;
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;

    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;


    std::string START_VSTRAP_FORWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + "input_forward.xml";
    int forward_return = 0.0;
    std::string START_VSTRAP_BACKWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + "input_backward.xml";
    int backward_return = 0.0;

    logger::Info("Reading paramters done");

    if(zero_control == 0) {
        logger::Info("Starting with zero control");
        outController.writeControl_XML(control);
        std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + DOMAIN_MESH +
                " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field.xml";
        system(&interpolating_control_python[0]);
    } else {
        logger::Info("Starting with existing control (multiplied by a positive constant)");
        std::string READ_CONTROL = PATH_TO_SHARED_FILES + "control_field_cells_optimal.xml";
        control = input::readControl(&READ_CONTROL[0]);
        outController.writeControl_XML(fraction_of_optimal_control*control);
        std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + DOMAIN_MESH +
                " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field.xml";
        system(&interpolating_control_python[0]);
        std::cout << control << std::endl;
    }


    /**
     * START OPTIMIZATION ITERATION
     */

    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::unordered_map<coordinate_phase_space_time,double> forwardPDF;
    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);
    std::unordered_map<coordinate_phase_space_time,double> backwardPDF;
    arma::mat gradient(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),3,arma::fill::zeros);
    arma::mat stepDirection(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),3,arma::fill::zeros);
    double value_objective = 0.0;
    int stepsize_flag;
    double norm_Gradient = 0.0;

    std::chrono::time_point<std::chrono::system_clock> start, end, end1;

    unsigned int optimizationIteration_max_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);

    for(unsigned int r = 0; r < optimizationIteration_max_gp; r++) {

        logger::Info("Starting VSTRAP (foward)... ");
        forward_return = system(&START_VSTRAP_FORWARD[0]);
        if (forward_return != 0) {
            logger::Info("Forward VSTRAP returned non-zero value: " + std::to_string(forward_return));
            throw  std::system_error();
        }
        logger::Info("Finished VSTRAP... Reading particle files");

        for(unsigned int k = 1; k<=ntimesteps_gp; k++) {
            forwardParticles[k-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_batch_1_forward_particles_CPU_"+std::to_string(k)+".csv",",");
        }

        logger::Info("Finished reading files...");
        logger::Info("Starting VSTRAP (backward)...");

        backward_return  = system(&START_VSTRAP_BACKWARD[0]);
        if (backward_return != 0)  {
            logger::Info("Backward VSTRAP returned non-zero value: " + std::to_string(backward_return));
            throw std::system_error();
        }

        logger::Info("Reading particle files...");

        for(unsigned int k = 1; k<=ntimesteps_gp; k++) {
            backwardParticles[ntimesteps_gp - k] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_batch_1_adjoint_particles_CPU_"+std::to_string(k)+".csv",",");
        }

        start = std::chrono::system_clock::now();
        logger::Info("Assembling pdfs...");
        // t1 = std::thread(optim_controller::assemblePDF_thread,std::ref(forwardParticles),std::ref(forwardPDF),0,data_provider_opt);
        //t2 = std::thread(optim_controller::assemblePDF_thread,std::ref(backwardParticles),std::ref(backwardPDF),1,data_provider_opt);
        //t1.join();
        //t2.join();


        std::future<std::unordered_map<coordinate_phase_space_time,double>> forward_thread =
                std::async(std::launch::async,optim_controller::assemblePDF_thread,std::ref(forwardParticles),0,data_provider_opt);
        std::future<std::unordered_map<coordinate_phase_space_time,double>> backward_thread =
                std::async(std::launch::async,optim_controller::assemblePDF_thread,std::ref(backwardParticles),0,data_provider_opt);

        forwardPDF = forward_thread.get();
        backwardPDF = backward_thread.get();

        end1 = std::chrono::system_clock::now();

        logger::Info("Assembling of pdfs (parallel) took: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>
                                                                  (end1-start).count()) + " seconds");

//        start = std::chrono::system_clock::now();

//        forwardPDF = pdf_control.assemblingMultiDim(forwardParticles,0);
//        backwardPDF = pdf_control.assemblingMultiDim(backwardParticles,1);

//        end = std::chrono::system_clock::now();
//        logger::Info("Assembling of pdfs took: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>
//                                                                  (end-start).count()) + " seconds");

        if (calculation_functional == 0) {
            logger::Info("No calculation of functional");
        }
        else if (fmod(r,calculation_functional) == 0.0) {
            logger::Info("Calculating functional...");
            value_objective = objective.calculate_objective_L2(forwardPDF,control);
            outDiag.writeDoubleToFile(value_objective,"objectiveTrack");
        }

        //        forward_return = system(&START_VSTRAP_FORWARD[0]);
        //        if (forward_return != 0) {
        //            logger::Info("Forward VSTRAP returned non-zero value: " + std::to_string(forward_return));
        //            throw  std::system_error();
        //        }
        //        for(unsigned int k = 1; k<=ntimesteps_gp; k++) {
        //            forwardParticles[k-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_batch_1_forward_particles_CPU_"+std::to_string(k)+".csv",",");
        //        }
        //        forwardPDF = pdf_control.assemblingMultiDim(forwardParticles,0);
        //        double value_objective2 = objective.calculate_objective_L2(forwardPDF,control);


        logger::Info("Building gradient...");
        gradient = gradient_calculator_opt.calculateGradient_forceControl_space_L2(forwardPDF,backwardPDF,control);
        outDiag.writeDoubleToFile(arma::norm(gradient,"fro"),"normGradientTrack");
        outDiag.writeGradientToFile(gradient,"gradient_"+std::to_string(r));


        norm_Gradient = arma::norm(gradient,"fro");


        logger::Info("Updating the control...");
        stepDirection = -gradient;
        stepsize_flag = stepsize_contr.calculate_stepsize(gradient,value_objective,control,stepDirection,forwardParticles[0],
                fixed_gradient_descent_stepsize/std::max(1.0,arma::norm(gradient)));

        if (stepsize_flag == 1) {
            std::string small_stepsize = "Linesearch returned too small stepsize; Found minimum after " + std::to_string(r+1) + " iterations";
            logger::Info(small_stepsize);
            //return 0;
        } else if (stepsize_flag == 2) {
            std::string iteration_depth_reached = "Linesearch reached maximum iteration depth ("
                    + std::to_string(optimizationIteration_max_gp) + "), try to increase tolerance_gp";
            //return 1;
        }

        outController.writeControl_XML(control);
        outDiag.writeDoubleToFile(arma::norm(control,"fro"),"normControlTrack");
        std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + DOMAIN_MESH +
                " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field.xml";
        system(&interpolating_control_python[0]);


        logger::Info("Starting " + std::to_string(r+1) + " iteration");
    }



    return 0;
}

std::unordered_map<coordinate_phase_space_time,double> optim_controller::assemblePDF_thread(std::vector<std::vector<particle>> &particles, unsigned int equation_type, data_provider data_provider_)
{
    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(data_provider_);

    std::unordered_map<coordinate_phase_space_time,double> particlePDF = pdf_control.assemblingMultiDim(particles,equation_type);

    return particlePDF;
}

int optim_controller::test(int testing)
{
    return testing;
}


