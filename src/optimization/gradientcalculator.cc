#include "gradientcalculator.h"

GradientCalculator::GradientCalculator(
    std::shared_ptr<MOTIONS::InputData> &input_data) {
  this->setInput_data(input_data);
}

arma::mat GradientCalculator::CalculategradientForcecontrolSpaceHmNotParallel(
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
        forwardPDF_time,
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
        backwardPDF_time,
    arma::mat control) {

  std::map<int, std::vector<double>> barycenters =
      input_data_->barycenters_list;

  PdfController pdf_control = PdfController(input_data_);
  EquationSolvingController model_solver =
      EquationSolvingController(input_data_);
  OutputDiagnostics outDiag = OutputDiagnostics(input_data_);

  if (input_data_->magnetic_force == 1 && input_data_->electric_force == 1) {
    std::cerr << "Magnetic and electric force combined is not implemented yet"
              << std::endl;
  }
  if (input_data_->magnetic_force == 0 && input_data_->electric_force == 0) {
    std::cerr << "Force/Control should be either magnetic or electric. Nothing "
                 "was specified"
              << std::endl;
  }

  arma::mat Laplace = model_solver.Laplacian3D();
  arma::mat Laplace_Squared = model_solver.LaplacianSquared3D();

  arma::mat gradient(input_data_->number_cells_position, 3, arma::fill::zeros);
  arma::mat gradient_Riesz(input_data_->dimension_control, 3,
                           arma::fill::zeros);
  arma::mat rhs_Riesz(input_data_->dimension_control, 3, arma::fill::zeros);

  double vmax_gp = input_data_->vmax_gp;
  arma::vec velocity_Discr =
      arma::linspace<arma::vec>(-vmax_gp, vmax_gp, input_data_->vcell_gp);

  double dv_gp = input_data_->dv_gp;
  double dt_gp = input_data_->dt_gp;

  // Caculate integral in gradient
  const unsigned int n = input_data_->number_cells_position;
  int start_control = static_cast<int>(input_data_->start_control_gp);

  //#pragma omp parallel for
  for (unsigned int i = 1; i < n + 1; i++) {

    std::vector<double> current_barycenter =
        barycenters.find(static_cast<int>(i))->second;

    if (current_barycenter[0] > input_data_->local_control_x_min_gp &&
        current_barycenter[0] < input_data_->local_control_x_max_gp) {
      DoublePDF forwardPDFdouble =
          DoublePDF(input_data_->ntimesteps_gp, input_data_->vcell_gp);
      DoublePDF backwardPDFdouble =
          DoublePDF(input_data_->ntimesteps_gp, input_data_->vcell_gp);

      double firstDerivativeForwardPDF_V1_current,
          firstDerivativeForwardPDF_V2_current,
          firstDerivativeForwardPDF_V3_current;

      for (unsigned int o = 0; o < input_data_->ntimesteps_gp; o++) {
        for (unsigned int l = 0; l < input_data_->vcell_gp; l++) {
          for (unsigned int m = 0; m < input_data_->vcell_gp; m++) {
            for (unsigned int n = 0; n < input_data_->vcell_gp; n++) {
              CoordinatePhaseSpaceTime coordinate = CoordinatePhaseSpaceTime(
                  static_cast<int>(i), static_cast<int>(l), static_cast<int>(m),
                  static_cast<int>(n), static_cast<int>(o));
              auto iteratorForward = forwardPDF_time[o].find(coordinate);
              auto iteratorBackward = backwardPDF_time[o].find(coordinate);
              if (iteratorForward != forwardPDF_time[o].end()) {
                forwardPDFdouble.at(o, l, m, n) = iteratorForward->second;
              } else {
                forwardPDFdouble.at(o, l, m, n) = 0.0;
              }
              if (iteratorBackward != backwardPDF_time[o].end()) {
                backwardPDFdouble.at(o, l, m, n) = iteratorBackward->second;
              } else {
                backwardPDFdouble.at(o, l, m, n) = 0.0;
              }
            }
          }
        }
      }

      forwardPDFdouble =
          pdf_control.RelaxatingGaussseidel4D(forwardPDFdouble, 1000);

      for (unsigned int o = 0; o < input_data_->ntimesteps_gp; o++) {
        for (unsigned int l = 0; l < input_data_->vcell_gp; l++) {
          for (unsigned int m = 0; m < input_data_->vcell_gp; m++) {
            for (unsigned int n = 0; n < input_data_->vcell_gp; n++) {
              // derivative v_x
              double forwardPDF_current = forwardPDFdouble.at(o, l, m, n);
              if (l == 0) {
                firstDerivativeForwardPDF_V1_current =
                    (forwardPDFdouble.at(o, l + 1, m, n) - forwardPDF_current) /
                    dv_gp;
              } else if (l == input_data_->vcell_gp - 1) {
                firstDerivativeForwardPDF_V1_current =
                    (forwardPDF_current - forwardPDFdouble.at(o, l - 1, m, n)) /
                    dv_gp;
              } else {
                firstDerivativeForwardPDF_V1_current =
                    (forwardPDFdouble.at(o, l + 1, m, n) -
                     forwardPDFdouble.at(o, l - 1, m, n)) /
                    (2.0 * dv_gp);
              }
              // derivative v_y
              if (m == 0) {
                firstDerivativeForwardPDF_V2_current =
                    (forwardPDFdouble.at(o, l, m + 1, n) - forwardPDF_current) /
                    dv_gp;
              } else if (m == input_data_->vcell_gp - 1) {
                firstDerivativeForwardPDF_V2_current =
                    (forwardPDF_current - forwardPDFdouble.at(o, l, m - 1, n)) /
                    dv_gp;
              } else {
                firstDerivativeForwardPDF_V2_current =
                    (forwardPDFdouble.at(o, l, m + 1, n) -
                     forwardPDFdouble.at(o, l, m - 1, n)) /
                    (2.0 * dv_gp);
              }
              // derivative v_z
              if (n == 0) {
                firstDerivativeForwardPDF_V3_current =
                    (forwardPDFdouble.at(o, l, m, n + 1) - forwardPDF_current) /
                    dv_gp;
              } else if (n == input_data_->vcell_gp - 1) {
                firstDerivativeForwardPDF_V3_current =
                    (forwardPDF_current - forwardPDFdouble.at(o, l, m, n - 1)) /
                    dv_gp;
              } else {
                firstDerivativeForwardPDF_V3_current =
                    (forwardPDFdouble.at(o, l, m, n + 1) -
                     forwardPDFdouble.at(o, l, m, n - 1)) /
                    (2.0 * dv_gp);
              }
              /*
               * electric field
               */
              double backwardPDF_current = backwardPDFdouble.at(o, l, m, n);
              if (input_data_->electric_force == 1) {
                gradient(i - 1, 0) += backwardPDF_current *
                                      firstDerivativeForwardPDF_V1_current *
                                      pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
                gradient(i - 1, 1) += backwardPDF_current *
                                      firstDerivativeForwardPDF_V2_current *
                                      pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
                gradient(i - 1, 2) += backwardPDF_current *
                                      firstDerivativeForwardPDF_V3_current *
                                      pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
              }
              /*
               * Magnetic field
               */
              if (input_data_->magnetic_force == 1) {
                gradient(i - 1, 0) +=
                    backwardPDF_current *
                    (-velocity_Discr[n] * firstDerivativeForwardPDF_V2_current +
                     velocity_Discr[m] * firstDerivativeForwardPDF_V3_current) *
                    pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
                gradient(i - 1, 1) +=
                    backwardPDF_current *
                    (velocity_Discr[n] * firstDerivativeForwardPDF_V1_current -
                     velocity_Discr[l] * firstDerivativeForwardPDF_V3_current) *
                    pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
                gradient(i - 1, 2) +=
                    backwardPDF_current *
                    (-velocity_Discr[m] * firstDerivativeForwardPDF_V1_current +
                     velocity_Discr[l] * firstDerivativeForwardPDF_V2_current) *
                    pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
              }
            }
          }
        }
      }
    } else {
      std::cout << "Cell_id " << i << ": Gradient stays zero here" << std::endl;
    }
    std::cout << gradient(i - 1, 0) << std::endl;
  }

  /*
   * Assemble gradient
   */

  std::cout << "Gradient:" << std::endl;
  std::cout << gradient << std::endl;

  for (int j = 0; j < input_data_->number_cells_position; j++) {
    if (j > start_control - 2 &&
        j < static_cast<int>(input_data_->end_control_gp)) {
      rhs_Riesz(j - start_control + 1, 0) = gradient(j, 0);
      rhs_Riesz(j - start_control + 1, 1) = gradient(j, 1);
      rhs_Riesz(j - start_control + 1, 2) = gradient(j, 2);
    }
  }

  // std::cout << "rhs_Riesz:" << std::endl;
  std::cout << rhs_Riesz << std::endl;

  // std::cout << "Riesz Matrix" << std::endl;
  arma::mat Riesz =
      input_data_->weight_control_gp *
      (arma::eye(input_data_->dimension_control,
                 input_data_->dimension_control) -
       1.0 / (pow(input_data_->small_discr_sidelength, 2)) * Laplace +
       1.0 / (pow(input_data_->small_discr_sidelength, 4)) * Laplace_Squared);
  // std::cout << Riesz << std::endl;
  outDiag.WriteArmaMatrixToFile(Riesz, "RiesMatrix");
  // std::cout << "Condition number Matrix Riesz: " << arma::cond(Riesz) <<
  // std::endl;

  arma::mat Riesz_control(input_data_->dimension_control, 3, arma::fill::zeros);
  for (int j = 0; j < input_data_->number_cells_position; j++) {
    if (j > start_control - 2 && j < input_data_->end_control_gp) {
      Riesz_control(j - start_control + 1, 0) = control(j, 0);
      Riesz_control(j - start_control + 1, 1) = control(j, 1);
      Riesz_control(j - start_control + 1, 2) = control(j, 2);
    }
  }

  gradient_Riesz = arma::solve(Riesz, Riesz * Riesz_control + rhs_Riesz);
  arma::mat return_gradient(input_data_->number_cells_position, 3,
                            arma::fill::zeros);

  //    std::cout << "Solution elliptic equation:" << std::endl;
  std::cout << gradient_Riesz << std::endl;

  for (int j = 0; j < input_data_->number_cells_position; j++) {
    if (j > start_control - 2 && j < input_data_->end_control_gp) {
      return_gradient(j, 0) = gradient_Riesz(j - start_control + 1, 0);
      return_gradient(j, 1) = gradient_Riesz(j - start_control + 1, 1);
      return_gradient(j, 2) = gradient_Riesz(j - start_control + 1, 2);
    }
  }
  std::cout << "Return_Gradient:" << std::endl;
  std::cout << return_gradient << std::endl;

  return return_gradient;
}

arma::mat GradientCalculator::CalculategradientForcecontrolSpaceHm(
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
        forwardPDF_time,
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
        backwardPDF_time,
    arma::mat control) {

  std::map<int, std::vector<double>> barycenters =
      input_data_->barycenters_list;

  PdfController pdf_control = PdfController(input_data_);
  EquationSolvingController model_solver =
      EquationSolvingController(input_data_);
  OutputDiagnostics outDiag = OutputDiagnostics(input_data_);

  if (input_data_->magnetic_force == 1 && input_data_->electric_force == 1) {
    std::cerr << "Magnetic and electric force combined is not implemented yet"
              << std::endl;
  }
  if (input_data_->magnetic_force == 0 && input_data_->electric_force == 0) {
    std::cerr << "Force/Control should be either magnetic or electric. Nothing "
                 "was specified"
              << std::endl;
  }

  arma::mat Laplace = model_solver.Laplacian3D();
  arma::mat Laplace_Squared = model_solver.LaplacianSquared3D();

  arma::mat gradient(input_data_->number_cells_position, 3, arma::fill::zeros);
  arma::mat gradient_Riesz(input_data_->dimension_control, 3,
                           arma::fill::zeros);
  arma::mat rhs_Riesz(input_data_->dimension_control, 3, arma::fill::zeros);

  double vmax_gp = input_data_->vmax_gp;
  arma::vec velocity_Discr =
      arma::linspace<arma::vec>(-vmax_gp, vmax_gp, input_data_->vcell_gp);

  double dv_gp = input_data_->dv_gp;
  double dt_gp = input_data_->dt_gp;

  // Caculate integral in gradient
  const unsigned int n = input_data_->number_cells_position;

#pragma omp parallel for
  for (unsigned int i = 1; i < n + 1; i++) {
    // int tid = omp_get_thread_num();
    std::cout << "Calculating gradient in Cell " << i << std::endl;

    std::vector<double> current_barycenter =
        barycenters.find(static_cast<int>(i))->second;

    if (current_barycenter[0] > input_data_->local_control_x_min_gp &&
        current_barycenter[0] < input_data_->local_control_x_max_gp) {

      DoublePDF forwardPDFdouble =
          DoublePDF(input_data_->ntimesteps_gp, input_data_->vcell_gp);
      DoublePDF backwardPDFdouble =
          DoublePDF(input_data_->ntimesteps_gp, input_data_->vcell_gp);

      double firstDerivativeForwardPDF_V1_current = 0.0;
      double firstDerivativeForwardPDF_V2_current = 0.0;
      double firstDerivativeForwardPDF_V3_current = 0.0;

      for (unsigned int o = 0; o < input_data_->ntimesteps_gp; o++) {
        for (unsigned int l = 0; l < input_data_->vcell_gp; l++) {
          for (unsigned int m = 0; m < input_data_->vcell_gp; m++) {
            for (unsigned int n = 0; n < input_data_->vcell_gp; n++) {
              CoordinatePhaseSpaceTime coordinate = CoordinatePhaseSpaceTime(
                  static_cast<int>(i), static_cast<int>(l), static_cast<int>(m),
                  static_cast<int>(n), static_cast<int>(o));
              auto iteratorForward = forwardPDF_time[o].find(coordinate);
              //                            auto iteratorBackward =
              //                            backwardPDF_time[o].find(coordinate);
              if (iteratorForward != forwardPDF_time[o].end()) {
                forwardPDFdouble.at(o, l, m, n) = iteratorForward->second;
                auto iteratorBackward = backwardPDF_time[o].find(coordinate);
                if (iteratorBackward != backwardPDF_time[o].end()) {
                  backwardPDFdouble.at(o, l, m, n) = iteratorBackward->second;
                }
              } else {
                forwardPDFdouble.at(o, l, m, n) = 0.0;
                backwardPDFdouble.at(o, l, m, n) = 0.0;
              }
              //                            if( iteratorBackward !=
              //                            backwardPDF_time[o].end() ) {
              //                                backwardPDFdouble[o][l][m][n]=
              //                                iteratorBackward->second;
              //                            } else {
              //                                backwardPDFdouble[o][l][m][n] =
              //                                0.0;
              //                            }
            }
          }
        }
      }

      forwardPDFdouble =
          pdf_control.RelaxatingGaussseidel4D(forwardPDFdouble, 1000);

      for (unsigned int o = 0; o < input_data_->ntimesteps_gp; o++) {
        for (unsigned int l = 0; l < input_data_->vcell_gp; l++) {
          for (unsigned int m = 0; m < input_data_->vcell_gp; m++) {
            for (unsigned int n = 0; n < input_data_->vcell_gp; n++) {
              // derivative v_x
              double forwardPDF_current = forwardPDFdouble.at(o, l, m, n);
              if (l == 0) {
                firstDerivativeForwardPDF_V1_current =
                    (forwardPDFdouble.at(o, l + 1, m, n) - forwardPDF_current) /
                    dv_gp;
              } else if (l == input_data_->vcell_gp - 1) {
                firstDerivativeForwardPDF_V1_current =
                    (forwardPDF_current - forwardPDFdouble.at(o, l - 1, m, n)) /
                    dv_gp;
              } else {
                firstDerivativeForwardPDF_V1_current =
                    (forwardPDFdouble.at(o, l + 1, m, n) -
                     forwardPDFdouble.at(o, l - 1, m, n)) /
                    (2.0 * dv_gp);
              }
              // derivative v_y
              if (m == 0) {
                firstDerivativeForwardPDF_V2_current =
                    (forwardPDFdouble.at(o, l, m + 1, n) - forwardPDF_current) /
                    dv_gp;
              } else if (m == input_data_->vcell_gp - 1) {
                firstDerivativeForwardPDF_V2_current =
                    (forwardPDF_current - forwardPDFdouble.at(o, l, m - 1, n)) /
                    dv_gp;
              } else {
                firstDerivativeForwardPDF_V2_current =
                    (forwardPDFdouble.at(o, l, m + 1, n) -
                     forwardPDFdouble.at(o, l, m - 1, n)) /
                    (2.0 * dv_gp);
              }
              // derivative v_z
              if (n == 0) {
                firstDerivativeForwardPDF_V3_current =
                    (forwardPDFdouble.at(o, l, m, n + 1) - forwardPDF_current) /
                    dv_gp;
              } else if (n == input_data_->vcell_gp - 1) {
                firstDerivativeForwardPDF_V3_current =
                    (forwardPDF_current - forwardPDFdouble.at(o, l, m, n - 1)) /
                    dv_gp;
              } else {
                firstDerivativeForwardPDF_V3_current =
                    (forwardPDFdouble.at(o, l, m, n + 1) -
                     forwardPDFdouble.at(o, l, m, n - 1)) /
                    (2.0 * dv_gp);
              }
              /*
               * Electric field
               *  */

              double backwardPDF_current = backwardPDFdouble.at(o, l, m, n);
              if (input_data_->electric_force == 1) {
                gradient(i - 1, 0) += backwardPDF_current *
                                      firstDerivativeForwardPDF_V1_current *
                                      pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
                gradient(i - 1, 1) += backwardPDF_current *
                                      firstDerivativeForwardPDF_V2_current *
                                      pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
                gradient(i - 1, 2) += backwardPDF_current *
                                      firstDerivativeForwardPDF_V3_current *
                                      pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
              }
              // integration by parts leads to
              //                            gradient_2(i-1,0) -=
              //                            forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V1_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
              //                            gradient_2(i-1,1) -=
              //                            forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V2_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
              //                            gradient_2(i-1,2) -=
              //                            forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V3_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);

              /*
               * Magnetic field
               */
              if (input_data_->magnetic_force == 1) {
                gradient(i - 1, 0) +=
                    backwardPDF_current *
                    (-velocity_Discr[n] * firstDerivativeForwardPDF_V2_current +
                     velocity_Discr[m] * firstDerivativeForwardPDF_V3_current) *
                    pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
                gradient(i - 1, 1) +=
                    backwardPDF_current *
                    (velocity_Discr[n] * firstDerivativeForwardPDF_V1_current -
                     velocity_Discr[l] * firstDerivativeForwardPDF_V3_current) *
                    pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
                gradient(i - 1, 2) +=
                    backwardPDF_current *
                    (-velocity_Discr[m] * firstDerivativeForwardPDF_V1_current +
                     velocity_Discr[l] * firstDerivativeForwardPDF_V2_current) *
                    pow(dv_gp, 3.0) * pow(dt_gp, 1.0);
              }
            }
          }
        }
      }
    } else {
      std::cout << "Cell_id " << i << ": Gradient stays zero here" << std::endl;
    }
  }

  /*
   * Assemble gradient
   */

  //    std::cout << "Gradient:" << std::endl;
  //    std::cout << gradient << std::endl;

  int start_control = static_cast<int>(input_data_->start_control_gp);

  for (int j = 0; j < input_data_->number_cells_position; j++) {
    if (j > start_control - 2 && j < input_data_->end_control_gp) {
      rhs_Riesz(j - start_control + 1, 0) = gradient(j, 0);
      rhs_Riesz(j - start_control + 1, 1) = gradient(j, 1);
      rhs_Riesz(j - start_control + 1, 2) = gradient(j, 2);
    }
  }

  std::cout << "rhs_Riesz:" << std::endl;
  std::cout << rhs_Riesz << std::endl;

  // std::cout << "Riesz Matrix" << std::endl;
  arma::mat Riesz =
      input_data_->weight_control_gp *
      (arma::eye(input_data_->dimension_control,
                 input_data_->dimension_control) -
       1.0 / (pow(input_data_->small_discr_sidelength, 2)) * Laplace +
       1.0 / (pow(input_data_->small_discr_sidelength, 4)) * Laplace_Squared);
  // std::cout << Riesz << std::endl;
  outDiag.WriteArmaMatrixToFile(Riesz, "RiesMatrix");
  // std::cout << "Condition number Matrix Riesz: " << arma::cond(Riesz) <<
  // std::endl;

  arma::mat Riesz_control(input_data_->dimension_control, 3, arma::fill::zeros);
  for (int j = 0; j < input_data_->number_cells_position; j++) {
    if (j > start_control - 2 && j < input_data_->end_control_gp) {
      Riesz_control(j - start_control + 1, 0) = control(j, 0);
      Riesz_control(j - start_control + 1, 1) = control(j, 1);
      Riesz_control(j - start_control + 1, 2) = control(j, 2);
    }
  }

  gradient_Riesz = arma::solve(Riesz, Riesz * Riesz_control + rhs_Riesz);
  // gradient_Riesz = arma::solve(Riesz,-rhs_Riesz);
  arma::mat return_gradient(input_data_->number_cells_position, 3,
                            arma::fill::zeros);

  //    std::cout << "Solution elliptic equation:" << std::endl;
  std::cout << gradient_Riesz << std::endl;

  for (int j = 0; j < input_data_->number_cells_position; j++) {
    if (j > start_control - 2 && j < input_data_->end_control_gp) {
      return_gradient(j, 0) = gradient_Riesz(j - start_control + 1, 0);
      return_gradient(j, 1) = gradient_Riesz(j - start_control + 1, 1);
      return_gradient(j, 2) = gradient_Riesz(j - start_control + 1, 2);
    }
  }
  std::cout << "Return_Gradient:" << std::endl;
  std::cout << return_gradient << std::endl;

  return return_gradient;
}

/*
arma::mat
GradientCalculator::CalculategradientForcecontrolSpaceHmPlasma(std::vector<std::unordered_map<CoordinatePhaseSpaceTime,
double>> forwardPDF_time,
                                                                              std::vector<std::unordered_map<CoordinatePhaseSpaceTime,
double>> backwardPDF_time,
                                                                              std::vector<std::unordered_map<CoordinatePhaseSpaceTime,
double>> forwardPDF_time_electrons,
                                                                              std::vector<std::unordered_map<CoordinatePhaseSpaceTime,
double>> backwardPDF_time_electrons, arma::mat control)
{
    std::map<std::string, double> optimizationParameters =
this->get_DataProviderOptim().getOptimizationParameters();
    std::map<int,std::vector<double>> barycenters =
this->get_DataProviderOptim().getMeshBarycenters();


    PdfController pdf_control = PdfController();
    pdf_control.set_DataProviderOptim(this->get_DataProviderOptim());
    EquationSolvingController model_solver = EquationSolvingController();
    model_solver.set_DataProviderOptim(this->get_DataProviderOptim());
    OutputDiagnostics outDiag = OutputDiagnostics();
    outDiag.set_DataProviderOptim(this->get_DataProviderOptim());

    unsigned int number_cells_position = static_cast<unsigned
int>(optimizationParameters.find("number_cells_position")->second); unsigned int
vcell_gp = static_cast<unsigned
int>(optimizationParameters.find("vcell_gp")->second); unsigned int
ntimesteps_gp = static_cast<unsigned
int>(optimizationParameters.find("ntimesteps_gp")->second); unsigned int
dimensionOfControl_gp = static_cast<unsigned
int>(optimizationParameters.find("dimensionOfControl_gp")->second); double dv_gp
= static_cast<double>(optimizationParameters.find("dv_gp")->second); double
dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double small_discr_sidelength =
static_cast<double>(optimizationParameters.find("small_discr_sidelength")->second);
    double weight_control_gp =
static_cast<double>(optimizationParameters.find("weight_control_gp")->second);
    double local_control_x_min_gp =
static_cast<double>(optimizationParameters.find("local_control_x_min_gp")->second);
    double local_control_x_max_gp =
static_cast<double>(optimizationParameters.find("local_control_x_max_gp")->second);

    unsigned long start_control = static_cast<unsigned
long>(optimizationParameters.find("start_control_gp")->second); unsigned long
end_control = static_cast<unsigned
long>(optimizationParameters.find("end_control_gp")->second);

    arma::mat Laplace = model_solver.Laplacian3D();
    arma::mat Laplace_Squared = model_solver.LaplacianSquared3D();

    //std::cout << Laplace << std::endl;
    //std::cout << Laplace_Squared << std::endl;

    arma::mat gradient(number_cells_position,3,arma::fill::zeros);
    arma::mat gradient_2(number_cells_position,3,arma::fill::zeros);
    arma::mat gradient_Riesz(dimensionOfControl_gp,3,arma::fill::zeros);
    arma::mat rhs_Riesz(dimensionOfControl_gp,3,arma::fill::zeros);

    //Caculate integral in gradient
    const unsigned int n = number_cells_position;

#pragma omp parallel for
    for(unsigned int i = 1; i< n+1; i++) {

        std::cout << "Calculating gradient in Cell " << i << std::endl;

        std::vector<double> current_barycenter =
barycenters.find(static_cast<int>(i))->second;

        if (current_barycenter[0]> local_control_x_min_gp &&
current_barycenter[0] < local_control_x_max_gp) {
            std::vector<std::vector<std::vector<std::vector<double>>>>
forwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                        (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
            std::vector<std::vector<std::vector<std::vector<double>>>>
backwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                         (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
            std::vector<std::vector<std::vector<std::vector<double>>>>
forwardPDFdouble_electrons(ntimesteps_gp,
std::vector<std::vector<std::vector<double>>> (vcell_gp,
std::vector<std::vector<double>> (vcell_gp, std::vector<double>
(vcell_gp,0.0)))); std::vector<std::vector<std::vector<std::vector<double>>>>
backwardPDFdouble_electrons(ntimesteps_gp,
std::vector<std::vector<std::vector<double>>> (vcell_gp,
std::vector<std::vector<double>> (vcell_gp, std::vector<double>
(vcell_gp,0.0))));

            double firstDerivativeForwardPDF_V1_current = 0.0;
            double firstDerivativeForwardPDF_V2_current = 0.0;
            double firstDerivativeForwardPDF_V3_current = 0.0;

            double firstDerivativeForwardPDF_V1_current_electrons = 0.0;
            double firstDerivativeForwardPDF_V2_current_electrons = 0.0;
            double firstDerivativeForwardPDF_V3_current_electrons = 0.0;

            for(unsigned int o = 0; o<ntimesteps_gp; o++) {
                for(unsigned int l = 0; l<vcell_gp; l++) {
                    for(unsigned int m= 0; m<vcell_gp; m++) {
                        for(unsigned int n = 0; n<vcell_gp; n++) {
                            CoordinatePhaseSpaceTime coordinate =
CoordinatePhaseSpaceTime(
static_cast<int>(i),static_cast<int>(l),static_cast<int>(m),static_cast<int>(n),static_cast<int>(o));
                            auto iteratorForward =
forwardPDF_time[o].find(coordinate);
                            //                            auto iteratorBackward
= backwardPDF_time[o].find(coordinate); if(iteratorForward !=
forwardPDF_time[o].end()) { forwardPDFdouble[o][l][m][n] =
iteratorForward->second; auto iteratorBackward =
backwardPDF_time[o].find(coordinate); if( iteratorBackward !=
backwardPDF_time[o].end() ) { backwardPDFdouble[o][l][m][n]=
iteratorBackward->second;
                                }
                            } else {
                                forwardPDFdouble[o][l][m][n] = 0.0;
                                backwardPDFdouble[o][l][m][n] = 0.0;
                            }
                            //                            if( iteratorBackward
!= backwardPDF_time[o].end() ) {
                            // backwardPDFdouble[o][l][m][n]=
iteratorBackward->second;
                            //                            } else {
                            // backwardPDFdouble[o][l][m][n] = 0.0;
                            //                            }
                            auto iteratorForward_electrons =
forwardPDF_time_electrons[o].find(coordinate); if(iteratorForward_electrons !=
forwardPDF_time_electrons[o].end()) { forwardPDFdouble_electrons[o][l][m][n] =
iteratorForward_electrons->second; auto iteratorBackward_electrons =
backwardPDF_time_electrons[o].find(coordinate); if( iteratorBackward_electrons
!= backwardPDF_time_electrons[o].end() ) {
                                    backwardPDFdouble_electrons[o][l][m][n]=
iteratorBackward_electrons->second;
                                }
                            } else {
                                forwardPDFdouble_electrons[o][l][m][n] = 0.0;
                                backwardPDFdouble_electrons[o][l][m][n] = 0.0;
                            }
                        }
                    }
                }
            }


            forwardPDFdouble =
pdf_control.RelaxatingGaussseidel4D(forwardPDFdouble,1000);

            for (unsigned int o = 0; o<ntimesteps_gp; o++) {
                for(unsigned int l = 0; l<vcell_gp; l++) {
                    for(unsigned int m= 0; m<vcell_gp; m++) {
                        for(unsigned int n = 0; n<vcell_gp; n++) {
                            // derivative v_x
                            double current_forward =
forwardPDFdouble[o][l][m][n]; double current_forward_electrons =
forwardPDFdouble_electrons[o][l][m][n]; if (l == 0) {
                                firstDerivativeForwardPDF_V1_current =
(forwardPDFdouble[o][l+1][m][n]-current_forward)/dv_gp;
                                firstDerivativeForwardPDF_V1_current_electrons =
(forwardPDFdouble_electrons[o][l+1][m][n]-current_forward_electrons)/dv_gp;
                                //firstDerivativeBackwardPDF_V1_current =
(backwardPDFdouble[o][l+1][m][n]-backwardPDFdouble[o][l][m][n])/dv_gp; } else if
(l == vcell_gp-1) { firstDerivativeForwardPDF_V1_current =
(current_forward-forwardPDFdouble[o][l-1][m][n])/dv_gp;
                                firstDerivativeForwardPDF_V1_current_electrons =
(current_forward_electrons-forwardPDFdouble_electrons[o][l-1][m][n])/dv_gp;
                                //firstDerivativeBackwardPDF_V1_current =
(backwardPDFdouble[o][l][m][n]-backwardPDFdouble[o][l-1][m][n])/dv_gp; } else {
                                firstDerivativeForwardPDF_V1_current =
(forwardPDFdouble[o][l+1][m][n]-forwardPDFdouble[o][l-1][m][n])/(2.0*dv_gp);
                                firstDerivativeForwardPDF_V1_current_electrons =
(forwardPDFdouble_electrons[o][l+1][m][n]-forwardPDFdouble_electrons[o][l-1][m][n])/(2.0*dv_gp);
                                //firstDerivativeBackwardPDF_V1_current =
(backwardPDFdouble[o][l+1][m][n]-backwardPDFdouble[o][l-1][m][n])/(2.0*dv_gp);
                            }
                            //derivative v_y
                            if (m== 0) {
                                firstDerivativeForwardPDF_V2_current =
(forwardPDFdouble[o][l][m+1][n]-current_forward)/dv_gp;
                                firstDerivativeForwardPDF_V2_current_electrons =
(forwardPDFdouble_electrons[o][l][m+1][n]-current_forward_electrons)/dv_gp;
                                //firstDerivativeBackwardPDF_V2_current =
(backwardPDFdouble[o][l][m+1][n]-backwardPDFdouble[o][l][m][n])/dv_gp; } else if
(m == vcell_gp-1) { firstDerivativeForwardPDF_V2_current =
(current_forward-forwardPDFdouble[o][l][m-1][n])/dv_gp;
                                firstDerivativeForwardPDF_V2_current_electrons =
(current_forward_electrons-forwardPDFdouble_electrons[o][l][m-1][n])/dv_gp;
                                //firstDerivativeBackwardPDF_V2_current =
(backwardPDFdouble[o][l][m][n]-backwardPDFdouble[o][l][m-1][n])/dv_gp; } else {
                                firstDerivativeForwardPDF_V2_current =
(forwardPDFdouble[o][l][m+1][n]-forwardPDFdouble[o][l][m-1][n])/(2.0*dv_gp);
                                firstDerivativeForwardPDF_V2_current_electrons =
(forwardPDFdouble_electrons[o][l][m+1][n]-forwardPDFdouble_electrons[o][l][m-1][n])/(2.0*dv_gp);
                                //firstDerivativeBackwardPDF_V2_current =
(backwardPDFdouble[o][l][m+1][n]-backwardPDFdouble[o][l][m-1][n])/(2.0*dv_gp);
                            }
                            //derivative v_z
                            if (n == 0) {
                                firstDerivativeForwardPDF_V3_current =
(forwardPDFdouble[o][l][m][n+1]-current_forward)/dv_gp;
                                firstDerivativeForwardPDF_V3_current_electrons =
(forwardPDFdouble_electrons[o][l][m][n+1]-current_forward_electrons)/dv_gp;
                                //firstDerivativeBackwardPDF_V3_current =
(backwardPDFdouble[o][l][m][n+1]-backwardPDFdouble[o][l][m][n])/dv_gp; } else if
(n == vcell_gp-1) { firstDerivativeForwardPDF_V3_current =
(current_forward-forwardPDFdouble[o][l][m][n-1])/dv_gp;
                                firstDerivativeForwardPDF_V3_current_electrons =
(current_forward_electrons-forwardPDFdouble_electrons[o][l][m][n-1])/dv_gp;
                                //firstDerivativeBackwardPDF_V3_current =
(backwardPDFdouble[o][l][m][n]-backwardPDFdouble[o][l][m][n-1])/dv_gp; } else {
                                firstDerivativeForwardPDF_V3_current =
(forwardPDFdouble[o][l][m][n+1]-forwardPDFdouble[o][l][m][n-1])/(2.0*dv_gp);
                                firstDerivativeForwardPDF_V3_current_electrons =
(forwardPDFdouble_electrons[o][l][m][n+1]-forwardPDFdouble_electrons[o][l][m][n-1])/(2.0*dv_gp);
                                //firstDerivativeBackwardPDF_V3_current =
(backwardPDFdouble[o][l][m][n+1]-backwardPDFdouble[o][l][m][n-1])/(2.0*dv_gp);
                            }

                            //DIFFERENT MASSES!!!
                            gradient(i-1,0) +=
backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V1_current*pow(dv_gp,3.0)*pow(dt_gp,1.0)
                                    - 1.0/6.0*backwardPDFdouble_electrons[o][l][m][n]*firstDerivativeForwardPDF_V1_current_electrons*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            gradient(i-1,1) +=
backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V2_current*pow(dv_gp,3.0)*pow(dt_gp,1.0)
                                    - 1.0/6.0*backwardPDFdouble_electrons[o][l][m][n]*firstDerivativeForwardPDF_V2_current_electrons*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            gradient(i-1,2) +=
backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V3_current*pow(dv_gp,3.0)*pow(dt_gp,1.0)
                                    - 1.0/6.0*backwardPDFdouble_electrons[o][l][m][n]*firstDerivativeForwardPDF_V3_current_electrons*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            //integration by parts leads to
                            //                            gradient_2(i-1,0) -=
forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V1_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            //                            gradient_2(i-1,1) -=
forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V2_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            //                            gradient_2(i-1,2) -=
forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V3_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                        }
                    }
                }
            }
        } else {
            std::cout << "Cell_id " << i << ": Gradient stays zero here" <<
std::endl;
        }
    }




    //  Assemble gradient


    //    std::cout << "Gradient:" << std::endl;
    //    std::cout << gradient << std::endl;


    for(unsigned long j = 0; j < number_cells_position; j++) {
        if (j>start_control-2 && j<end_control) {
            rhs_Riesz(j-start_control+1,0) = gradient(j,0);
            rhs_Riesz(j-start_control+1,1) = gradient(j,1);
            rhs_Riesz(j-start_control+1,2) = gradient(j,2);
        }
    }

    //std::cout << "rhs_Riesz:" << std::endl;
    std::cout << rhs_Riesz << std::endl;

    // std::cout << "Riesz Matrix" << std::endl;
    arma::mat Riesz =
weight_control_gp*(arma::eye(dimensionOfControl_gp,dimensionOfControl_gp)
- 1.0/(pow(small_discr_sidelength,2))*Laplace
+ 1.0/(pow(small_discr_sidelength,4))*Laplace_Squared);
    //std::cout << Riesz << std::endl;
    outDiag.WriteArmaMatrixToFile(Riesz,"RiesMatrix");
    //std::cout << "Condition number Matrix Riesz: " << arma::cond(Riesz) <<
std::endl;

    arma::mat   Riesz_control(dimensionOfControl_gp,3,arma::fill::zeros);
    for(unsigned long j = 0; j < number_cells_position; j++) {
        if (j>start_control-2 && j<end_control) {
            Riesz_control(j-start_control+1,0) = control(j,0);
            Riesz_control(j-start_control+1,1) = control(j,1);
            Riesz_control(j-start_control+1,2) = control(j,2);
        }
    }

    gradient_Riesz = arma::solve(Riesz,Riesz*Riesz_control+rhs_Riesz);
    //gradient_Riesz = arma::solve(Riesz,-rhs_Riesz);
    arma::mat return_gradient(number_cells_position,3,arma::fill::zeros);

    //    std::cout << "Solution elliptic equation:" << std::endl;
    std::cout << gradient_Riesz << std::endl;

    for(unsigned long j = 0; j < number_cells_position; j++) {
        if (j>start_control-2 && j<end_control) {
            return_gradient(j,0) = gradient_Riesz(j-start_control+1,0);
            return_gradient(j,1) = gradient_Riesz(j-start_control+1,1);
            return_gradient(j,2) = gradient_Riesz(j-start_control+1,2);
        }
    }
    std::cout << "Return_Gradient:" << std::endl;
    std::cout << return_gradient << std::endl;

    return return_gradient;
}*/
