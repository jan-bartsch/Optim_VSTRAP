#ifndef STEPSIZE_CONTROLLER_H
#define STEPSIZE_CONTROLLER_H

#include "../controller/abstract_controller.h"
#include "../objects/particle.h"

#include <vector>

class stepsize_controller : public abstract_controller
{
public:
    stepsize_controller(const char * filename);

    double get_stepsize(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles,  double stepsize0);


private:
    double armijo_linesearch(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles,  double stepsize0);
    double gradient_descent(arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles, double stepsize);

};

#endif // STEPSIZE_CONTROLLER_H
