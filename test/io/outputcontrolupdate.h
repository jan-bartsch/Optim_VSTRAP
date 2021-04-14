#include <gtest/gtest.h>

#include "../../src/io/input.h"
#include "../../src/io/outputcontrolupdate.h"

#include "../../src/objects/MOTIONS.h"

TEST(io, controlUpdate) {
  bool all_clear(false);
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);
  auto shared_input_data = std::make_shared<MOTIONS::InputData>(
      MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  OutputControlUpdate out = OutputControlUpdate(shared_input_data);
  Input in = Input(shared_input_data);

  double norm_difference = 1.0;

  arma::mat control_out(64, 3, arma::fill::randu);

  out.WritecontrolXml(control_out);

  std::cout << control_out << std::endl;
  std::string Input_directory_control =
      shared_input_data->path_to_shared_files +
      shared_input_data->control_field_cells_name;

  const char *filename_control_in = Input_directory_control.c_str();

  arma::mat control_in = in.ReadControl(
      filename_control_in, shared_input_data->number_cells_position);
  std::cout << control_in << std::endl;
  std::cout << control_in - control_out << std::endl;

  norm_difference = arma::norm(control_in - control_out, "inf");

  if (norm_difference < pow(10, -5)) {
    all_clear = true;
  }

  ASSERT_TRUE(all_clear);
}
