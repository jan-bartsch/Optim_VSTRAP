#include <gtest/gtest.h>

#include "../../src/controller/optimcontroller.h"
#include "../../src/objects/dataprovider.h"
#include "../../src/io/outputcontrolupdate.h"

//TEST(optContr,startup) {
//    optim_controller contr = optim_controller();

//    int argc = 3;
//    const char **argv;
//    argv[0] = "/home/jan/Promotion_linuxPC/build-Optim/src/Optim_VSTRAP_CMAKE";
//    argv[1] = "../../Optim_VSTRAP/data/box_shifting_tests/Optim_Input.xml";
//    argv[2] = "too much";

//    int ret_flag = 0;
//    ret_flag = contr.start_optimizer(argc,argv);

//    int expected = 3;

//    std::cout << ret_flag-3.0 << std::endl;

//    EXPECT_DOUBLE_EQ(ret_flag-3,0.0);
//}

TEST(optContr,interpolateControl) {
    std::string Input_directory = "./data/Optim_Input_gTest.xml";
    const char *  filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);
    OutputControlUpdate outContr = OutputControlUpdate();
    outContr.set_DataProviderOptim(provider);

    ASSERT_NO_THROW(outContr.InterpolateControl(provider));
}

TEST(optContr,startZeroControl) {
    std::string Input_directory = "./data/Optim_Input_gTest.xml";
    const char *  filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);
    OptimController contr = OptimController();

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("number_cells_position")->second);
    arma::mat zero(dimensionOfControl_gp,3,arma::fill::zeros);

    arma::mat control = contr.StartWithZeroControl(filename);

    double norm_difference = arma::norm(zero-control);

    ASSERT_LE(norm_difference,pow(10,-10));
}

TEST(optContr,startReadInControl) {
    std::string Input_directory = "./data/Optim_Input_gTest.xml";
    const char *  filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);
    OptimController contr = OptimController();

    OutputControlUpdate updater = OutputControlUpdate();
    updater.set_DataProviderOptim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("number_cells_position")->second);
    arma::mat control_rand(dimensionOfControl_gp,3,arma::fill::randu);

    updater.WritecontrolXml(control_rand);

    std::cout << control_rand << std::endl;

    arma::mat control = contr.StartWithGivenControl(filename);

    std::cout << control << std::endl;

    double norm_difference = arma::norm(control_rand-control);

    ASSERT_LE(norm_difference,pow(10,-5));
}

/*
TEST(optContr,runIterationMaxDepth) {
    std::string Input_directory = "./data/Optim_Input_gTest.xml";
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
