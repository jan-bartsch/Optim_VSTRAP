#ifndef STEPSIZE_CONTROLLER_H
#define STEPSIZE_CONTROLLER_H

#include <vector>
#include <sstream>


#include "../io/input.h"
#include "../io/outputcontrolupdate.h"
#include "../io/outputdiagnostics.h"

#include "../controller/abstractcontroller.h"
#include "../objects/particle.h"

#include "../optimization/objectivecalculator.h"

#include "../tools/innerproducts.h"


/**
 * @brief The stepsize_controller class provides different methods for finding an accepted step-size
 * (resulting in a decreasing value of the functional)
 */
class StepsizeController : public AbstractController
{
public:
    StepsizeController(const char * filename);

    int CalculateStepsize(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<Particle> &Input_particles,  double &stepsize0);

private:
    int ArmijoLinesearch(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<Particle> &Input_particles,  double &stepsize0);
    int GradientDescent(arma::mat &control, arma::mat &stepdirection, std::vector<Particle> &Input_particles, double &stepsize);
    int SuccessiveApproximation(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<Particle> &Input_particles, double &stepsize0);

};

#endif // STEPSIZE_CONTROLLER_H
