#ifndef OBJECTIVE_CALCULATOR_H
#define OBJECTIVE_CALCULATOR_H

#include <armadillo>
#include <vector>

#include "../controller/abstractcontroller.h"
#include "../controller/pdfcontroller.h"
#include "../controller/desiredtrajectorycontroller.h"
#include "../controller/equationsolvingcontroller.h"

#include "../objects/particle.h"
#include "../objects/coordinatephasespacetime.h"

#include "../logger/logger.h"

#include "../tools/innerproducts.h"

#include "../io/input.h"


/**
 * @brief The objective_calculator class calculates the objective/functional
 * according to Brockett's approach of ensemble optimal control problems;
 * see, e.g.,  Bartsch, J., Borzì, A., Fanelli, F. et al. A theoretical investigation of Brockett’s ensemble optimal control problems.
 * Calc. Var. 58, 162 (2019). https://doi.org/10.1007/s00526-019-1604-2
 */
class ObjectiveCalculator : public AbstractController
{
public:
    ObjectiveCalculator();
    ObjectiveCalculator(const char * filename);

    double CalculateObjective(std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double> > forwardPDF_time, arma::mat control);
};

#endif // OBJECTIVE_CALCULATOR_H
