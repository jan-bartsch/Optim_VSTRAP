#ifndef OBJECTIVE_CALCULATOR_H
#define OBJECTIVE_CALCULATOR_H

#include <armadillo>
#include <vector>

#include "../controller/abstract_controller.h"
#include "../controller/pdf_controller.h"
#include "../controller/desired_trajectory_controller.h"
#include "../controller/equation_solving_controller.h"

#include "../objects/particle.h"
#include "../objects/coordinate_phase_space_time.h"

#include "../logger/logger.h"

#include "../tools/inner_products.h"

#include "../io/input.h"


/**
 * @brief The objective_calculator class calculates the objective/functional
 * according to Brockett's approach of ensemble optimal control problems;
 * see, e.g.,  Bartsch, J., Borzì, A., Fanelli, F. et al. A theoretical investigation of Brockett’s ensemble optimal control problems.
 * Calc. Var. 58, 162 (2019). https://doi.org/10.1007/s00526-019-1604-2
 */
class objective_calculator : public abstract_controller
{
public:
    objective_calculator();
    objective_calculator(const char * filename);

    double calculate_objective(std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF_time, arma::mat control);
};

#endif // OBJECTIVE_CALCULATOR_H
