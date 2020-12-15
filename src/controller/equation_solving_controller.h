#ifndef EQUATION_SOLVING_CONTROLLER_H
#define EQUATION_SOLVING_CONTROLLER_H

#include <map>

#include "abstract_controller.h"
#include "../objects/comparator.h"

#include "../controller/equation_solving_controller.h"

class equation_solving_controller : public abstract_controller
{
public:
    equation_solving_controller();

    int start_solving_forward(std::string start_forward);

    int start_solving_backward(std::string start_backward);

    arma::mat D1_second_order();

    arma::mat D1_forward();

    arma::mat D1_backward();

    arma::mat Laplacian_3D();

    arma::mat Laplacian_Squared_3D();

    arma::mat D1X1_second_order();
    arma::mat D1X2_second_order();
    arma::mat D1X3_second_order();
};

#endif // EQUATION_SOLVING_CONTROLLER_H
