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

    provider.setSubroutines(subs);

    size_contr.setData_provider_optim(provider);

    arma::mat gradient(10,3,arma::fill::randn);
    arma::mat control(10,3,arma::fill::randu);
    double J0 = -1.0;
    arma::mat stepdirection(10,3,arma::fill::randn);

    std::vector<particle> inputParticles(10);
    double stepsize0 = 0.1;

    int stepsize_flag = size_contr.calculate_stepsize(gradient,J0,control,stepdirection,inputParticles,stepsize0);

    EXPECT_EQ(stepsize_flag,1);
}

TEST(stepdirection,gradientDescent) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepsize_controller size_contr = stepsize_controller(filename);

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("control_update");
    subs.insert(std::pair<std::string,std::string>("control_update","gradient_descent"));
    provider.setSubroutines(subs);
    size_contr.setData_provider_optim(provider);

    arma::mat gradient(10,3,arma::fill::randn);
    arma::mat control_test(10,3,arma::fill::randu);
    double J0 = -1.0;
    std::vector<particle> inputParticles(10);
    double stepsize0 = 0.1;

    arma::mat stepdirection(10,3,arma::fill::randu);
    arma::mat control = control_test;
    double stepsize = 0.1;

    arma::mat updated_control_test = control_test + stepsize*stepdirection;

    std::cout << updated_control_test << std::endl;

    size_contr.calculate_stepsize(gradient,J0,control,stepdirection,inputParticles,stepsize0);

    std::cout << control << std::endl;

    double norm_diff = arma::norm(updated_control_test-control);

    EXPECT_LE(norm_diff,pow(10,-5));
}
