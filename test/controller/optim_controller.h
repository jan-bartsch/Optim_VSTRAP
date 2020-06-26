#include <gtest/gtest.h>

#include "../../src/controller/optim_controller.h"
#include "../../src/objects/data_provider.h"
#include "../../src/io/output_control_update.h"

TEST(optimContr,startup) {
    optim_controller contr = optim_controller();

    int argc = 3;
    const char **argv;
    argv[0] = "/home/jan/Promotion_linuxPC/build-Optim/src/Optim_VSTRAP_CMAKE";
    argv[1] = "../../Optim_VSTRAP/data/box_shifting_tests/Optim_input.xml";
    argv[2] = "too much";

    ASSERT_ANY_THROW(contr.start_optimizer(argc,argv));
}

TEST(optimContr,interpolateControl) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    optim_controller contr = optim_controller();

    ASSERT_NO_THROW(contr.interpolate_control(provider));
}

TEST(optimContr,startZeroControl) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    optim_controller contr = optim_controller();

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    arma::mat zero(dimensionOfControl_gp,3,arma::fill::zeros);

    arma::mat control = contr.start_with_zero_control(filename);

    double norm_difference = arma::norm(zero-control);

    ASSERT_LE(norm_difference,pow(10,-10));
}

TEST(optContr,startReadInControl) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    optim_controller contr = optim_controller();

    output_control_update updater = output_control_update();
    updater.setData_provider_optim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    arma::mat control_rand(dimensionOfControl_gp,3,arma::fill::randu);

    updater.writeControl_XML(control_rand);

    std::cout << control_rand << std::endl;

    arma::mat control = contr.start_with_given_control(filename);

    std::cout << control << std::endl;

    double norm_difference = arma::norm(control_rand-control);

    ASSERT_LE(norm_difference,pow(10,-5));
}
