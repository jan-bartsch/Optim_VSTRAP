#include <gtest/gtest.h>

#include "../../src/objects/MOTIONS.h"
#include "../../src/objects/dataprovider.h"
#include "../../src/tools/innerproducts.h"

TEST(innerProducts, L2Norm) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
  auto shared_input_data = std::make_shared<MOTIONS::InputData>(
      MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  InnerProducts pro = InnerProducts(shared_input_data);

  arma::mat control(shared_input_data->number_cells_position, 3,
                    arma::fill::randu);

  double inner_product_norm = std::sqrt(pro.L2InnerProduct(control, control));
  double arma_norm = arma::norm(control, "fro") *
                     std::sqrt(shared_input_data->small_discr_volume);

  ASSERT_DOUBLE_EQ(inner_product_norm, arma_norm);
}
