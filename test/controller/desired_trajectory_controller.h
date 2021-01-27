#include <gtest/gtest.h>

#include "../../src/io/input.h"
#include "../../src/controller/desired_trajectory_controller.h"

TEST(traj,parameter) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  input_xml_path = input_directory.c_str();

    data_provider provider = data_provider(input_xml_path);

    desired_trajectory_controller trajContr = desired_trajectory_controller();
    trajContr.setData_provider_optim(data_provider(input_xml_path));


    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("desired_trajectory");
    subs.insert(std::pair<std::string,std::string>("desired_trajectory","parameter"));
    provider.setSubroutines(subs);
    trajContr.setData_provider_optim(provider);


    std::map<std::string, double> parameters = provider.getOptimizationParameters();

    std::vector<double> bc(6,0.0);
    std::vector<double> p_d(6,0.0);

    double expected_speed = parameters.find("expected_speed")->second;
    double desired_position_x = parameters.find("desired_position_x")->second;
    double desired_position_y = parameters.find("desired_position_y")->second;
    double desired_position_z = parameters.find("desired_position_z")->second;

    double desired_velocity_x = parameters.find("adjoint_vx")->second;
    double desired_velocity_y = parameters.find("adjoint_vy")->second;
    double desired_velocity_z = parameters.find("adjoint_vz")->second;

    p_d[0] = desired_position_x;
    p_d[1] = desired_position_y;
    p_d[2] = desired_position_z;

    p_d[3] = desired_velocity_x;
    p_d[4] = desired_velocity_y;
    p_d[5] = desired_velocity_z;

    std::vector<std::vector<double> > brockett_file_dummy;
    unsigned int plasma_state_output_interval_dummy = 1;

    std::vector<double> p_d_trajectory = trajContr.trajectory_desired(bc,0,0,0,0,brockett_file_dummy,plasma_state_output_interval_dummy);

    double diff = 0.0;

    if(p_d_trajectory.size() != p_d.size()) {
        throw std::runtime_error("Size of vectors do not match");
    }

    for(unsigned int i = 0; i<p_d.size(); i++) {
        diff += p_d[i]-p_d_trajectory[i];
    }

    ASSERT_DOUBLE_EQ(diff,0.0);
}

TEST(traj,brockett) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  input_xml_path = input_directory.c_str();

    data_provider provider = data_provider(input_xml_path);

    desired_trajectory_controller trajContr = desired_trajectory_controller();
    trajContr.setData_provider_optim(data_provider(input_xml_path));

    input in = input();
    in.setData_provider_optim(provider);


    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    std::map<std::string, std::string> paths = provider.getPaths();

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("desired_trajectory");
    subs.insert(std::pair<std::string,std::string>("desired_trajectory","brockett"));
    provider.setSubroutines(subs);
    trajContr.setData_provider_optim(provider);


    std::map<std::string, double> parameters = provider.getOptimizationParameters();
    int ntimesteps_gp = static_cast<unsigned int>(parameters.find("ntimesteps_gp")->second);
    unsigned int plasma_state_output_interval = static_cast<unsigned int>(parameters.find("plasma_state_output_interval")->second);

    std::vector<double> bc(6,0.0);
    std::vector<double> p_d(6,0.0);
    double diff = 0.0;

    std::vector<std::vector<double>> brockettVector = in.readBrockettFile("./data/brockett.csv",",", ntimesteps_gp);

    for(unsigned int o= 0; o<ntimesteps_gp; o++) {

        std::vector<double> p_d_trajectory = trajContr.trajectory_desired(bc,0,0,0,o,brockettVector,plasma_state_output_interval);

        //position
        p_d[0] = brockettVector[o*plasma_state_output_interval][0];
        p_d[1] = brockettVector[o*plasma_state_output_interval][1];
        p_d[2] = brockettVector[o*plasma_state_output_interval][2];

        //velocity
        p_d[3] = brockettVector[o*plasma_state_output_interval][6];
        p_d[4] = brockettVector[o*plasma_state_output_interval][7];
        p_d[5] = brockettVector[o*plasma_state_output_interval][8];

        if(p_d_trajectory.size() != p_d.size()) {
            throw std::runtime_error("Size of vectors do not match");
        }

        for(unsigned int i = 0; i<p_d.size(); i++) {
            diff += p_d[i]-p_d_trajectory[i];
        }
    }

    ASSERT_DOUBLE_EQ(diff,0.0);
}
