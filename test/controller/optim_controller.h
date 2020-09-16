#include <gtest/gtest.h>

#include "../../src/controller/optim_controller.h"
#include "../../src/objects/data_provider.h"
#include "../../src/io/output_control_update.h"

//TEST(optimContr,startup) {
//    optim_controller contr = optim_controller();

//    int argc = 3;
//    const char **argv;
//    argv[0] = "/home/jan/Promotion_linuxPC/build-Optim/src/Optim_VSTRAP_CMAKE";
//    argv[1] = "../../Optim_VSTRAP/data/box_shifting_tests/Optim_input.xml";
//    argv[2] = "too much";

//    int ret_flag = 0;
//    ret_flag = contr.start_optimizer(argc,argv);

//    int expected = 3;

//    std::cout << ret_flag-3.0 << std::endl;

//    EXPECT_DOUBLE_EQ(ret_flag-3,0.0);
//}

TEST(optimContr,interpolateControl) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    output_control_update outContr = output_control_update();
    outContr.setData_provider_optim(provider);

    ASSERT_NO_THROW(outContr.interpolate_control(provider));
}

TEST(optimContr,startZeroControl) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    optim_controller contr = optim_controller();

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    arma::mat zero(dimensionOfControl_gp,3,arma::fill::zeros);

    arma::mat control = contr.start_with_zero_control(filename);

    double norm_difference = arma::norm(zero-control);

    ASSERT_LE(norm_difference,pow(10,-10));
}

TEST(optContr,startReadInControl) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    optim_controller contr = optim_controller();

    output_control_update updater = output_control_update();
    updater.setData_provider_optim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    arma::mat control_rand(dimensionOfControl_gp,3,arma::fill::randu);

    updater.writeControl_XML(control_rand);

    std::cout << control_rand << std::endl;

    arma::mat control = contr.start_with_given_control(filename);

    std::cout << control << std::endl;

    double norm_difference = arma::norm(control_rand-control);

    ASSERT_LE(norm_difference,pow(10,-5));
}

TEST(optContr,runIterationMaxDepth) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    optim_controller contr = optim_controller();

    output_control_update updater = output_control_update();
    updater.setData_provider_optim(provider);

    std::map<std::string, double> param = provider.getOptimizationParameters();
    param.erase("optimizationIteration_max_gp");
    param.insert(std::pair<std::string,double>("optimizationIteration_max_gp",1.0));
    provider.setOptimizationParameters(param);
    contr.setData_provider_optim(provider);

    int flag = contr.start_optimization_iteration(filename);
    std::cout << flag << std::endl;

}
