#ifndef CALCULUS_H
#define CALCULUS_H

#include <armadillo>
#include <map>
#include <vector>

#include "../controller/abstractcontroller.h"
#include "../controller/equationsolvingcontroller.h"

/**
 * @brief The calculus class provides method from analysis
 */
class Calculus : public AbstractController
{
public:
    Calculus();

    static std::vector<double> cross_product(std::vector<double> v1, std::vector<double> v2);

    [[deprecated]]
    double DivergenceVector(arma::mat Input);
};

#endif // CALCULUS_H
