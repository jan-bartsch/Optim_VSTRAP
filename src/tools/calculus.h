#ifndef CALCULUS_H
#define CALCULUS_H

#include <armadillo>
#include <map>
#include <vector>

#include "../controller/abstract_controller.h"
#include "../controller/equation_solving_controller.h"

/**
 * @brief The calculus class provides method from analysis
 */
class calculus : public abstract_controller
{
public:
    calculus();

    static std::vector<double> cross_product(std::vector<double> v1, std::vector<double> v2);

    double divergence_vector(arma::mat input);
};

#endif // CALCULUS_H
