#ifndef STEPSIZE_CONTROLLER_H
#define STEPSIZE_CONTROLLER_H

#include "../controller/abstract_controller.h"
#include "../objects/particle.h"

#include <vector>

class stepsize_controller : public abstract_controller
{
public:
    stepsize_controller();

    double armijo_linesearch(arma::mat gradient, double J0, arma::vec control_F, arma::mat stepdirection, std::vector<particle> inputParticles,  double stepsize0);

    double gradient_descent(arma::vec control, arma::mat stepdirection, std::vector<particle> inputParticles, double stepsize);

};

#endif // STEPSIZE_CONTROLLER_H
