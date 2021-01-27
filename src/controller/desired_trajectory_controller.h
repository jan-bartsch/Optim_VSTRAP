#ifndef DESIRED_TRAJECTORY_CONTROLLER_H
#define DESIRED_TRAJECTORY_CONTROLLER_H

#include <vector>

#include "../logger/logger.h"
#include "abstract_controller.h"

#include "../io/input.h"

class desired_trajectory_controller : public abstract_controller
{
public:
    desired_trajectory_controller();

    std::vector<double> trajectory_desired(std::vector<double> barycenter,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o, std::vector<std::vector<double> > brockettVector,
                                           unsigned int plasma_state_output_interval);

    std::vector<double> trajectory_desired_parameters(std::vector<double> barycenter,
                                                      unsigned int l, unsigned int m, unsigned int n,
                                                      unsigned int o,
                                                      unsigned int plasma_state_output_interval);

    std::vector<double> trajectory_desired_brockett(std::vector<std::vector<double> > brockettVector, unsigned int o,
                                                    unsigned int plasma_state_output_interval);

private:
    std::map<int,std::vector<double>> barycenters;

};

#endif // DESIRED_TRAJECTORY_CONTROLLER_H
