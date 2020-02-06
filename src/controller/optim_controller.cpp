#include "optim_controller.h"



optim_controller::optim_controller() {
    logger::InitLog();
    logger::Info("Reading parameters ...");
    this->setOPTIMIZATION_PARAMETERS( initializer::read_parameters("/home/bartsch/SPARC/Optim_VSTRAP/data/Optim_input.xml") );
}

void optim_controller::start_optimizer()
{

    logger::Info("Starting optimizer...");

    std::vector<double> control;

    optim_controller::start_optimization_iteration(control);


}

int optim_controller::start_optimization_iteration(std::vector<double> &control)
{

    input in = input();
    std::map<std::string, double> globalParameters = initializer::read_parameters("/home/bartsch/SPARC/Optim_VSTRAP/data/Optim_input.xml");

    std::string BUILD_DIRECTORY_VSTRAP = "/home/bartsch/SPARC/build-vstrap-Desktop-Debug/src/"; //will be an input parameter
    std::string BUILD_DIRECTORY_OPTIM = "/home/bartsch/SPARC/build-Optim_VSTRAP-Desktop-Debug/src/";

    std::vector<std::vector<particle>> forwardParticles(globalParameters.find("ntimesteps_gp")->second, std::vector<particle>(20));



    //std::string PARAM_VSTRAP = "../../vstrap/data/particle_initializer/unit_test/input.xml"; //will be an input parameter
    std::string PARAM_VSTRAP = "../../vstrap/data/bgf/first_test/input_b.xml";
    std::string START_VSTRAP = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PARAM_VSTRAP;

    logger::Info("Starting VSTRAP (foward)... with parameter"+PARAM_VSTRAP);

    system(&START_VSTRAP[0]);

    logger::Info("Finished VSTRAP... Reading particle files");

    for(int k = 1; k<20; k++) {
        forwardParticles[k] = in.readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_electrons_CPU_"+std::to_string(k)+".csv",",");
    }

    logger::Info("Finished reading files...");
    logger::Info("Starting VSTRAP (backward)...");

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

