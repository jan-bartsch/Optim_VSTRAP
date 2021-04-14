#ifndef PARAMETER_SANITY_H
#define PARAMETER_SANITY_H

#include <cmath>
#include <map>
#include <sstream>
#include <vector>

#include "../controller/abstractcontroller.h"

#include "../objects/MOTIONS.h"
#include "../objects/dataprovider.h"

/**
 * @brief The parameter_sanity class provides sanity checks for parameters
 * definied in the Input file of the optimizer
 */
class ParameterSanity : AbstractController {
public:
  ParameterSanity(std::shared_ptr<MOTIONS::InputData> &input_data);

  int CheckAdjointVelocity();

  int CheckVelocityDiscretization();
};

#endif // PARAMETER_SANITY_H
