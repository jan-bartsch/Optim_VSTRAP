#include "stepsize_controller.h"

stepsize_controller::stepsize_controller(const char *filename)
{
    this->setData_provider_optim(data_provider(filename));

}

double stepsize_controller::get_stepsize(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles, double stepsize0)
{
    return gradient_descent(control,stepdirection,inputParticles,stepsize0);
}

double stepsize_controller::armijo_linesearch(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles, double stepsize0)
{
    return 1;

}

double stepsize_controller::gradient_descent(arma::mat &control, arma::mat &stepdirection, std::vector<particle> &nputParticles, double stepsize)
{

    return static_cast<double>(this->getData_provider_optim().getOptimizationParameters().find("fixed_gradient_descent_stepsize")->second);

}
