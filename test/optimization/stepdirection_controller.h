#include <gtest/gtest.h>

#include "../../src/optimization/stepdirection_controller.h"

TEST(stepdirection,negativeGradient) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepdirection_controller stepdir_contr = stepdirection_controller(filename);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);

    arma::mat stepDirection;
    unsigned int r;

    arma::mat gradient(dimensionOfControl_gp,dimensionOfControl_gp,arma::fill::randu);
    stepDirection = stepdir_contr.get_stepdirection(gradient,gradient,stepDirection,r);

    double norm_difference = arma::norm(stepDirection-(-gradient),"fro");
    ASSERT_EQ(norm_difference,0.0);
}

TEST(ncg,noException) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepdirection_controller stepdir_contr = stepdirection_controller(filename);

    unsigned int r = 1;

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);

    arma::mat gradient(dimensionOfControl_gp,3,arma::fill::randn);
    arma::mat gradient_old(dimensionOfControl_gp,3,arma::fill::randu);
    arma::mat stepdirectionOld(dimensionOfControl_gp,3,arma::fill::randu);

    ASSERT_NO_THROW(stepdir_contr.ncg_scheme_FR(gradient, gradient_old, stepdirectionOld, r));
    ASSERT_NO_THROW(stepdir_contr.ncg_scheme_PR(gradient, gradient_old, stepdirectionOld, r));
    ASSERT_NO_THROW(stepdir_contr.ncg_scheme_HZ(gradient, gradient_old, stepdirectionOld, r));
}
