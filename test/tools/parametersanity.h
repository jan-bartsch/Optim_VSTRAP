#include <gtest/gtest.h>

#include "../../src/tools/parametersanity.h"

TEST(sanity, velocity) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);

  ParameterSanity sanity = ParameterSanity();

  std::map<std::string, double> optimizationParameters =
      provider.getOptimizationParameters();

  int returnValue = sanity.CheckAdjointVelocity(provider);

  ASSERT_EQ(returnValue, 0);
}

TEST(sanity, discr) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);

  ParameterSanity sanity = ParameterSanity();

  std::map<std::string, double> optimizationParameters =
      provider.getOptimizationParameters();

  int returnValue = sanity.CheckVelocityDiscretization(provider);

  ASSERT_EQ(returnValue, 0);
}
