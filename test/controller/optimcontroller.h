#include <gtest/gtest.h>

#include "../../src/controller/optimcontroller.h"
#include "../../src/io/outputcontrolupdate.h"
#include "../../src/objects/dataprovider.h"

#include "../../src/objects/MOTIONS.h"

// TEST(optContr,startup) {
//    optim_controller contr = optim_controller();

//    int argc = 3;
//    const char **argv;
//    argv[0] =
//    "/home/jan/Promotion_linuxPC/build-Optim/src/Optim_VSTRAP_CMAKE"; argv[1]
//    = "../../Optim_VSTRAP/data/box_shifting_tests/Optim_Input.xml"; argv[2] =
//    "too much";

//    int ret_flag = 0;
//    ret_flag = contr.start_optimizer(argc,argv);

//    int expected = 3;

//    std::cout << ret_flag-3.0 << std::endl;

//    EXPECT_DOUBLE_EQ(ret_flag-3,0.0);
//}

TEST(optContr, interpolateControl) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
   auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  OutputControlUpdate outContr = OutputControlUpdate(shared_input_data);

  ASSERT_NO_THROW(outContr.InterpolateControl(shared_input_data));
}

TEST(optContr, startZeroControl) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
   auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  OptimController contr = OptimController();

  arma::mat zero(shared_input_data->dimension_control, 3, arma::fill::zeros);

  arma::mat control = contr.StartWithZeroControl(shared_input_data);

  double norm_difference = arma::norm(zero - control);

  ASSERT_LE(norm_difference, pow(10, -10));
}

TEST(optContr, startReadInControl) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
   auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));
  OptimController contr = OptimController();

  OutputControlUpdate updater = OutputControlUpdate(shared_input_data);

  arma::mat control_rand(shared_input_data->dimension_control, 3, arma::fill::randu);

  updater.WritecontrolXml(control_rand);

  std::cout << control_rand << std::endl;

  arma::mat control = contr.StartWithGivenControl(shared_input_data);

  std::cout << control << std::endl;

  double norm_difference = arma::norm(control_rand - control);

  ASSERT_LE(norm_difference, pow(10, -5));
}

/*
TEST(optContr,runIterationMaxDepth) {
    std::string Input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = Input_directory.c_str();

    data_provider provider = data_provider(filename);
    optim_controller contr = optim_controller();

    output_control_update updater = output_control_update();
    updater.set_DataProviderOptim(provider);

    std::map<std::string, double> param = provider.getOptimizationParameters();
    param.erase("optimizationIteration_max_gp");
    param.insert(std::pair<std::string,double>("optimizationIteration_max_gp",1.0));
    provider.setOptimizationParameters(param);
    contr.set_DataProviderOptim(provider);

    int flag = contr.start_optimization_iteration(filename);
    std::cout << flag << std::endl;
}
*/
