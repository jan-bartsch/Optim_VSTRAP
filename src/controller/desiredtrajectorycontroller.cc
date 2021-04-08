#include "desiredtrajectorycontroller.h"

DesiredTrajectoryController::DesiredTrajectoryController() { }

std::vector<double> DesiredTrajectoryController::TrajectoryDesired(std::vector<double> barycenter,
                                                                      unsigned int l, unsigned int m, unsigned int n,
                                                                      unsigned int o, std::vector<std::vector<double> > brockett_vector,
                                                                       unsigned int plasma_state_output_interval)
{
    std::vector<double> p_d(6,0.0);
    std::string desired_traj = MOTIONS::subroutines::desired_trajectory;

    if(desired_traj.compare("parameters")==0) {
        p_d = this->TrajectoryDesiredParameters(barycenter,l,m,n,o, plasma_state_output_interval);
    } else if (desired_traj.compare("brockett")==0) {
        p_d = this->TrajectoryDesiredBrockett(brockett_vector, o, plasma_state_output_interval);
    } else {
        std::invalid_argument("No such desired_trajectory subroutine");
        throw std::runtime_error("No such desired_trajectory subroutine");
    }

    return p_d;
}

std::vector<double> DesiredTrajectoryController::TrajectoryDesiredParameters(std::vector<double> barycenter, unsigned int l, unsigned int m, unsigned int n,
                                                                               unsigned int o, unsigned int plasma_state_output_interval)
{
    std::vector<double> p_d(6,0.0);

    p_d[0] = MOTIONS::params::desired_position_x;
    p_d[1] = MOTIONS::params::desired_position_y;
    p_d[2] = MOTIONS::params::desired_position_z;

    p_d[3] = MOTIONS::params::adjoint_vx;
    p_d[4] = MOTIONS::params::adjoint_vy;
    p_d[5] = MOTIONS::params::adjoint_vz;

    return p_d;
}

std::vector<double> DesiredTrajectoryController::TrajectoryDesiredBrockett(std::vector<std::vector<double> > brockett_vector,
                                                                           unsigned int o,
                                                                           unsigned int plasma_state_output_interval)
{
    Input in = Input();

    std::vector<double> p_d(6,0.0);

    //position
    p_d[0] = brockett_vector[o*plasma_state_output_interval][0];
    p_d[1] = brockett_vector[o*plasma_state_output_interval][1];
    p_d[2] = brockett_vector[o*plasma_state_output_interval][2];

    //velocity
    p_d[3] = brockett_vector[o*plasma_state_output_interval][6];
    p_d[4] = brockett_vector[o*plasma_state_output_interval][7];
    p_d[5] = brockett_vector[o*plasma_state_output_interval][8];

    return p_d;
}
