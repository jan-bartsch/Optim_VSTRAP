#include <gtest/gtest.h>

#include "../../src/objects/data_provider.h"
#include "../../src/tools/inner_products.h"

TEST(innerProducts,L2Norm) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);

    inner_products pro = inner_products();
    pro.setData_provider_optim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    double small_discr_volume = static_cast<double>(optimizationParameters.find("small_discr_volume")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int number_cells_position = static_cast<unsigned int>(optimizationParameters.find("number_cells_position")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    arma::mat control(number_cells_position,3,arma::fill::randu);

    double inner_product_norm = std::sqrt(pro.L2_inner_product(control,control));
    double arma_norm = arma::norm(control,"fro")*std::sqrt(small_discr_volume);

    ASSERT_DOUBLE_EQ(inner_product_norm,arma_norm);
}
