#ifndef DESIRED_TRAJECTORY_CONTROLLER_H
#define DESIRED_TRAJECTORY_CONTROLLER_H

#include "abstract_controller.h"

class desired_trajectory_controller : public abstract_controller
{
public:
    desired_trajectory_controller();

    std::vector<double> trajectory_desired(unsigned int i,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o);

    std::vector<double> trajectory_desired_harmonic_oscillation(unsigned int i,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o);

    std::vector<double> trajectory_desired_concentrating_mean(unsigned int i,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o);
};

#endif // DESIRED_TRAJECTORY_CONTROLLER_H
