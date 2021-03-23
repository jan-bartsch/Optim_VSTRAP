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
    double small_discr_volume = static_cast<double>(optimizationParameters.find("small_discr_volume")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int number_cells_position = static_cast<unsigned int>(optimizationParameters.find("number_cells_position")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    arma::mat control(number_cells_position,3,arma::fill::zeros);

    double divergence = calc.divergence_vector(control);

    ASSERT_DOUBLE_EQ(divergence,0.0);
}
