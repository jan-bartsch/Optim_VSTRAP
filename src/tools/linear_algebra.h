#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <armadillo>
#include <map>
#include <vector>

#include "../controller/abstract_controller.h"
#include "../controller/equation_solving_controller.h"


class linear_algebra : public abstract_controller
{
public:
    linear_algebra();

    static std::vector<double> cross_product(std::vector<double> v1, std::vector<double> v2);
};

#endif // LINEAR_ALGEBRA_H
