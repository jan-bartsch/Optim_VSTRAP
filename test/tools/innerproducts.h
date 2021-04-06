#include <gtest/gtest.h>

#include "../../src/objects/dataprovider.h"
#include "../../src/tools/innerproducts.h"

TEST(innerProducts,L2Norm) {
    std::string Input_directory = "./data/Optim_Input_gTest.xml";
    const char *  filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);

    InnerProducts pro = InnerProducts();
    pro.set_DataProviderOptim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    double small_discr_volume = static_cast<double>(optimizationParameters.find("small_discr_volume")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int number_cells_position = static_cast<unsigned int>(optimizationParameters.find("number_cells_position")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    arma::mat control(number_cells_position,3,arma::fill::randu);

    double inner_product_norm = std::sqrt(pro.L2InnerProduct(control,control));
    double arma_norm = arma::norm(control,"fro")*std::sqrt(small_discr_volume);

    ASSERT_DOUBLE_EQ(inner_product_norm,arma_norm);
}
