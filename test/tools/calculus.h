#include <gtest/gtest.h>

#include "../../src/objects/dataprovider.h"
#include "../../src/tools/calculus.h"
#include "../../src/objects/MOTIONS.h"

TEST(calc, divergence) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
   auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  Calculus calc = Calculus(shared_input_data);

  unsigned int number_cells_position = shared_input_data->number_cells_position;

  arma::mat control(number_cells_position, 3, arma::fill::zeros);

  double divergence = calc.DivergenceVector(control);

  ASSERT_DOUBLE_EQ(divergence, 0.0);
}
