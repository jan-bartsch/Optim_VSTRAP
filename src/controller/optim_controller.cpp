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

    output_diagnostics outDiag = output_diagnostics();


    std::map<std::string, double> optimizationParameters = data_provider_opt.getOptimizationParameters();
    std::map<std::string, std::string> paths = data_provider_opt.getPaths();



    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);


    std::string BUILD_DIRECTORY_VSTRAP = paths.find("BUILD_DIRECTORY_VSTRAP")->second;
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;

    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;



    std::string START_VSTRAP_FORWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + "/input_forward.xml";
    std::string START_VSTRAP_BACKWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + "/input_backward.xml";



    logger::Info("Reading paramters done");



    /**
     * START OPTIMIZATION ITERATION
     */

    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);
    arma::mat gradient(static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second),2,arma::fill::zeros);
    double value_objective = 0.0;

    unsigned int optimizationIteration_max_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);

    for(unsigned int r = 0; r < optimizationIteration_max_gp; r++) {


        logger::Info("Starting VSTRAP (foward)... ");
        system(&START_VSTRAP_FORWARD[0]);
        logger::Info("Finished VSTRAP... Reading particle files");

        for(unsigned int k = 1; k<=ntimesteps_gp; k++) {
            forwardParticles[k-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_batch_1_electrons_CPU_"+std::to_string(k)+".csv",",");
        }

        logger::Info("Finished reading files...");
        logger::Info("Starting VSTRAP (backward)...");

        system(&START_VSTRAP_BACKWARD[0]);

        logger::Info("Reading particle files...");

        for(unsigned int k = 1; k<=ntimesteps_gp; k++) {
            backwardParticles[k-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_adjoint_particles_batch_1_adjoint_particles_CPU_"+std::to_string(k)+".csv",",");
        }

        logger::Info("Calculating functional...");
        value_objective = objective.calculate_objective_L2(forwardParticles,control);
        outDiag.writeDoubleToFile(value_objective,"objectiveTrack");


        logger::Info("Start building gradient...");
        gradient = gradient_calculator_opt.calculateGradient_forceControl_space_L2(forwardParticles,backwardParticles,control);
        outDiag.writeDoubleToFile(arma::norm(gradient,"fro"),"normGradientTrack");


        logger::Info("Updateing the control...");
        control = control - pow(10,5)*gradient;

        outController.writeControl_XML(control);
        outDiag.writeDoubleToFile(arma::norm(control,"fro"),"normControlTrack");



        std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + PATH_TO_SHARED_FILES + "box_coarse.xml" +
                " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field.xml";
        system(&interpolating_control_python[0]);

        outDiag.writeGradientToFile(gradient,"gradient_"+std::to_string(r));

        logger::Info("Starting " + std::to_string(r+1) + " iteration");
    }



    return 0;
}


