#include <gtest/gtest.h>

#include "../../src/tools/parameter_sanity.h"

TEST(sanity,velocity) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  input_xml_path = input_directory.c_str();

    data_provider provider = data_provider(input_xml_path);

    parameter_sanity sanity = parameter_sanity();

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();

    int returnValue = sanity.check_adjoint_velocity(provider);

    ASSERT_EQ(returnValue,0);
}

TEST(sanity,discr) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  input_xml_path = input_directory.c_str();

    data_provider provider = data_provider(input_xml_path);

    parameter_sanity sanity = parameter_sanity();

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();

    int returnValue = sanity.check_velocity_discretization(provider);

    ASSERT_EQ(returnValue,0);
}
