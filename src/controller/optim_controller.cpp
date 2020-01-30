#include "optim_controller.h"



optim_controller::optim_controller() {
    logger::InitLog();
    logger::Info("Reading parameters ...");
    this->setOPTIMIZATION_PARAMETERS( initializer::read_parameters("/") );
}

void optim_controller::start_optimizer()
{

    logger::Info("Starting optimizer...");

    std::vector<double> control;

    optim_controller::start_optimization_iteration(control);


}

int optim_controller::start_optimization_iteration(std::vector<double> &control)
{

    std::string BUILD_DIRECTORY_VSTRAP = "/home/bartsch/SPARC/build-vstrap-Desktop-Vorgabe/src/"; //will be an input parameter


    logger::Info("Starting VSTRAP (foward)...");


    std::string PARAM_VSTRAP = "../../vstrap/data/particle_initializer/unit_test/input.xml"; //will be an input parameter
    std::string START_VSTRAP = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PARAM_VSTRAP;
    system(&START_VSTRAP[0]);

    logger::Info("Finished VSTRAP... Starting optimization iteration");

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
