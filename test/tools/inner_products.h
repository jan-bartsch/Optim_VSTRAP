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
    double dp_gp = static_cast<double>(optimizationParameters.find("dp_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    arma::mat control(pcell_gp,3,arma::fill::randu);

    double inner_product_norm = std::sqrt(pro.L2_inner_product(control,control));
    double arma_norm = arma::norm(control,"fro")*std::sqrt(dp_gp);

    ASSERT_DOUBLE_EQ(inner_product_norm,arma_norm);
}