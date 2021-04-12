#include <gtest/gtest.h>

#include "../../src/objects/dataprovider.h"
#include <cmath>

TEST(dataprovider, amount_InputData) {
  bool all_clear(true);

  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);

  std::map<std::string, double> parameters =
      provider.ReadOptimizationParameters(filename);
  std::map<std::string, std::string> paths = provider.ReadPaths(filename);
  std::map<std::string, std::string> routines =
      provider.ReadSubroutines(filename);

  if (parameters.size() != 82) {
    std::cout << "Parameters size wrong; Expected 82 but was " +
                     std::to_string(parameters.size())
              << std::endl;
    all_clear = false;
  }
  if (paths.size() != 20) {
    std::cout << "Paths size wrong; Expected 20 but was " +
                     std::to_string(paths.size())
              << std::endl;
    all_clear = false;
  }
  if (routines.size() != 4) {
    std::cout << "Parameters size wrong; Expected 4 but was " +
                     std::to_string(routines.size())
              << std::endl;
    all_clear = false;
  }
  ASSERT_TRUE(all_clear);
}
