#include <gtest/gtest.h>

#include "../../src/optimization/stepdirection_controller.h"

TEST(stepdirection,negativeGradient) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepdirection_controller stepdir_contr = stepdirection_controller(filename);

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("direction_update");
    subs.insert(std::pair<std::string,std::string>("direction_update","negative_gradient"));
    provider.setSubroutines(subs);
    stepdir_contr.setData_provider_optim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);

    arma::mat stepDirection;
    unsigned int r;

    arma::mat gradient(dimensionOfControl_gp,dimensionOfControl_gp,arma::fill::randu);
    stepDirection = stepdir_contr.get_stepdirection(gradient,gradient,stepDirection,r);

    double norm_difference = arma::norm(stepDirection-(-gradient),"fro");
    ASSERT_EQ(norm_difference,0.0);
}

TEST(ncgFR,noException) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepdirection_controller stepdir_contr = stepdirection_controller(filename);

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("direction_update");
    subs.insert(std::pair<std::string,std::string>("direction_update","ncg_FR"));
    provider.setSubroutines(subs);
    stepdir_contr.setData_provider_optim(provider);

    unsigned int r = 1;

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);

    arma::mat gradient(dimensionOfControl_gp,3,arma::fill::randn);
    arma::mat gradient_old(dimensionOfControl_gp,3,arma::fill::randu);
    arma::mat stepdirectionOld(dimensionOfControl_gp,3,arma::fill::randu);

    ASSERT_NO_THROW(stepdir_contr.get_stepdirection(gradient, gradient_old, stepdirectionOld, r));
}

TEST(ncgPR,noException) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepdirection_controller stepdir_contr = stepdirection_controller(filename);

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("direction_update");
    subs.insert(std::pair<std::string,std::string>("direction_update","ncg_PR"));
    provider.setSubroutines(subs);
    stepdir_contr.setData_provider_optim(provider);

    unsigned int r = 1;

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);

    arma::mat gradient(dimensionOfControl_gp,3,arma::fill::randn);
    arma::mat gradient_old(dimensionOfControl_gp,3,arma::fill::randu);
    arma::mat stepdirectionOld(dimensionOfControl_gp,3,arma::fill::randu);

   ASSERT_NO_THROW(stepdir_contr.get_stepdirection(gradient, gradient_old, stepdirectionOld, r));
}

TEST(ncgHZ,noException) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepdirection_controller stepdir_contr = stepdirection_controller(filename);

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("direction_update");
    subs.insert(std::pair<std::string,std::string>("direction_update","ncg_HZ"));
    provider.setSubroutines(subs);
    stepdir_contr.setData_provider_optim(provider);

    unsigned int r = 1;

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);

    arma::mat gradient(dimensionOfControl_gp,3,arma::fill::randn);
    arma::mat gradient_old(dimensionOfControl_gp,3,arma::fill::randu);
    arma::mat stepdirectionOld(dimensionOfControl_gp,3,arma::fill::randu);

    ASSERT_NO_THROW(stepdir_contr.get_stepdirection(gradient, gradient_old, stepdirectionOld, r));
}
