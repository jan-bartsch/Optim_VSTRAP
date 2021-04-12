#include "calculus.h"

Calculus::Calculus(std::shared_ptr<MOTIONS::InputData> &input_data) {
    this->setInput_data(input_data);
}

double Calculus::DivergenceVector(arma::mat &Input) {

  double small_discr_sidelength = input_data_->small_discr_sidelength;
  double small_discr_volume = input_data_->small_discr_volume;

  EquationSolvingController solver = EquationSolvingController(input_data_);

  arma::mat D1X1 = solver.D1x1SecondOrder();
  arma::mat D1X2 = solver.D1x2SecondOrder();
  arma::mat D1X3 = solver.D1x3SecondOrder();

  arma::vec control_x1 = D1X1 * Input.col(0);
  arma::vec control_x2 = D1X2 * Input.col(1);
  arma::vec control_x3 = D1X3 * Input.col(2);

  double div = 0.0;

  div += arma::accu(control_x1) / small_discr_sidelength;
  div += arma::accu(control_x2) / small_discr_sidelength;
  div += arma::accu(control_x3) / small_discr_sidelength;

  div *= (small_discr_sidelength * small_discr_sidelength *
          small_discr_sidelength);

  return div;
}
