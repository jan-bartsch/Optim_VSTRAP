#include <gtest/gtest.h>

#include "../../src/tools/parametersanity.h"
#include "../../src/objects/MOTIONS.h"

TEST(sanity, velocity) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);
   auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  ParameterSanity sanity = ParameterSanity(shared_input_data);

  int returnValue = sanity.CheckAdjointVelocity();

  ASSERT_EQ(returnValue, 0);
}

TEST(sanity, discr) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);
   auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  ParameterSanity sanity = ParameterSanity(shared_input_data);

  int returnValue = sanity.CheckVelocityDiscretization();

  ASSERT_EQ(returnValue, 0);
}
