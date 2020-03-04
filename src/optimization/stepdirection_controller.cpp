#include "stepdirection_controller.h"

stepdirection_controller::stepdirection_controller(const char *filename)
{
    this->setData_provider_optim(data_provider(filename));

}

arma::mat stepdirection_controller::get_stepdirection(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration)
{
    return fixed_gradient_descent(gradient,optimization_iteration);
}



arma::mat stepdirection_controller::fixed_gradient_descent(arma::mat gradient, unsigned int optimization_iteration)
{
    return -gradient;
}

arma::mat stepdirection_controller::ncg_scheme(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration)
{

    arma::mat stepdirection_new;

    return stepdirection_new;

}
