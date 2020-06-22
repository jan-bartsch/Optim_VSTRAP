#include <gtest/gtest.h>

#include <cmath>
#include "../../src/objects/data_provider.h"

TEST(dataprovider,globalParameters) {
    bool all_clear(true);
    data_provider provider = data_provider();
    std::string input_directory = "../../Optim_VSTRAP/test/data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();
    std::map<std::string,double> parameters = provider.read_optimization_parameters(filename);

    std::map<std::string,double> parameters_test;
    parameters_test.insert(std::pair<std::string,double>("armijo_descent_fraction",pow(10,-30)));
    parameters_test.insert(std::pair<std::string,double>("calculation_functional",1.0));
    parameters_test.insert(std::pair<std::string,double>("calculation_wasserstein",1.0));
    parameters_test.insert(std::pair<std::string,double>("C_theta_gp",pow(10,8)));
    parameters_test.insert(std::pair<std::string,double>("C_phi_gp",pow(10,8)));
    parameters_test.insert(std::pair<std::string,double>("dimensionOfControl_gp",64.0));
    parameters_test.insert(std::pair<std::string,double>("end_control_gp",48.0));
    parameters_test.insert(std::pair<std::string,double>("fixed_gradient_descent_stepsize",0.1));
    parameters_test.insert(std::pair<std::string,double>("fraction_of_optimal_control",pow(10,-5)));
    parameters_test.insert(std::pair<std::string,double>("optimizationIteration_max_gp",100.0));
    parameters_test.insert(std::pair<std::string,double>("dp_gp",0.0156));
    parameters_test.insert(std::pair<std::string,double>("dt_gp",pow(10,-5)));
    parameters_test.insert(std::pair<std::string,double>("dv_gp",400.0));
    parameters_test.insert(std::pair<std::string,double>("numberParticles_gp",2000.0));
    parameters_test.insert(std::pair<std::string,double>("ntimesteps_gp",10.0));
    parameters_test.insert(std::pair<std::string,double>("pcell_gp",64.0));
    parameters_test.insert(std::pair<std::string,double>("pmax_gp",0.5));
    parameters_test.insert(std::pair<std::string,double>("sigma_x_gp",0.25));
    parameters_test.insert(std::pair<std::string,double>("sigma_v_gp",pow(10,2)));
    parameters_test.insert(std::pair<std::string,double>("start_control_gp",17.0));
    parameters_test.insert(std::pair<std::string,double>("start_zero_control",0.0));
    parameters_test.insert(std::pair<std::string,double>("tolerance_gp",pow(10,-13)));
    parameters_test.insert(std::pair<std::string,double>("vcell_gp",10.0));
    parameters_test.insert(std::pair<std::string,double>("vmax_gp" ,2000.0));
    parameters_test.insert(std::pair<std::string,double>("weight_control_gp",10.0));

    int size_before_merge = parameters_test.size();
    parameters_test.merge(parameters);
    if(parameters.size() != size_before_merge) {
        all_clear = false;
    }
    ASSERT_TRUE(all_clear);
}

TEST(dataprovider,currentDirectory) {
    bool all_clear(true);

    std::string input_directory = "/home/jan/Promotion_linuxPC/Optim_VSTRAP/test/data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    std::map<std::string, std::string> paths = provider.getPaths();
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM_TEST")->second;

    std::string current_directory(get_current_dir_name());

    EXPECT_EQ(current_directory,BUILD_DIRECTORY_OPTIM);
}
