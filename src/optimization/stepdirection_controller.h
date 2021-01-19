#ifndef STEPDIRECTION_CONTROLLER_H
#define STEPDIRECTION_CONTROLLER_H

#include <map>
#include <string>

#include "../controller/abstract_controller.h"
#include "../logger/logger.h"
#include "../tools/inner_products.h"

class stepdirection_controller : public abstract_controller
{
public:
    stepdirection_controller(const char * filename);

    arma::mat get_stepdirection(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration);

    arma::mat fixed_gradient_descent(arma::mat gradient, unsigned int optimization_iteration);
    arma::mat ncg_scheme_FR(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration);
    arma::mat ncg_scheme_PR(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration);
    arma::mat ncg_scheme_HZ(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration);
};

#endif // STEPDIRECTION_CONTROLLER_H
