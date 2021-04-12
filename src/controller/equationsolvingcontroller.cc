#include "equationsolvingcontroller.h"

EquationSolvingController::EquationSolvingController(std::shared_ptr<MOTIONS::InputData> &input_data) {
    this->setInput_data(input_data);
}

int EquationSolvingController::StartSolvingForward(std::string start_forward) {
  return system(&start_forward[0]);
}

int EquationSolvingController::StartSolvingBackward(
    std::string start_backward) {
  return system(&start_backward[0]);
}

arma::mat EquationSolvingController::D1SecondOrder() {
  Comparator comp = Comparator();

   std::map<int, std::vector<double>> barycenters = input_data_->barycenters_list;

  unsigned int dimension_control = input_data_->dimension_control; // input_data_->dimension_control;
  double small_discr_sidelength = input_data_->small_discr_sidelength;
  double position_max_gp = input_data_->position_max_gp;

  std::vector<double> current_barycenter;
  std::vector<double> next_cell_xm;
  std::vector<double> next_cell_xp;
  std::vector<double> next_cell_ym;
  std::vector<double> next_cell_yp;
  std::vector<double> next_cell_zm;
  std::vector<double> next_cell_zp;

  arma::mat gradient(dimension_control, 3, arma::fill::zeros);
  arma::mat D1(dimension_control, dimension_control, arma::fill::zeros);

  uint start_control = input_data_->start_control_gp;
  uint end_control = input_data_->end_control_gp;
  double fabs_tol_gp = input_data_->fabs_tol_gp;

  if (barycenters.size() < dimension_control) {
    throw std::invalid_argument(
        "Dimension of control bigger than number of barycenters");
  }

  for (uint i = start_control; i <= end_control; i++) {
    // for(int i = 1; i<=dimensionOfControl_gp; i++) {
    current_barycenter = barycenters.find(static_cast<int>(i))->second;
    D1(i - start_control, i - start_control) = 0;

    next_cell_xm = current_barycenter;
    next_cell_xp = current_barycenter;
    next_cell_ym = current_barycenter;
    next_cell_yp = current_barycenter;
    next_cell_zm = current_barycenter;
    next_cell_zp = current_barycenter;

    if (std::abs(current_barycenter[0] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_xm[0] = current_barycenter[0] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[0] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_xp[0] = current_barycenter[0] + small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_ym[1] = current_barycenter[1] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_yp[1] = current_barycenter[1] + small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_zm[2] = current_barycenter[2] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_zp[2] = current_barycenter[2] + small_discr_sidelength;
    }

    double xm = -1.0;
    double xp = 1.0;
    double ym = -1.0;
    double yp = 1.0;
    double zm = -1.0;
    double zp = 1.0;

    for (uint l = start_control; l <= end_control; l++) {
      std::vector<double> temp = barycenters.at(static_cast<int>(l));
      if (comp.NormDifferenceDoublevector(temp, next_cell_xp) < fabs_tol_gp) {
        if (i != l) {
          xp = 0.0;
          D1(i - start_control, l - start_control) = 1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_yp) < fabs_tol_gp) {
        if (i != l) {
          yp = 0.0;
          D1(i - start_control, l - start_control) = 1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_zp) < fabs_tol_gp) {
        if (i != l) {
          zp = 0.0;
          D1(i - start_control, l - start_control) = 1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_xm) < fabs_tol_gp) {
        if (i != l) {
          xm = 0.0;
          D1(i - start_control, l - start_control) = -1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_ym) < fabs_tol_gp) {
        if (i != l) {
          ym = 0.0;
          D1(i - start_control, l - start_control) = -1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_zm) < fabs_tol_gp) {
        if (i != l) {
          zm = 0.0;
          D1(i - start_control, l - start_control) = -1.0;
        }
      }
      D1(i - start_control, i - start_control) = xm + xp + ym + yp + zm + zp;
    }
  }
  return D1;
}

arma::mat EquationSolvingController::D1Forward() {
  Comparator comp = Comparator();

   std::map<int, std::vector<double>> barycenters = input_data_->barycenters_list;

  uint dimension_control = input_data_->dimension_control;
  double small_discr_sidelength = input_data_->small_discr_sidelength;
  double position_max_gp = input_data_->position_max_gp;

  std::vector<double> current_barycenter;
  std::vector<double> next_cell_xm;
  std::vector<double> next_cell_xp;
  std::vector<double> next_cell_ym;
  std::vector<double> next_cell_yp;
  std::vector<double> next_cell_zm;
  std::vector<double> next_cell_zp;

  arma::mat gradient(dimension_control, 3, arma::fill::zeros);
  arma::mat D1(dimension_control, dimension_control, arma::fill::zeros);

  uint start_control = input_data_->start_control_gp;
  uint end_control = input_data_->end_control_gp;
  double fabs_tol_gp = input_data_->fabs_tol_gp;

  if (barycenters.size() < dimension_control) {
    throw std::invalid_argument("Dimension of control <" +
                                std::to_string(dimension_control) +
                                "> bigger than number of barycenters <" +
                                std::to_string(barycenters.size()) + ">");
  }

  for (uint i = start_control; i <= end_control; i++) {
    // for(int i = 1; i<=dimensionOfControl_gp; i++) {
    current_barycenter = barycenters.find(static_cast<int>(i))->second;
    D1(i - start_control, i - start_control) += 0.0;

    next_cell_xm = current_barycenter;
    next_cell_xp = current_barycenter;
    next_cell_ym = current_barycenter;
    next_cell_yp = current_barycenter;
    next_cell_zm = current_barycenter;
    next_cell_zp = current_barycenter;

    if (std::abs(current_barycenter[0] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_xm[0] = current_barycenter[0] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[0] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_xp[0] = current_barycenter[0] + small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_ym[1] = current_barycenter[1] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_yp[1] = current_barycenter[1] + small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_zm[2] = current_barycenter[2] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_zp[2] = current_barycenter[2] + small_discr_sidelength;
    }

    double xm = 0.0;
    double xp = 0.0;
    double ym = 0.0;
    double yp = 0.0;
    double zm = 0.0;
    double zp = 0.0;

    for (uint l = start_control; l <= end_control; l++) {
      std::vector<double> temp = barycenters.at(static_cast<int>(l));
      if (comp.NormDifferenceDoublevector(temp, next_cell_xp) < fabs_tol_gp) {
        if (i != l) {
          xp = -1.0;
          D1(i - start_control, l - start_control) += 1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_yp) < fabs_tol_gp) {
        if (i != l) {
          yp = -1.0;
          D1(i - start_control, l - start_control) += 1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_zp) < fabs_tol_gp) {
        if (i != l) {
          zp = -1.0;
          D1(i - start_control, l - start_control) += 1.0;
        }
      }
      D1(i - start_control, i - start_control) = xm + xp + ym + yp + zm + zp;
    }
  }
  return D1;
}

arma::mat EquationSolvingController::D1Backward() {
  Comparator comp = Comparator();

   std::map<int, std::vector<double>> barycenters = input_data_->barycenters_list;

  uint dimension_control = input_data_->dimension_control;
  double small_discr_sidelength = input_data_->small_discr_sidelength;
  double position_max_gp = input_data_->position_max_gp;

  std::vector<double> current_barycenter;
  std::vector<double> next_cell_xm;
  std::vector<double> next_cell_xp;
  std::vector<double> next_cell_ym;
  std::vector<double> next_cell_yp;
  std::vector<double> next_cell_zm;
  std::vector<double> next_cell_zp;

  arma::mat gradient(dimension_control, 3, arma::fill::zeros);
  arma::mat D1(dimension_control, dimension_control, arma::fill::zeros);

  uint start_control = input_data_->start_control_gp;
  uint end_control = input_data_->end_control_gp;
  double fabs_tol_gp = input_data_->fabs_tol_gp;

  if (barycenters.size() < dimension_control) {
    throw std::invalid_argument("Dimension of control <" +
                                std::to_string(dimension_control) +
                                "> bigger than number of barycenters <" +
                                std::to_string(barycenters.size()) + ">");
  }

  for (uint i = start_control; i <= end_control; i++) {
    // for(int i = 1; i<=dimensionOfControl_gp; i++) {
    current_barycenter = barycenters.find(static_cast<int>(i))->second;
    D1(i - start_control, i - start_control) = 3;

    next_cell_xm = current_barycenter;
    next_cell_xp = current_barycenter;
    next_cell_ym = current_barycenter;
    next_cell_yp = current_barycenter;
    next_cell_zm = current_barycenter;
    next_cell_zp = current_barycenter;

    if (std::abs(current_barycenter[0] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_xm[0] = current_barycenter[0] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[0] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_xp[0] = current_barycenter[0] + small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_ym[1] = current_barycenter[1] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_yp[1] = current_barycenter[1] + small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_zm[2] = current_barycenter[2] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_zp[2] = current_barycenter[2] + small_discr_sidelength;
    }

    double xm = 0.0;
    double xp = 0.0;
    double ym = 0.0;
    double yp = 0.0;
    double zm = 0.0;
    double zp = 0.0;

    for (uint l = start_control; l <= end_control; l++) {
      std::vector<double> temp = barycenters.at(static_cast<int>(l));
      if (comp.NormDifferenceDoublevector(temp, next_cell_xm) < fabs_tol_gp) {
        if (i != l) {
          xm = 1.0;
          D1(i - start_control, l - start_control) = -1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_ym) < fabs_tol_gp) {
        if (i != l) {
          ym = 1.0;
          D1(i - start_control, l - start_control) = -1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_zm) < fabs_tol_gp) {
        if (i != l) {
          zm = 1.0;
          D1(i - start_control, l - start_control) = -1.0;
        }
      }
      D1(i - start_control, i - start_control) = xm + xp + ym + yp + zm + zp;
    }
  }
  return D1;
}

arma::mat EquationSolvingController::Laplacian3D() {
  Comparator comp = Comparator();
  std::map<int, std::vector<double>> barycenters = input_data_->barycenters_list;

  uint dimension_control = input_data_->dimension_control;
  double small_discr_sidelength = input_data_->small_discr_sidelength;
  double position_max_gp = input_data_->position_max_gp;
  uint start_control = input_data_->start_control_gp;
  uint end_control = input_data_->end_control_gp;
  double fabs_tol_gp = input_data_->fabs_tol_gp;

  std::vector<double> current_barycenter;
  std::vector<double> next_cell_xm;
  std::vector<double> next_cell_xp;
  std::vector<double> next_cell_ym;
  std::vector<double> next_cell_yp;
  std::vector<double> next_cell_zm;
  std::vector<double> next_cell_zp;

  arma::mat gradient(dimension_control, 3, arma::fill::zeros);
  arma::mat Laplace(dimension_control, dimension_control, arma::fill::zeros);

  if (barycenters.size() < dimension_control) {
    throw std::invalid_argument("Dimension of control <" +
                                std::to_string(dimension_control) +
                                "> bigger than number of barycenters <" +
                                std::to_string(barycenters.size()) + ">");
  }

  for (uint i = start_control; i <= end_control; i++) {
    // for(int i = 1; i<=dimensionOfControl_gp; i++) {
    current_barycenter = barycenters.at(static_cast<int>(i));
    Laplace(i - start_control, i - start_control) = -6.0;

    next_cell_xm = current_barycenter;
    next_cell_xp = current_barycenter;
    next_cell_ym = current_barycenter;
    next_cell_yp = current_barycenter;
    next_cell_zm = current_barycenter;
    next_cell_zp = current_barycenter;

    if (std::abs(current_barycenter[0] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_xm[0] = current_barycenter[0] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[0] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_xp[0] = current_barycenter[0] + small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_ym[1] = current_barycenter[1] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_yp[1] = current_barycenter[1] + small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_zm[2] = current_barycenter[2] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_zp[2] = current_barycenter[2] + small_discr_sidelength;
    }

    for (uint l = start_control; l <= end_control; l++) {
      std::vector<double> temp = barycenters.at(static_cast<int>(l));
      if (comp.NormDifferenceDoublevector(temp, next_cell_xm) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_xp) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_ym) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_yp) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_zm) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_zp) < fabs_tol_gp) {
        if (i != l) {
          Laplace(i - start_control, l - start_control) = 1.0;
        }
      }
    }
  }

  return Laplace;
  // return this->D1_forward()*this->D1_backward();
}

arma::mat EquationSolvingController::LaplacianSquared3D() {
  Comparator comp = Comparator();
  std::map<int, std::vector<double>> barycenters = input_data_->barycenters_list;

  uint dimension_control = input_data_->dimension_control;
  double small_discr_sidelength = input_data_->small_discr_sidelength;
  double position_max_gp = input_data_->position_max_gp;
  uint start_control = input_data_->start_control_gp;
  uint end_control = input_data_->end_control_gp;
  double fabs_tol_gp = input_data_->fabs_tol_gp;

  std::vector<double> current_barycenter;
  std::vector<double> next_cell_xm;
  std::vector<double> next_cell_xp;
  std::vector<double> next_cell_ym;
  std::vector<double> next_cell_yp;
  std::vector<double> next_cell_zm;
  std::vector<double> next_cell_zp;

  std::vector<double> next_cell_xmm;
  std::vector<double> next_cell_xpp;
  std::vector<double> next_cell_ymm;
  std::vector<double> next_cell_ypp;
  std::vector<double> next_cell_zmm;
  std::vector<double> next_cell_zpp;

  arma::mat Laplace(dimension_control, dimension_control, arma::fill::zeros);

  for (uint i = start_control; i <= end_control; i++) {
    // for(int i = 1; i<=dimensionOfControl_gp; i++) {
    current_barycenter = barycenters.find(static_cast<int>(i))->second;
    Laplace(i - start_control, i - start_control) = 18.0;

    next_cell_xm = current_barycenter;
    next_cell_xp = current_barycenter;
    next_cell_ym = current_barycenter;
    next_cell_yp = current_barycenter;
    next_cell_zm = current_barycenter;
    next_cell_zp = current_barycenter;

    next_cell_xmm = current_barycenter;
    next_cell_xpp = current_barycenter;
    next_cell_ymm = current_barycenter;
    next_cell_ypp = current_barycenter;
    next_cell_zmm = current_barycenter;
    next_cell_zpp = current_barycenter;

    if (std::abs(current_barycenter[0] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_xm[0] = current_barycenter[0] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[0] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_xp[0] = current_barycenter[0] + small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_ym[1] = current_barycenter[1] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_yp[1] = current_barycenter[1] + small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_zm[2] = current_barycenter[2] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_zp[2] = current_barycenter[2] + small_discr_sidelength;
    }

    if (std::abs(current_barycenter[0] - 2.0 * small_discr_sidelength) <
        position_max_gp) {
      next_cell_xmm[0] = current_barycenter[0] - 2.0 * small_discr_sidelength;
    }
    if (std::abs(current_barycenter[0] + 2.0 * small_discr_sidelength) <
        position_max_gp) {
      next_cell_xpp[0] = current_barycenter[0] + 2.0 * small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] - 2.0 * small_discr_sidelength) <
        position_max_gp) {
      next_cell_ymm[1] = current_barycenter[1] - 2.0 * small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] + 2.0 * small_discr_sidelength) <
        position_max_gp) {
      next_cell_ypp[1] = current_barycenter[1] + 2.0 * small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] - 2.0 * small_discr_sidelength) <
        position_max_gp) {
      next_cell_zmm[2] = current_barycenter[2] - 2.0 * small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] + 2.0 * small_discr_sidelength) <
        position_max_gp) {
      next_cell_zpp[2] = current_barycenter[2] + 2.0 * small_discr_sidelength;
    }

    for (uint l = start_control; l <= end_control; l++) {
      std::vector<double> temp = barycenters.at(static_cast<int>(l));
      if (comp.NormDifferenceDoublevector(temp, next_cell_xm) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_xp) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_ym) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_yp) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_zm) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_zp) < fabs_tol_gp) {
        if (i != l) {
          Laplace(i - start_control, l - start_control) = -4.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_xmm) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_xpp) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_ymm) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_ypp) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_zmm) < fabs_tol_gp ||
          comp.NormDifferenceDoublevector(temp, next_cell_zpp) < fabs_tol_gp) {
        if (i != l) {
          Laplace(i - start_control, l - start_control) = +1.0;
        }
      }
    }
  }
  return Laplace;
}

arma::mat EquationSolvingController::D1x1SecondOrder() {
  Comparator comp = Comparator();
  std::map<int, std::vector<double>> barycenters = input_data_->barycenters_list;

  uint dimension_control = input_data_->dimension_control;
  double small_discr_sidelength = input_data_->small_discr_sidelength;
  double position_max_gp = input_data_->position_max_gp;
  uint start_control = input_data_->start_control_gp;
  uint end_control = input_data_->end_control_gp;
  double fabs_tol_gp = input_data_->fabs_tol_gp;

  std::vector<double> current_barycenter;
  std::vector<double> next_cell_xm;
  std::vector<double> next_cell_xp;

  arma::mat gradient(dimension_control, 3, arma::fill::zeros);
  arma::mat D1(dimension_control, dimension_control, arma::fill::zeros);

  if (barycenters.size() < dimension_control) {
    throw std::invalid_argument(
        "Dimension of control bigger than number of barycenters");
  }

  for (uint i = start_control; i <= end_control; i++) {
    // for(int i = 1; i<=dimensionOfControl_gp; i++) {
    current_barycenter = barycenters.find(static_cast<int>(i))->second;
    D1(i - start_control, i - start_control) = 0;

    next_cell_xm = current_barycenter;
    next_cell_xp = current_barycenter;

    if (std::abs(current_barycenter[0] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_xm[0] = current_barycenter[0] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[0] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_xp[0] = current_barycenter[0] + small_discr_sidelength;
    }

    double xm = -1.0;
    double xp = 1.0;

    for (uint l = start_control; l <= end_control; l++) {
      std::vector<double> temp = barycenters.at(static_cast<int>(l));
      if (comp.NormDifferenceDoublevector(temp, next_cell_xp) < fabs_tol_gp) {
        if (i != l) {
          xp = 0.0;
          D1(i - start_control, l - start_control) = 1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_xm) < fabs_tol_gp) {
        if (i != l) {
          xm = 0.0;
          D1(i - start_control, l - start_control) = -1.0;
        }
      }
      D1(i - start_control, i - start_control) = xm + xp;
    }
  }
  return D1;
}

arma::mat EquationSolvingController::D1x2SecondOrder() {
  Comparator comp = Comparator();
   std::map<int, std::vector<double>> barycenters = input_data_->barycenters_list;
  uint dimension_control = input_data_->dimension_control;
  double small_discr_sidelength = input_data_->small_discr_sidelength;
  double position_max_gp = input_data_->position_max_gp;
  uint start_control = input_data_->start_control_gp;
  uint end_control = input_data_->end_control_gp;
  double fabs_tol_gp = input_data_->fabs_tol_gp;

  std::vector<double> current_barycenter;
  std::vector<double> next_cell_ym;
  std::vector<double> next_cell_yp;

  arma::mat gradient(dimension_control, 3, arma::fill::zeros);
  arma::mat D1(dimension_control, dimension_control, arma::fill::zeros);

  if (barycenters.size() < dimension_control) {
    throw std::invalid_argument(
        "Dimension of control bigger than number of barycenters");
  }

  for (uint i = start_control; i <= end_control; i++) {
    // for(int i = 1; i<=dimensionOfControl_gp; i++) {
    current_barycenter = barycenters.find(static_cast<int>(i))->second;
    D1(i - start_control, i - start_control) = 0;

    next_cell_ym = current_barycenter;
    next_cell_yp = current_barycenter;

    if (std::abs(current_barycenter[1] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_ym[1] = current_barycenter[1] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[1] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_yp[1] = current_barycenter[1] + small_discr_sidelength;
    }

    double ym = -1.0;
    double yp = 1.0;

    for (uint l = start_control; l <= end_control; l++) {
      std::vector<double> temp = barycenters.at(static_cast<int>(l));
      if (comp.NormDifferenceDoublevector(temp, next_cell_yp) < fabs_tol_gp) {
        if (i != l) {
          yp = 0.0;
          D1(i - start_control, l - start_control) = 1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_ym) < fabs_tol_gp) {
        if (i != l) {
          ym = 0.0;
          D1(i - start_control, l - start_control) = -1.0;
        }
      }
      D1(i - start_control, i - start_control) = ym + yp;
    }
  }
  return D1;
}

arma::mat EquationSolvingController::D1x3SecondOrder() {
  Comparator comp = Comparator();

   std::map<int, std::vector<double>> barycenters = input_data_->barycenters_list;

  uint dimension_control = input_data_->dimension_control;
  double small_discr_sidelength = input_data_->small_discr_sidelength;
  double position_max_gp = input_data_->position_max_gp;
  uint start_control = input_data_->start_control_gp;
  uint end_control = input_data_->end_control_gp;
  double fabs_tol_gp = input_data_->fabs_tol_gp;

  std::vector<double> current_barycenter;
  std::vector<double> next_cell_zm;
  std::vector<double> next_cell_zp;

  arma::mat gradient(dimension_control, 3, arma::fill::zeros);
  arma::mat D1(dimension_control, dimension_control, arma::fill::zeros);

  if (barycenters.size() < dimension_control) {
    throw std::invalid_argument(
        "Dimension of control bigger than number of barycenters");
  }

  for (uint i = start_control; i <= end_control; i++) {
    // for(int i = 1; i<=dimensionOfControl_gp; i++) {
    current_barycenter = barycenters.find(static_cast<int>(i))->second;
    D1(i - start_control, i - start_control) = 0;

    next_cell_zm = current_barycenter;
    next_cell_zp = current_barycenter;

    if (std::abs(current_barycenter[2] - small_discr_sidelength) <
        position_max_gp) {
      next_cell_zm[2] = current_barycenter[2] - small_discr_sidelength;
    }
    if (std::abs(current_barycenter[2] + small_discr_sidelength) <
        position_max_gp) {
      next_cell_zp[2] = current_barycenter[2] + small_discr_sidelength;
    }

    double zm = -1.0;
    double zp = 1.0;

    for (uint l = start_control; l <= end_control; l++) {
      std::vector<double> temp = barycenters.at(static_cast<int>(l));
      if (comp.NormDifferenceDoublevector(temp, next_cell_zp) < fabs_tol_gp) {
        if (i != l) {
          zp = 0.0;
          D1(i - start_control, l - start_control) = 1.0;
        }
      }
      if (comp.NormDifferenceDoublevector(temp, next_cell_zm) < fabs_tol_gp) {
        if (i != l) {
          zm = 0.0;
          D1(i - start_control, l - start_control) = -1.0;
        }
      }
      D1(i - start_control, i - start_control) = zm + zp;
    }
  }
  return D1;
}
