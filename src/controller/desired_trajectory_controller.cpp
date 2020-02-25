#include "desired_trajectory_controller.h"

desired_trajectory_controller::desired_trajectory_controller()
{
}

std::vector<double> desired_trajectory_controller::trajectory_desired(unsigned int i, unsigned int l, unsigned int m, unsigned int n, unsigned int o)
{
    std::vector<double> p_d(6,0.0);

    p_d = this->trajectory_desired_concentrating_mean(i,l,m,n,o);

    return p_d;
}

std::vector<double> desired_trajectory_controller::trajectory_desired_harmonic_oscillation(unsigned int i, unsigned int l, unsigned int m, unsigned int n, unsigned int o)
{
    std::vector<double> p_d(6,0.0);

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    double omega_gp = static_cast<double>(optimizationParameters.find("omega_gp")->second);
    double pmax_gp = static_cast<double>(optimizationParameters.find("pmax_gp")->second);
    double finalTime_gp =  static_cast<double>(optimizationParameters.find("finalTime_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    static arma::vec timeDiscr_gp = arma::linspace<arma::vec>(0,finalTime_gp,ntimesteps_gp);

    p_d[0] = 1.5*std::cos(omega_gp*timeDiscr_gp(o))+pmax_gp/2.0;
    p_d[3] = -1.5*omega_gp*std::sin(omega_gp*timeDiscr_gp(o));

    return p_d;

}

std::vector<double> desired_trajectory_controller::trajectory_desired_concentrating_mean(unsigned int i, unsigned int l, unsigned int m, unsigned int n, unsigned int o)
{
    std::vector<double> p_d(6,0.0);

    double pmax_gp = static_cast<double>(this->getData_provider_optim().getOptimizationParameters().find("pmax_gp")->second);

    p_d[0] = pmax_gp/2.0;
    p_d[3] = 0.0;

    return p_d;

}
