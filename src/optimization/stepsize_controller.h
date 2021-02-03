#ifndef STEPSIZE_CONTROLLER_H
#define STEPSIZE_CONTROLLER_H

#include <vector>
#include <sstream>


#include "../io/input.h"
#include "../io/output_control_update.h"
#include "../io/output_diagnostics.h"

#include "../controller/abstract_controller.h"
#include "../objects/particle.h"

#include "../optimization/objective_calculator.h"

#include "../tools/inner_products.h"


/**
 * @brief The stepsize_controller class provides different methods for finding an accepted step-size
 * (resulting in a decreasing value of the functional)
 */
class stepsize_controller : public abstract_controller
{
public:
    stepsize_controller(const char * filename);

    int calculate_stepsize(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles,  double &stepsize0);


private:
    int armijo_linesearch(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles,  double &stepsize0);
    int gradient_descent(arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles, double &stepsize);
    int successive_approximation(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles, double &stepsize0);

};

#endif // STEPSIZE_CONTROLLER_H
