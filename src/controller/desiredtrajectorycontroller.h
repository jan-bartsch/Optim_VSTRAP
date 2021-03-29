#ifndef DESIRED_TRAJECTORY_CONTROLLER_H
#define DESIRED_TRAJECTORY_CONTROLLER_H

#include <vector>

#include "../logger/logger.h"
#include "abstractcontroller.h"

#include "../io/input.h"


/**
 * @brief The desired_trajectory_controller class
 * provides the trajectory of the mean value in phase space
 */
class DesiredTrajectoryController : public AbstractController
{
public:
    DesiredTrajectoryController();

    /**
     * @brief trajectory_desired is the generic function for calling the desired trajectory
     *
     * @param barycenter
     * @param l
     * @param m
     * @param n
     * @param o
     * @param brockettVector
     * @param plasma_state_output_interval
     * @return
     */
    std::vector<double> TrajectoryDesired(std::vector<double> barycenter,
                                           unsigned int l, unsigned int m, unsigned int n,
                                           unsigned int o, std::vector<std::vector<double> > brockett_vector,
                                           unsigned int plasma_state_output_interval);

    /**
     * @brief trajectory_desired_brockett provides the desired trajectory using a time dependent vector as Input
     *      *
     * @param brockettVector
     * @param o
     * @param plasma_state_output_interval
     * @return
     */
    std::vector<double> TrajectoryDesiredBrockett(std::vector<std::vector<double> > brockett_vector, unsigned int o,
                                                    unsigned int plasma_state_output_interval);


    std::map<int,std::vector<double>> barycenters;

    /**
     * @brief trajectory_desired_parameters uses the parameters defined in the Input file of the optimizer
     *
     * @param barycenter
     * @param l
     * @param m
     * @param n
     * @param o
     * @param plasma_state_output_interval
     * @return
     */
    std::vector<double> TrajectoryDesiredParameters(std::vector<double> barycenter,
                                                      unsigned int l, unsigned int m, unsigned int n,
                                                      unsigned int o,
                                                      unsigned int plasma_state_output_interval);
};

#endif // DESIRED_TRAJECTORY_CONTROLLER_H
