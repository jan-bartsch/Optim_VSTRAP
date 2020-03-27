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

    optim_controller::start_optimization_iteration(control,input_xml_path);


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

    std::thread t1;
    std::thread t2;


    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    bool zero_control = static_cast<bool>(optimizationParameters.find("start_zero_control")->second);
    unsigned int calculation_functional = static_cast<unsigned int>(optimizationParameters.find("calculation_functional")->second);
    double fixed_gradient_descent_stepsize = static_cast<double>(optimizationParameters.find("fixed_gradient_descent_stepsize")->second);

    std::string BUILD_DIRECTORY_VSTRAP = paths.find("BUILD_DIRECTORY_VSTRAP")->second;
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;

    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;





    std::string START_VSTRAP_FORWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + "input_forward.xml";
    int forward_return = 0.0;
    std::string START_VSTRAP_BACKWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + "input_backward.xml";
    int backward_return = 0.0;

    logger::Info("Reading paramters done");

    if(zero_control == 0) {
        logger::Info("Starting with zero control");
        outController.writeControl_XML(control);
        std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + PATH_TO_SHARED_FILES + "box_coarse.xml" +
                " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field.xml";
        system(&interpolating_control_python[0]);

        outController.writeControl_XML(pow(10,-6)*control);
        std::string interpolating_control_python_adjoint = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + PATH_TO_SHARED_FILES + "box_coarse.xml" +
                " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field_adjoint.xml";
        system(&interpolating_control_python_adjoint[0]);
    }



    /**
     * START OPTIMIZATION ITERATION
     */

    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::unordered_map<coordinate_phase_space_time,double> forwardPDF;
    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);
    std::unordered_map<coordinate_phase_space_time,double> backwardPDF;
    arma::mat gradient(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),2,arma::fill::zeros);
    arma::mat stepDirection(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),2,arma::fill::zeros);
    double value_objective = 0.0;
    double stepsize = 0.0;
    double norm_Gradient = 0.0;

    std::chrono::time_point<std::chrono::system_clock> start, end;

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
            backwardParticles[k-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_batch_1_adjoint_particles_CPU_"+std::to_string(k)+".csv",",");
        }

        start = std::chrono::system_clock::now();
        logger::Info("Assembling pdfs...");
        //        t1 = std::thread(optim_controller::assemblePDF_thread,std::ref(forwardParticles),std::ref(forwardPDF),0,data_provider_opt);
        //        t2 = std::thread(optim_controller::assemblePDF_thread,std::ref(backwardParticles),std::ref(backwardPDF),1,data_provider_opt);
        //        t1.join();
        //        t2.join();

        //std::future<std::unordered_map<coordinate_phase_space_time,double>>

        //std::future<std::unordered_map<coordinate_phase_space_time,double>> forward_thread =
        //        std::async(std::launch::async,optim_controller::assemblePDF_thread,forwardParticles,0,data_provider_opt);

        forwardPDF = pdf_control.assemblingMultiDim(forwardParticles,0);
        backwardPDF = pdf_control.assemblingMultiDim(backwardParticles,1);

        end = std::chrono::system_clock::now();
        logger::Info("Assembling of pdfs took: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>
                                                                  (end-start).count()) + " seconds");



        if (fmod(r,calculation_functional) == 0.0) {
            logger::Info("Calculating functional...");
            value_objective = objective.calculate_objective_L2(forwardPDF,control);
            outDiag.writeDoubleToFile(value_objective,"objectiveTrack");
        }

        logger::Info("Building gradient...");
        gradient = gradient_calculator_opt.calculateGradient_forceControl_space_L2(forwardPDF,backwardPDF,control);
        outDiag.writeDoubleToFile(arma::norm(gradient,"fro"),"normGradientTrack");


        norm_Gradient = arma::norm(gradient,"fro");


        logger::Info("Updating the control...");
        stepDirection = -gradient;
        stepsize = stepsize_contr.get_stepsize(gradient,value_objective,control,stepDirection,forwardParticles[0],
                fixed_gradient_descent_stepsize/std::max(1.0,arma::norm(gradient)));
        outDiag.writeDoubleToFile(stepsize,"stepsizeTrack");


        control = control + stepsize*stepDirection;

        outController.writeControl_XML(control);
        outDiag.writeDoubleToFile(arma::norm(control,"fro"),"normControlTrack");
        std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + PATH_TO_SHARED_FILES + "box_coarse.xml" +
                " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field.xml";
        system(&interpolating_control_python[0]);

        outController.writeControl_XML(pow(10,-4)*control);
        std::string interpolating_control_python_adjoint = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + PATH_TO_SHARED_FILES + "box_coarse.xml" +
                " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field_adjoint.xml";
        system(&interpolating_control_python_adjoint[0]);

        outDiag.writeGradientToFile(gradient,"gradient_"+std::to_string(r));

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


