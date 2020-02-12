#include "optim_controller.h"



optim_controller::optim_controller() {
    logger::InitLog();
    logger::Info("Reading parameters ...");
    this->setOPTIMIZATION_PARAMETERS( initializer::read_optimization_parameters("/home/bartsch/SPARC/Optim_VSTRAP/data/Optim_input.xml") );
}

void optim_controller::start_optimizer(int argc, const char **argv)
{

    logger::Info("Starting optimizer...");

    std::vector<double> control;
    std::string current_directory(get_current_dir_name());
    std::string input_directory;
    const char * input_xml_path;

    switch (argc) {
    case 1:
         input_directory = current_directory + "/Optim_input.xml";
         input_xml_path = input_directory.c_str();
        break;
    case 2:
        input_xml_path = argv[1];
        break;
    default:
        throw std::runtime_error("Too many input parameters");
    }

    optim_controller::start_optimization_iteration(control,input_xml_path);


}

int optim_controller::start_optimization_iteration(std::vector<double> &control, const char * input_xml_path)
{

    input in = input();
    std::map<std::string, double> globalParameters = initializer::read_optimization_parameters(input_xml_path);
    std::map<std::string, std::string> inputParameters = initializer::read_input_parameters(input_xml_path);

    unsigned int ntimesteps_gp = static_cast<unsigned int>(globalParameters.find("ntimesteps_gp")->second);

    std::string BUILD_DIRECTORY_VSTRAP = inputParameters.find("BUILD_DIRECTORY_VSTRAP")->second;
    std::string BUILD_DIRECTORY_OPTIM = inputParameters.find("BUILD_DIRECTORY_OPTIM")->second;

    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);



    //std::string PARAM_VSTRAP = "../../vstrap/data/particle_initializer/unit_test/input.xml"; //will be an input parameter
    std::string PARAM_VSTRAP_FORWARD = inputParameters.find("PARAM_VSTRAP_FORWARD")->second;
    std::string PARAM_VSTRAP_BACKWARD = inputParameters.find("PARAM_VSTRAP_BACKWARD")->second;

    std::string START_VSTRAP_FORWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PARAM_VSTRAP_FORWARD;
    std::string START_VSTRAP_BACKWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PARAM_VSTRAP_BACKWARD;

    logger::Info("Starting VSTRAP (foward)... with parameter"+PARAM_VSTRAP_FORWARD);

    system(&START_VSTRAP_FORWARD[0]);

    logger::Info("Finished VSTRAP... Reading particle files");

    for(unsigned int k = 1; k<ntimesteps_gp; k++) {
        forwardParticles[k] = in.readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_electrons_CPU_"+std::to_string(k)+".csv",",");
    }

    logger::Info("Finished reading files...");
    logger::Info("Starting VSTRAP (backward)...");

    system(&START_VSTRAP_BACKWARD[0]);

    logger::Info("Finished VSTRAP... Reading particle files");

    for(unsigned int k = 1; k<ntimesteps_gp; k++) {
        backwardParticles[k] = in.readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_adjoint_particles_CPU_"+std::to_string(k)+".csv",",");
    }



    return 0;
}

std::map<std::string, double> optim_controller::getOPTIMIZATION_PARAMETERS() const
{
    return OPTIMIZATION_PARAMETERS;
}

void optim_controller::setOPTIMIZATION_PARAMETERS(const std::map<std::string, double> &value)
{
    OPTIMIZATION_PARAMETERS = value;
}

std::map<std::string, std::string> optim_controller::getINPUT_PARAMETERS() const
{
    return INPUT_PARAMETERS;
}

void optim_controller::setINPUT_PARAMETERS(const std::map<std::string, std::string> &value)
{
    INPUT_PARAMETERS = value;
}

