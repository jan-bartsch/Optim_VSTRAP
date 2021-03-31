#ifndef EQUATION_SOLVING_CONTROLLER_H
#define EQUATION_SOLVING_CONTROLLER_H

#include <map>

#include "abstractcontroller.h"
#include "../objects/comparator.h"
#include "../objects/MOTIONS.h"

using uint = unsigned int;

class EquationSolvingController : public AbstractController
{
public:
    EquationSolvingController();

    int StartSolvingForward(std::string start_forward);

    int StartSolvingBackward(std::string start_backward);

    /**
     * @brief D1_second_order calculates the first derivative with respect to position
     * with a seconr oder accuracy (using the Midpoint scheme)
     *
     * @return
     */
    arma::mat D1SecondOrder();

    /**
     * @brief D1_forward calculates the first derivitive with respect to position
     * using the forward scheme (first order accuracy)
     *
     * @return
     */
    arma::mat D1Forward();

    /**
     * @brief D1_backward calculates the first derivitive with respect to position
     * using the backward scheme (first order accuracy)
     * @return
     */
    arma::mat D1Backward();

    /**
     * @brief Laplacian_3D calculates the Laplacion in position in three dimensions
     *
     * @return
     */
    arma::mat Laplacian3D();

    /**
     * @brief Laplacian_Squared_3D
     * @return
     */
    arma::mat LaplacianSquared3D();

    /** for testing */
    arma::mat D1x1SecondOrder();
    arma::mat D1x2SecondOrder();
    arma::mat D1x3SecondOrder();
};

#endif // EQUATION_SOLVING_CONTROLLER_H
