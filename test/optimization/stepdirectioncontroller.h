#include <gtest/gtest.h>

#include "../../src/optimization/stepdirectioncontroller.h"
#include "../../src/objects/MOTIONS.h"

TEST(stepdirection, negativeGradient) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
   auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));
  StepdirectionController stepdir_contr = StepdirectionController(shared_input_data);

 shared_input_data->direction_update = "negative_gradient";

  unsigned int dimensionOfControl_gp = shared_input_data->dimension_control;

  arma::mat stepDirection;
  unsigned int r;

  arma::mat gradient(dimensionOfControl_gp, dimensionOfControl_gp,
                     arma::fill::randu);
  stepDirection =
      stepdir_contr.GetStepdirection(gradient, gradient, stepDirection, r);

  double norm_difference = arma::norm(stepDirection - (-gradient), "fro");
  ASSERT_EQ(norm_difference, 0.0);
}

TEST(ncgFR, noException) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
  auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));
  StepdirectionController stepdir_contr = StepdirectionController(shared_input_data);
  shared_input_data->direction_update = "ncg_FR";


  unsigned int r = 2;

  unsigned int dimensionOfControl_gp = shared_input_data->dimension_control;

  arma::mat gradient(dimensionOfControl_gp, 3, arma::fill::randn);
  arma::mat gradient_old(dimensionOfControl_gp, 3, arma::fill::randu);
  arma::mat stepdirectionOld(dimensionOfControl_gp, 3, arma::fill::randu);

  ASSERT_NO_THROW(stepdir_contr.GetStepdirection(gradient, gradient_old,
                                                 stepdirectionOld, r));
}

TEST(ncgPR, noException) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
  auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));
  StepdirectionController stepdir_contr = StepdirectionController(shared_input_data);
  shared_input_data->direction_update = "ncg_PR";

  unsigned int r = 2;


  unsigned int dimensionOfControl_gp = shared_input_data->dimension_control;

  arma::mat gradient(dimensionOfControl_gp, 3, arma::fill::randn);
  arma::mat gradient_old(dimensionOfControl_gp, 3, arma::fill::randu);
  arma::mat stepdirectionOld(dimensionOfControl_gp, 3, arma::fill::randu);

  ASSERT_NO_THROW(stepdir_contr.GetStepdirection(gradient, gradient_old,
                                                 stepdirectionOld, r));
}

TEST(ncgHZ, noException) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
  auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));
  StepdirectionController stepdir_contr = StepdirectionController(shared_input_data);
  shared_input_data->direction_update = "ncg_HZ";

  unsigned int r = 2;

  unsigned int dimensionOfControl_gp = shared_input_data->dimension_control;

  arma::mat gradient(dimensionOfControl_gp, 3, arma::fill::randn);
  arma::mat gradient_old(dimensionOfControl_gp, 3, arma::fill::randu);
  arma::mat stepdirectionOld(dimensionOfControl_gp, 3, arma::fill::randu);

  ASSERT_NO_THROW(stepdir_contr.GetStepdirection(gradient, gradient_old,
                                                 stepdirectionOld, r));
}
