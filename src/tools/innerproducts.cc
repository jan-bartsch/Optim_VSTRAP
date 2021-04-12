#include "innerproducts.h"

InnerProducts::InnerProducts(std::shared_ptr<MOTIONS::InputData> &input_data) {
    this->setInput_data(input_data);
}

double InnerProducts::L2InnerProduct(arma::mat m1, arma::mat m2) {

  double small_discr_volume = input_data_->small_discr_volume;
  double product = 0.0;

  const arma::SizeMat size1 = arma::size(m1);
  const arma::SizeMat size2 = arma::size(m2);

  if (size1.n_cols != size2.n_cols || size1.n_rows != size2.n_rows) {
    std::cout << m1 << std::endl;
    std::cout << m2 << std::endl;
    throw std::invalid_argument(
        "Objects in inner product calculation do not have proper size");
  }

  int columns = static_cast<int>(size1.n_cols);

  for (int i = 0; i < columns; i++) {
    product += arma::dot(m1.col(i), m2.col(i)) * small_discr_volume;
  }

  return product;
}

double InnerProducts::H1InnerProduct(arma::mat m1, arma::mat m2) {
  EquationSolvingController solver = EquationSolvingController(input_data_);

  arma::mat D1 = solver.D1SecondOrder();

  double small_discr_volume = input_data_->small_discr_volume;
  double small_discr_sidelength = input_data_->small_discr_sidelength;

  double product = 0.0;

  const arma::SizeMat size1 = arma::size(m1);
  const arma::SizeMat size2 = arma::size(m2);

  if (size1.n_cols != size2.n_cols || size1.n_rows != size2.n_rows) {
    std::cout << m1 << std::endl;
    std::cout << m2 << std::endl;
    throw std::invalid_argument(
        "Objects in inner product calculation do not have proper size");
  }

  int columns = static_cast<int>(size1.n_cols);

  /**
   * L2 part
   */
  product += L2InnerProduct(m1, m2);

  /*
   * first derivative
   */
  arma::mat D1mat1 = D1 * m1.rows(input_data_->start_control_gp - 1, input_data_->end_control_gp - 1) /
                     (2.0 * small_discr_sidelength);
  arma::mat D1mat2 = D1 * m2.rows(input_data_->start_control_gp - 1, input_data_->end_control_gp - 1) /
                     (2.0 * small_discr_sidelength);

  for (unsigned int i = 0; i < D1mat1.n_cols; i++) {
    std::cout << arma::dot(D1mat1.col(i), D1mat2.col(i)) * small_discr_volume
              << std::endl;
    product += arma::dot(D1mat1.col(i), D1mat2.col(i)) * small_discr_volume;
  }

  return product;
}

double InnerProducts::H2InnerProduct(arma::mat m1, arma::mat m2) {
  EquationSolvingController solver = EquationSolvingController(input_data_);

  arma::mat D2 = solver.Laplacian3D();

  double small_discr_volume = input_data_->small_discr_volume;
  double small_discr_sidelength = input_data_->small_discr_sidelength;

  double product = 0.0;

  const arma::SizeMat size1 = arma::size(m1);
  const arma::SizeMat size2 = arma::size(m2);

  if (size1.n_cols != size2.n_cols || size1.n_rows != size2.n_rows) {
    std::cout << m1 << std::endl;
    std::cout << m2 << std::endl;
    throw std::invalid_argument(
        "Objects in inner product calculation do not have proper size");
  }

  /**
   * L2 and H1 part
   */
  product += H1InnerProduct(m1, m2);

  /*
   * second derivative
   */
  arma::mat D2mat1 = D2 * m1.rows(input_data_->start_control_gp - 1, input_data_->end_control_gp - 1) /
                     (small_discr_sidelength * small_discr_sidelength);
  arma::mat D2mat2 = D2 * m2.rows(input_data_->start_control_gp - 1, input_data_->end_control_gp - 1) /
                     (small_discr_sidelength * small_discr_sidelength);

  for (unsigned int i = 0; i < m1.n_cols; i++) {
    product += arma::dot(D2mat1.col(i), D2mat2.col(i)) * small_discr_sidelength;
  }

  return product;
}
