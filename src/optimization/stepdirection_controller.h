#ifndef STEPDIRECTION_CONTROLLER_H
#define STEPDIRECTION_CONTROLLER_H

#include <map>
#include <string>

#include "../controller/abstract_controller.h"
#include "../logger/logger.h"
#include "../tools/inner_products.h"

/**
 * @brief The stepdirection_controller class provides different methods for finding the step-direction,
 * as gradient descent and non-linear conjugate gradient (NCG) schemes with different update rules;
 * see e.g. https://www.caam.rice.edu/~yzhang/caam554/pdf/cgsurvey.pdf
 */
class stepdirection_controller : public abstract_controller
{
public:
    stepdirection_controller(const char * filename);

    /**
     * @brief get_stepdirection is a generic method called in the main optimizer algorithm
     * @param gradient
     * @param gradient_old
     * @param stepdirectionOld
     * @param optimization_iteration
     * @return
     */
    arma::mat get_stepdirection(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration);

private:
    arma::mat fixed_gradient_descent(arma::mat gradient, unsigned int optimization_iteration);
    arma::mat ncg_scheme_FR(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration);
    arma::mat ncg_scheme_PR(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration);
    arma::mat ncg_scheme_HZ(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration);
};

#endif // STEPDIRECTION_CONTROLLER_H
