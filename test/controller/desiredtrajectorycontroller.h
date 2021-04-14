#include <gtest/gtest.h>

#include "../../src/controller/desiredtrajectorycontroller.h"
#include "../../src/io/input.h"

#include "../../src/objects/MOTIONS.h"

TEST(traj, parameter) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);

  auto shared_input_data = std::make_shared<MOTIONS::InputData>(
      MOTIONS::InitializeMotions::Load_MOTIONS(provider));
  DesiredTrajectoryController trajContr =
      DesiredTrajectoryController(shared_input_data);

  shared_input_data->desired_trajectory = "parameters";

  std::vector<double> bc(6, 0.0);
  std::vector<double> p_d(6, 0.0);

  p_d[0] = shared_input_data->desired_position_x;
  p_d[1] = shared_input_data->desired_position_y;
  p_d[2] = shared_input_data->desired_position_z;

  p_d[3] = shared_input_data->adjoint_vx;
  p_d[4] = shared_input_data->adjoint_vy;
  p_d[5] = shared_input_data->adjoint_vz;

  std::vector<std::vector<double>> brockett_file_dummy;
  unsigned int plasma_state_output_interval_dummy = 1;

  std::vector<double> p_d_trajectory = trajContr.TrajectoryDesired(
      bc, 0, 0, 0, 0, brockett_file_dummy, plasma_state_output_interval_dummy);

  double diff = 0.0;

  if (p_d_trajectory.size() != p_d.size()) {
    throw std::runtime_error("Size of vectors do not match");
  }

  for (unsigned int i = 0; i < p_d.size(); i++) {
    diff += p_d[i] - p_d_trajectory[i];
  }

  ASSERT_DOUBLE_EQ(diff, 0.0);
}

TEST(traj, brockett) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);

  auto shared_input_data = std::make_shared<MOTIONS::InputData>(
      MOTIONS::InitializeMotions::Load_MOTIONS(provider));
  DesiredTrajectoryController trajContr =
      DesiredTrajectoryController(shared_input_data);
  Input in = Input(shared_input_data);

  shared_input_data->desired_trajectory = "brockett";

  unsigned int ntimesteps_gp = shared_input_data->ntimesteps_gp;
  unsigned int plasma_state_output_interval =
      shared_input_data->plasma_state_output_interval;

  std::vector<double> bc(6, 0.0);
  std::vector<double> p_d(6, 0.0);
  double diff = 0.0;

  std::vector<std::vector<double>> brockettVector =
      in.ReadBrockettFile("./data/brockett.csv", ",", ntimesteps_gp);

  for (unsigned int o = 0; o < ntimesteps_gp; o++) {

    std::vector<double> p_d_trajectory = trajContr.TrajectoryDesired(
        bc, 0, 0, 0, o, brockettVector, plasma_state_output_interval);

    // position
    p_d[0] = brockettVector[o * plasma_state_output_interval][0];
    p_d[1] = brockettVector[o * plasma_state_output_interval][1];
    p_d[2] = brockettVector[o * plasma_state_output_interval][2];

    // velocity
    p_d[3] = brockettVector[o * plasma_state_output_interval][6];
    p_d[4] = brockettVector[o * plasma_state_output_interval][7];
    p_d[5] = brockettVector[o * plasma_state_output_interval][8];

    if (p_d_trajectory.size() != p_d.size()) {
      throw std::runtime_error("Size of vectors do not match");
    }

    for (unsigned int i = 0; i < p_d.size(); i++) {
      diff += p_d[i] - p_d_trajectory[i];
    }
  }

  ASSERT_DOUBLE_EQ(diff, 0.0);
}
