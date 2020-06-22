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
