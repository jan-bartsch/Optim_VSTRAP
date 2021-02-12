#ifndef DESIRED_TRAJECTORY_CONTROLLER_H
#define DESIRED_TRAJECTORY_CONTROLLER_H

#include <vector>

#include "../logger/logger.h"
#include "abstract_controller.h"

#include "../io/input.h"


/**
 * @brief The desired_trajectory_controller class
 * provides the trajectory of the mean value in phase space
 */
class desired_trajectory_controller : public abstract_controller
{
public:
    desired_trajectory_controller();

    std::vector<double> trajectory_desired(std::vector<double> barycenter,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o, std::vector<std::vector<double> > brockettVector,
                                           unsigned int plasma_state_output_interval);

    /**
     * @brief trajectory_desired_brockett provides the desired trajectory using a time dependent vector as input
     *
     * @param brockettVector
     * @param o
     * @param plasma_state_output_interval
     * @return
     */
    std::vector<double> trajectory_desired_brockett(std::vector<std::vector<double> > brockettVector, unsigned int o,
                                                    unsigned int plasma_state_output_interval);


    std::map<int,std::vector<double>> barycenters;

    /**
     * @brief trajectory_desired_parameters uses the parameters defined in the input file of the optimizer
     *
     * @param barycenter
     * @param l
     * @param m
     * @param n
     * @param o
     * @param plasma_state_output_interval
     * @return
     */
    std::vector<double> trajectory_desired_parameters(std::vector<double> barycenter,
                                                      unsigned int l, unsigned int m, unsigned int n,
                                                      unsigned int o,
                                                      unsigned int plasma_state_output_interval);
};

#endif // DESIRED_TRAJECTORY_CONTROLLER_H
