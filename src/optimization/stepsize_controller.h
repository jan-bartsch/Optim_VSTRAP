#ifndef STEPSIZE_CONTROLLER_H
#define STEPSIZE_CONTROLLER_H

#include "../io/input.h"
#include "../io/output_control_update.h"
#include "../io/output_diagnostics.h"

#include "../controller/abstract_controller.h"
#include "../objects/particle.h"

#include "../optimization/objective_calculator.h"

#include <vector>

class stepsize_controller : public abstract_controller
{
public:
    stepsize_controller(const char * filename);

    int calculate_stepsize(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles,  double stepsize0);


private:
    int armijo_linesearch(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles,  double stepsize0);
    int gradient_descent(arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles, double stepsize);

};

#endif // STEPSIZE_CONTROLLER_H