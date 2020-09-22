#ifndef INNER_PRODUCTS_H
#define INNER_PRODUCTS_H

#include <armadillo>
#include <map>
#include <vector>

#include "../controller/abstract_controller.h"
#include "../controller/equation_solving_controller.h"

class inner_products : public abstract_controller
{
public:
    inner_products();

    double L2_inner_product(arma::mat m1, arma::mat m2);
    double H1_inner_product(arma::mat m1, arma::mat m2);
    double H2_inner_product(arma::mat m1, arma::mat m2);
};

#endif // INNER_PRODUCTS_H
