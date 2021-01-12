#include "parameter_sanity.h"

parameter_sanity::parameter_sanity() { }

int parameter_sanity::check_adjoint_velocity(data_provider provider)
{
    std::ostringstream streamObj;

    std::map<std::string,double> parameters = provider.getOptimizationParameters();

    double kb = 1.38064852*std::pow(10,-23);

    double mass_gp = parameters.find("mass_forward")->second;
    double temperature_gp = parameters.find("temperature_x_val")->second;

    double expected_speed = parameters.find("expected_speed")->second;
    double most_probable_speed = parameters.find("most_probable_speed")->second;

    double vmax_gp = parameters.find("vmax_gp")->second;

    double sigma_v_forward = std::sqrt(2.0*kb*temperature_gp/mass_gp);
    double lowerBound_expectedSpeed = sigma_v_forward/std::sqrt(M_PI/8.0);
    double upperBound_expectedSpeed = vmax_gp-sigma_v_forward;

    std::cout << "For the expected adjoint speed it shoud hold: " << lowerBound_expectedSpeed << " <= " << expected_speed << " <= " << upperBound_expectedSpeed << std::endl;

    if (lowerBound_expectedSpeed >= expected_speed || upperBound_expectedSpeed <= expected_speed) {
        std::string error_string = "Warning! Expected speed does not obey its bounds!";
        std::cout << error_string << std::endl;
        return 1;
    }

    return 0;
}

int parameter_sanity::check_velocity_discretization(data_provider provider)
{
    std::ostringstream streamObj;

    std::map<std::string,double> parameters = provider.getOptimizationParameters();

    double dv_gp = parameters.find("dv_gp")->second;
    double vcell_gp = parameters.find("vcell_gp")->second;
    double vmax_gp = parameters.find("vmax_gp")->second;

    double max = dv_gp*vcell_gp;

    if (max != 2.0*vmax_gp) {
        std::cerr << "Discretization of velocity space is erroneous" << std::endl;
    }

}
