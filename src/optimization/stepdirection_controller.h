#ifndef STEPDIRECTION_CONTROLLER_H
#define STEPDIRECTION_CONTROLLER_H

#include "../controller/abstract_controller.h"

class stepdirection_controller : public abstract_controller
{
public:
    stepdirection_controller(const char * filename);

    arma::mat get_stepdirection(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration);

private:
    arma::mat fixed_gradient_descent(arma::mat gradient, unsigned int optimization_iteration);
    arma::mat ncg_scheme(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration);


};

#endif // STEPDIRECTION_CONTROLLER_H
