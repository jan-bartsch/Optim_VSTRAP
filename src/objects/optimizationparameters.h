#pragma once

namespace optimizationParameters{

struct globalparameters {
    bool simulating_plasma;
    bool fmm;
    bool magnetic_force;
    bool electric_force;

    double armijo_descent_fraction;
    bool calculation_functional;
    double C_theta;
    double C_phi;
    double sigma_x;
    double sigma_v;

    //create adjoint particles and trajectory


};

int LoadParameters();

}
