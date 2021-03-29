#ifndef PARAMETER_SANITY_H
#define PARAMETER_SANITY_H

#include <map>
#include <vector>
#include <cmath>
#include <sstream>

#include "../objects/dataprovider.h"


/**
 * @brief The parameter_sanity class provides sanity checks for parameters
 * definied in the Input file of the optimizer
 */
class ParameterSanity
{
public:
    ParameterSanity();

    int CheckAdjointVelocity(DataProvider provider);

    int CheckVelocityDiscretization(DataProvider provider);
};

#endif // PARAMETER_SANITY_H
