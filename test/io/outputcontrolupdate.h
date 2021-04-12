#include <gtest/gtest.h>

#include "../../src/io/input.h"
#include "../../src/io/outputcontrolupdate.h"

TEST(io, controlUpdate) {
  bool all_clear(false);
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
  OutputControlUpdate out = OutputControlUpdate(filename);
  Input in = Input();
  in.set_DataProviderOptim(provider);

  double norm_difference = 1.0;

  arma::mat control_out(64, 3, arma::fill::randu);

  out.WritecontrolXml(control_out);

  std::cout << control_out << std::endl;
  std::string PATH_TO_SHARED_FILES =
      provider.getPaths().find("PATH_TO_SHARED_FILES")->second;
  std::string CONTROL_FIELD_CELLS_NAME =
      provider.getPaths().find("CONTROL_FIELD_CELLS_NAME")->second;
  std::string Input_directory_control =
      PATH_TO_SHARED_FILES + CONTROL_FIELD_CELLS_NAME;

  int number_cells_position =
      static_cast<int>(provider.getOptimizationParameters()
                           .find("number_cells_position")
                           ->second);

  const char *filename_control_in = Input_directory_control.c_str();

  arma::mat control_in =
      in.ReadControl(filename_control_in, number_cells_position);
  std::cout << control_in << std::endl;
  std::cout << control_in - control_out << std::endl;

  norm_difference = arma::norm(control_in - control_out, "inf");

  if (norm_difference < pow(10, -5)) {
    all_clear = true;
  }

  ASSERT_TRUE(all_clear);
}
