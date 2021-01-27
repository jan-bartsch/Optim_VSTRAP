#include <gtest/gtest.h>

#include "../../src/objects/data_provider.h"
#include "../../src/tools/calculus.h"

TEST(calc,divergence) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);

    calculus calc = calculus();
    calc.setData_provider_optim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    double dp_gp = static_cast<double>(optimizationParameters.find("dp_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    arma::mat control(pcell_gp,3,arma::fill::zeros);

    double divergence = calc.divergence_vector(control);

    ASSERT_DOUBLE_EQ(divergence,0.0);
}
