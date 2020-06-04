#ifndef DESIRED_TRAJECTORY_CONTROLLER_H
#define DESIRED_TRAJECTORY_CONTROLLER_H

#include <vector>

#include "abstract_controller.h"

class desired_trajectory_controller : public abstract_controller
{
public:
    desired_trajectory_controller();

    std::vector<double> trajectory_desired(std::vector<double> barycenter,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o);

    std::vector<double> trajectory_desired_harmonic_oscillation(std::vector<double> barycenter,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o);

    std::vector<double> trajectory_desired_concentrating_mean(std::vector<double> barycenter,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o);

    std::vector<double> trajectory_desired_shifting_halfbox(std::vector<double> barycenter,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o);

    std::vector<double> trajectory_desired_concentrating_center(std::vector<double> barycenter,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o);

private:
    std::map<int,std::vector<double>> barycenters;

};

#endif // DESIRED_TRAJECTORY_CONTROLLER_H
