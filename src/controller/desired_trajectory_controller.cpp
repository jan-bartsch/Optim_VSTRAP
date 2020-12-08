#include "desired_trajectory_controller.h"

desired_trajectory_controller::desired_trajectory_controller() { }

std::vector<double> desired_trajectory_controller::trajectory_desired(std::vector<double> barycenter, unsigned int l, unsigned int m, unsigned int n, unsigned int o)
{
    std::vector<double> p_d(6,0.0);
    std::map<std::string,std::string> subroutines = this->getData_provider_optim().getSubroutines();
    std::string desired_traj = subroutines.find("desired_trajectory")->second;
    std::string objective_calculation = subroutines.find("objective_calculation")->second;


    if(desired_traj.compare("box_center")==0) {
        p_d = this->trajectory_desired_concentrating_center(barycenter,l,m,n,o);
    } else if (desired_traj.compare("box_shifting_negative_x")==0 && objective_calculation.compare("magnitude")==0) {
        p_d = this->trajectory_desired_shifting_halfbox(barycenter,l,m,n,o);
    } else if (desired_traj.compare("box_shifting_negative_x")==0 && objective_calculation.compare("components")==0) {
        p_d = this->trajectory_desired_shifting_halfbox_components(barycenter,l,m,n,o);
    } else if (desired_traj.compare("brockett")==0) {
        p_d = this->trajectory_desired_brockett(barycenter,l,m,n,o);
    } else {
        std::invalid_argument("No such desired_trajectory subroutine");
        throw std::runtime_error("No such desired_trajectory subroutine");
    }

    return p_d;
}

std::vector<double> desired_trajectory_controller::trajectory_desired_harmonic_oscillation(std::vector<double> barycenter, unsigned int l, unsigned int m, unsigned int n, unsigned int o)
{
    std::vector<double> p_d(6,0.0);

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    double omega_gp = static_cast<double>(optimizationParameters.find("omega_gp")->second);
    double pmax_gp = static_cast<double>(optimizationParameters.find("pmax_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    double finalTime_gp =  static_cast<double>(ntimesteps_gp)*dt_gp;

    static arma::vec timeDiscr_gp = arma::linspace<arma::vec>(0,finalTime_gp,ntimesteps_gp);

    //TODO p_d[0] = 1.5*std::cos(omega_gp*timeDiscr_gp(o))+pmax_gp/2.0;
    //p_d[3] = -1.5*omega_gp*std::sin(omega_gp*timeDiscr_gp(o));

    return p_d;

}

std::vector<double> desired_trajectory_controller::trajectory_desired_concentrating_mean(std::vector<double> barycenter, unsigned int l, unsigned int m, unsigned int n, unsigned int o)
{
    std::vector<double> p_d(6,0.0);

    double pmax_gp = static_cast<double>(this->getData_provider_optim().getOptimizationParameters().find("pmax_gp")->second);

    p_d[0] = 0.0;
    p_d[3] = 0.0;
    p_d[4] = 0.0;
    p_d[5] = 0.0;

    return p_d;

}

std::vector<double> desired_trajectory_controller::trajectory_desired_shifting_halfbox(std::vector<double> barycenter, unsigned int l, unsigned int m, unsigned int n, unsigned int o)
{
    std::vector<double> p_d(6,0.0);

    double expected_speed = static_cast<double>(this->getData_provider_optim().getOptimizationParameters().find("expected_speed")->second);
    double desired_position_x = static_cast<double>(this->getData_provider_optim().getOptimizationParameters().find("desired_position_x")->second);
    double desired_position_y = static_cast<double>(this->getData_provider_optim().getOptimizationParameters().find("desired_position_y")->second);
    double desired_position_z = static_cast<double>(this->getData_provider_optim().getOptimizationParameters().find("desired_position_z")->second);

    //    if (barycenter[0] < 0) {
    //        p_d[0] = 0;
    //    } else {
    //        p_d[0] = barycenter[0]*barycenter[0]+
    //                barycenter[1]*barycenter[1]+
    //                barycenter[2]*barycenter[2];
    //    }
    //    p_d[1] = -100.0;
    //    p_d[2] = 100.0;
    //    p_d[3] = 100.0;

    p_d[0] = desired_position_x;
    p_d[1] = desired_position_y;
    p_d[2] = desired_position_z;

    p_d[4] = expected_speed;
    return p_d;
}

std::vector<double> desired_trajectory_controller::trajectory_desired_shifting_halfbox_components(std::vector<double> barycenter, unsigned int l, unsigned int m, unsigned int n, unsigned int o)
{
    std::vector<double> p_d(6,0.0);

    data_provider optim_provider = this->getData_provider_optim();
    std::map<std::string,double> parameters = optim_provider.getOptimizationParameters();

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

    return p_d;
}

std::vector<double> desired_trajectory_controller::trajectory_desired_concentrating_center(std::vector<double> barycenter, unsigned int l, unsigned int m, unsigned int n, unsigned int o)
{
    std::vector<double> p_d(6,0.0);

    data_provider optim_provider = this->getData_provider_optim();
    std::map<std::string,double> parameters = optim_provider.getOptimizationParameters();

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

    return p_d;
}

std::vector<double> desired_trajectory_controller::trajectory_desired_brockett(std::vector<double> barycenter, unsigned int l, unsigned int m, unsigned int n, unsigned int o)
{
    input in = input();

    std::vector<double> p_d(6,0.0);

    data_provider optim_provider = this->getData_provider_optim();
    std::map<std::string,double> parameters = optim_provider.getOptimizationParameters();
    std::map<std::string,std::string> paths = optim_provider.getPaths();

    std::vector<std::vector<double>> brockettVector = in.readBrockettFile(paths.find("PATH_TO_SHARED_FILES")->second+"brockett.csv",",", static_cast<unsigned int>(parameters.find("ntimesteps_gp")->second));

    //position
    p_d[0] = brockettVector[o][0];
    p_d[1] = brockettVector[o][1];
    p_d[2] = brockettVector[o][2];

    //velocity
    p_d[3] = brockettVector[o][6];
    p_d[4] = brockettVector[o][7];
    p_d[5] = brockettVector[o][8];

    return p_d;
}
