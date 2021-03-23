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

    /**
     * @brief D1_second_order calculates the first derivative with respect to position
     * with a seconr oder accuracy (using the Midpoint scheme)
     *
     * @return
     */
    arma::mat D1_second_order();

    /**
     * @brief D1_forward calculates the first derivitive with respect to position
     * using the forward scheme (first order accuracy)
     *
     * @return
     */
    arma::mat D1_forward();

    /**
     * @brief D1_backward calculates the first derivitive with respect to position
     * using the backward scheme (first order accuracy)
     * @return
     */
    arma::mat D1_backward();

    /**
     * @brief Laplacian_3D calculates the Laplacion in position in three dimensions
     *
     * @return
     */
    arma::mat Laplacian_3D();

    /**
     * @brief Laplacian_Squared_3D
     * @return
     */
    arma::mat Laplacian_Squared_3D();

    /** for testing */
    arma::mat D1X1_second_order();
    arma::mat D1X2_second_order();
    arma::mat D1X3_second_order();
};

#endif // EQUATION_SOLVING_CONTROLLER_H
