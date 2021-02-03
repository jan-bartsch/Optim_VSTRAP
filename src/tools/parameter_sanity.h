#ifndef PARAMETER_SANITY_H
#define PARAMETER_SANITY_H

#include <map>
#include <vector>
#include <cmath>
#include <sstream>

#include "../controller/abstract_controller.h"
#include "../objects/data_provider.h"


/**
 * @brief The parameter_sanity class provides sanity checks for parameters
 * definied in the input file of the optimizer
 */
class parameter_sanity
{
public:
    parameter_sanity();

    int check_adjoint_velocity(data_provider provider);

    int check_velocity_discretization(data_provider provider);
};

#endif // PARAMETER_SANITY_H
