#ifndef STEPDIRECTION_CONTROLLER_H
#define STEPDIRECTION_CONTROLLER_H

#include <map>
#include <string>

#include "../controller/abstractcontroller.h"
#include "../logger/logger.h"
#include "../tools/innerproducts.h"

/**
 * @brief The stepdirection_controller class provides different methods for finding the step-direction,
 * as gradient descent and non-linear conjugate gradient (NCG) schemes with different update rules;
 * see e.g. https://www.caam.rice.edu/~yzhang/caam554/pdf/cgsurvey.pdf
 */
class StepdirectionController : public AbstractController
{
public:
    StepdirectionController(const char * filename);

    /**
     * @brief get_stepdirection is a generic method called in the main optimizer algorithm
     * @param gradient
     * @param gradient_old
     * @param stepdirection_old
     * @param optimization_iteration
     * @return
     */
    arma::mat GetStepdirection(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirection_old, unsigned int optimization_iteration);

private:
    arma::mat FixedGradientDescent(arma::mat gradient, unsigned int optimization_iteration);
    arma::mat NcgSchemeFR(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirection_old, unsigned int optimization_iteration);
    arma::mat NcgSchemePR(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirection_old, unsigned int optimization_iteration);
    arma::mat NcgSchemeHZ(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirection_old, unsigned int optimization_iteration);
};

#endif // STEPDIRECTION_CONTROLLER_H
