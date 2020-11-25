#ifndef GLOBALPARAMETERS_H
#define GLOBALPARAMETERS_H

#include <cmath>
#include <armadillo>

namespace globalParameters {

/** nature constants */
static double boltz_gp = 1.3806*pow(10.0,-23.0);    // Boltzmann's constant (J/K)

/*** mcc Parameters */
static double effective_number = 9.0*pow(10,8); //Weight of simulation particle (1 particle resembles effective_number amount of particles
static double mass_gp = 6.63*pow(10,-26)*effective_number;       // Mass of argon atom (kg) times effective number
static double T = pow(10,3);               // Temperature (K)


static double gamma_gp = 0.9999; //Parameter of Keilson-Storer kernel, should be close but less than 1
static double beta_gp = mass_gp/(2.0*boltz_gp*T*(1-gamma_gp*gamma_gp)); // Important constant for MCC, influence on variance of the scattering
static double sigma_gp = 1.0/sqrt(2.0*beta_gp);
static double a0_gp = std::sqrt(beta_gp/M_PI)/(6.0*pow(10,-4));

static double tau_gp = 1.0/(a0_gp*pow(M_PI/beta_gp,1.0/2.0));
static double C_0_star_gp = (1-gamma_gp)/(gamma_gp*tau_gp);

/** numerical Parameters */
const static unsigned int ntimesteps_gp = 50; //total number of timesteps
const static unsigned int numberParticles_gp = static_cast<unsigned int>(5*pow(10,3)); //total number of simulation particles
const static unsigned int generationAdjointParticles_gp = static_cast<unsigned int>(pow(10,2)); //number of particles added by the source term
static double timestep_gp = 0.0025; //time-disretization
const static double finalTime_gp = ntimesteps_gp*timestep_gp; //terminal time
static arma::vec timeDiscr_gp = arma::linspace<arma::vec>(0,finalTime_gp,ntimesteps_gp);
static double dt_gp = timeDiscr_gp(1)-timeDiscr_gp(0);

/** domain in phase space */
//velocity space
const static int vcell_gp = 50; //number of cells
static double vmax_gp = 10.0; // domain: [-vmax_gp,vmax_gp]
static arma::vec vbin_gp = arma::linspace<arma::vec>(-vmax_gp,vmax_gp,vcell_gp);
static double dv_gp = vbin_gp(1)-vbin_gp(0);

//position space
const static int pcell_gp = 10; //number of cells
const static double pmax_gp = 1.0; // domain: [0,pmax_gp]
static arma::vec positionDiscr_gp = arma::linspace<arma::vec>(0.0,pmax_gp,pcell_gp);
static double dp_gp = positionDiscr_gp(1) - positionDiscr_gp(0);

static unsigned int tooManyFastParticles_gp = static_cast<unsigned int>(0.1*numberParticles_gp);

static unsigned int dimensionOfControl_gp = pcell_gp; //size of control vector
//parameter for harmonic oscillator
static double omega_gp = (2.0*M_PI)/(ntimesteps_gp*timestep_gp);

/** optimization parameters */
const static double C_theta_gp = pow(10,15); //weight of tracking part in functional
const static double C_phi_gp = pow(10,1); //weight of terminal state

static double weight_control_gp = pow(10.0,-6.0); //weight of the control, in particular important for smoothness of control

/*
* Parameters for generating particles and calculating functional,
* must not be choosen too big or too small depending on the problem
*/
const static double sigma_x_gp = 0.10;
const static double sigma_v_gp = 2.0*1.0; //*3.0

const static unsigned int optimizationIteration_max_gp = 500; //maximal iteration depth for optimization
const static double alpha0_gp = pow(10,2); //initial stepsize for armijo-linesearch
const static double armijo_descent_fraction = 1.0/pow(10,5); //acceptance of new step-direction
const static double tol_linesearch_gp = pow(10,-20); //tolerance for Armijo-linesearch
const static double tol_optimization_gp = pow(10,-14); //tolerance for optimization
const static double optimizationStepsize_gp = pow(10,-7); //stepsize for gradient descent method
}
#endif // GLOBALPARAMETERS_H
