#ifndef OBJECTIVE_CALCULATOR_H
#define OBJECTIVE_CALCULATOR_H

#include <armadillo>
#include <vector>

#include <tbb/tbb.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#include "../controller/abstract_controller.h"
#include "../controller/pdf_controller.h"
#include "../controller/desired_trajectory_controller.h"

#include "../objects/particle.h"
#include "../objects/coordinate_phase_space_time.h"

#include "../logger/logger.h"

class objective_calculator : public abstract_controller
{
public:
    objective_calculator(const char * filename);

    double calculate_objective_L2(std::vector<std::vector<particle>> forwardParticles, arma::mat control);
};

#endif // OBJECTIVE_CALCULATOR_H
