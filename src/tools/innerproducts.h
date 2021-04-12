#ifndef INNER_PRODUCTS_H
#define INNER_PRODUCTS_H

#include <armadillo>
#include <map>
#include <vector>

#include "../controller/abstractcontroller.h"
#include "../controller/equationsolvingcontroller.h"

class InnerProducts : public AbstractController {
public:
  InnerProducts(std::shared_ptr<MOTIONS::InputData> &input_data);

  double L2InnerProduct(arma::mat m1, arma::mat m2);
  double H1InnerProduct(arma::mat m1, arma::mat m2);
  double H2InnerProduct(arma::mat m1, arma::mat m2);
};

#endif // INNER_PRODUCTS_H
