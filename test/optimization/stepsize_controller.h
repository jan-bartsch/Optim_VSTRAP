#include <gtest/gtest.h>

#include "../../src/optimization/stepsize_controller.h"

TEST(stepdirection,noSuchMethod) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepsize_controller size_contr = stepsize_controller(filename);

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("control_update");
    subs.insert(std::pair<std::string,std::string>("control_update","FAIL_SAFE"));

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
     unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    provider.setSubroutines(subs);

    size_contr.setData_provider_optim(provider);

    arma::mat gradient(pcell_gp,3,arma::fill::randn);
    arma::mat control(pcell_gp,3,arma::fill::randu);
    double J0 = -1.0;
    arma::mat stepdirection(pcell_gp,3,arma::fill::randn);

    std::vector<particle> inputParticles(ntimesteps_gp);
    double stepsize0 = 0.1;

    int stepsize_flag = size_contr.calculate_stepsize(gradient,J0,control,stepdirection,inputParticles,stepsize0);

    EXPECT_EQ(stepsize_flag,1);
}

TEST(stepdirection,gradientDescentNoThrow) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepsize_controller size_contr = stepsize_controller(filename);

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("control_update");
    subs.insert(std::pair<std::string,std::string>("control_update","gradient_descent"));
    provider.setSubroutines(subs);
    size_contr.setData_provider_optim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
     unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);


    arma::mat gradient(pcell_gp,3,arma::fill::randn);
    arma::mat control_test(pcell_gp,3,arma::fill::randu);
    double J0 = -1.0;
    std::vector<particle> inputParticles(ntimesteps_gp);
    double stepsize0 = 0.1;

    arma::mat stepdirection(pcell_gp,3,arma::fill::randu);
    arma::mat control = control_test;
    double stepsize = 0.1;

    arma::mat updated_control_test = control_test + stepsize*stepdirection;
    int return_flag = size_contr.calculate_stepsize(gradient,J0,control,stepdirection,inputParticles,stepsize0);

    EXPECT_EQ(return_flag,0);
}

TEST(stepdirection,armijoLinesearchNoThrow) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepsize_controller size_contr = stepsize_controller(filename);

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("control_update");
    subs.insert(std::pair<std::string,std::string>("control_update","armijo_linesearch"));
    provider.setSubroutines(subs);
    size_contr.setData_provider_optim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
     unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    arma::mat gradient(pcell_gp,3,arma::fill::randn);
    arma::mat control_test(pcell_gp,3,arma::fill::randu);
    double J0 = -1.0;
    std::vector<particle> inputParticles(ntimesteps_gp);
    double stepsize0 = 0.1;

    arma::mat stepdirection(pcell_gp,3,arma::fill::randu);
    arma::mat control = control_test;
    double stepsize = 0.1;

    arma::mat updated_control_test = control_test + stepsize*stepdirection;
    int return_flag = size_contr.calculate_stepsize(gradient,J0,control,stepdirection,inputParticles,stepsize0);

    EXPECT_EQ(return_flag,2);
}

TEST(stepdirection,successiveApproximationNoThrow) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepsize_controller size_contr = stepsize_controller(filename);

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("control_update");
    subs.insert(std::pair<std::string,std::string>("control_update","successive_approximation"));
    provider.setSubroutines(subs);
    size_contr.setData_provider_optim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
     unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    arma::mat gradient(pcell_gp,3,arma::fill::randn);
    arma::mat control_test(pcell_gp,3,arma::fill::randu);
    double J0 = -1.0;
    std::vector<particle> inputParticles(ntimesteps_gp);
    double stepsize0 = 0.1;

    arma::mat stepdirection(pcell_gp,3,arma::fill::randu);
    arma::mat control = control_test;
    double stepsize = 0.1;

    arma::mat updated_control_test = control_test + stepsize*stepdirection;
    int return_flag = size_contr.calculate_stepsize(gradient,J0,control,stepdirection,inputParticles,stepsize0);

    EXPECT_EQ(return_flag,0);
}
