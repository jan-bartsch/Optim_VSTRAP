#include "objectivecalculator.h"

ObjectiveCalculator::ObjectiveCalculator(
    std::shared_ptr<MOTIONS::InputData> &input_data) {
  this->setInput_data(input_data);
}

double ObjectiveCalculator::CalculateObjective(
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
        &forwardPDF_time,
    arma::mat control) {

  EquationSolvingController solver = EquationSolvingController(input_data_);
  Input in = Input(input_data_);
  DesiredTrajectoryController trajectory_controller =
      DesiredTrajectoryController(input_data_);

  unsigned int number_cells_position = input_data_->number_cells_position;
  double vmax_gp = input_data_->vmax_gp;
  unsigned int vcell_gp = input_data_->vcell_gp;
  unsigned int plasma_state_output_interval =
      input_data_->plasma_state_output_interval;

  double dv_gp = input_data_->dv_gp;
  double dt_gp = input_data_->dt_gp;
  double small_discr_volume = input_data_->small_discr_sidelength;
  double sigma_v_gp = input_data_->sigma_v;
  double velocity_part_objective = input_data_->velocity_part_objective;

  static arma::vec velocityDiscr_gp =
      arma::linspace<arma::vec>(-vmax_gp, vmax_gp, vcell_gp);

  std::map<int, std::vector<double>> baryc = input_data_->barycenters_list;

  // std::cout << baryc.at(0)[0] << std::endl;

  double objective = 0.0;
  double costOfControl = 0.0;

  std::vector<double> objective_time(number_cells_position, 0.0);

  std::vector<std::vector<double>> brockettVector = in.ReadBrockettFile(
      input_data_->path_to_shared_files + "brockett.csv", ",",
      input_data_->ntimesteps_gp * plasma_state_output_interval);

  /*
   * Add terminal integral
   */

  /*for(unsigned int i = 0; i<number_cells_position; i++)  {
   double current_termPot;
   std::vector<double> p_d;
      for(unsigned int l = 0; l<vcell_gp; l++) {
          for(unsigned int m = 0; m<vcell_gp; m++) {
              for(unsigned int n = 0; n<vcell_gp; n++) {
                  p_d =
  trajectory_controller.trajectory_desired(i,l,m,n,ntimesteps_gp-1);
                  coordinate_phase_space_time coordinate =
  coordinate_phase_space_time(i,l,m,n,ntimesteps_gp-1);
                  //termPot[i][l][m][n]
                  current_termPot = -
  C_phi_gp/(2.0*M_PI*sigma_x_gp*sigma_v_gp)*exp(
                              -(
                                  pow(positionDiscr_gp(i)-p_d[0],2.0)/(2.0*sigma_x_gp*sigma_x_gp)+
                              pow(velocityDiscr_gp(l)-p_d[3],2.0)/(2.0*sigma_v_gp*sigma_v_gp)+
                          pow(velocityDiscr_gp(m)-p_d[4],2.0)/(2.0*sigma_v_gp*sigma_v_gp)+
                          pow(velocityDiscr_gp(n)-p_d[5],2.0)/(2.0*sigma_v_gp*sigma_v_gp)
                          ));
                  if (forwardPDF.find(coordinate) != forwardPDF.end()) {
                      objective +=
  forwardPDF.at(coordinate)*current_termPot*pow(small_discr_volume,3)*pow(dv_gp,3);

                  }
              }
          }
      }
  }*/

  /*
   * Add tracking integral using first-order(?) RULE
   */
  std::string desired_traj = input_data_->desired_trajectory;
  std::string objective_calculation = input_data_->objective_calculation;

  std::cout << "Using <" << desired_traj << "> for desired trajectory"
            << std::endl;

#pragma omp parallel for
  for (unsigned int i = 1; i <= input_data_->number_cells_position; i++) {
    std::cout << "Functionalcalc. in part " << i << std::endl;
    std::vector<double> current_barycenter =
        baryc.find(static_cast<int>(i))->second;
    if (current_barycenter[0] > -0.04) {

      for (unsigned int o = 0; o < input_data_->ntimesteps_gp; o++) {
        // std::cout << "Calculating functional in " << o << " timestep" <<
        // std::endl;
        std::vector<double> p_d(6, 0.0);
        double sigma_x_1 = 0.0;
        double sigma_x_2 = 0.0;
        double sigma_x_3 = 0.0;

        if (desired_traj.compare("brockett") == 0) {
          sigma_x_1 = brockettVector[o * plasma_state_output_interval][3];
          sigma_x_2 = brockettVector[o * plasma_state_output_interval][4];
          sigma_x_3 = brockettVector[o * plasma_state_output_interval][5];

          p_d = trajectory_controller.TrajectoryDesiredBrockett(
              brockettVector, o, plasma_state_output_interval);
        }

        // TODO: this currently works only for brockett
        double scaling_gaussian = std::sqrt(
            pow(2.0 * M_PI, 6.0) * pow(sigma_x_1 * sigma_x_2 * sigma_x_3, 2.0) *
            pow(sigma_v_gp * sigma_v_gp, 3.0));

        for (unsigned int l = 0; l < vcell_gp; l++) {
          for (unsigned int m = 0; m < vcell_gp; m++) {
            for (unsigned int n = 0; n < vcell_gp; n++) {
              if (desired_traj.compare("brockett") != 0) {
                p_d = trajectory_controller.TrajectoryDesired(
                    current_barycenter, l, m, n, o, brockettVector,
                    plasma_state_output_interval);
              }
              CoordinatePhaseSpaceTime coordinate = CoordinatePhaseSpaceTime(
                  static_cast<int>(i), static_cast<int>(l), static_cast<int>(m),
                  static_cast<int>(n), static_cast<int>(o));
              // std::cout << velocityDiscr_gp(l) << std::endl;
              double current_trackPot = 0.0;

              if (objective_calculation.compare("magnitude") == 0) {
                current_trackPot =
                    -1.0 / scaling_gaussian *
                        exp(-(
                            std::pow(current_barycenter[0] - p_d[0], 2) /
                                (2.0 * sigma_x_1 * sigma_x_1) +
                            std::pow(current_barycenter[1] - p_d[1], 2) /
                                (2.0 * sigma_x_2 * sigma_x_2) +
                            std::pow(current_barycenter[2] - p_d[2], 2) /
                                (2.0 * sigma_x_3 * sigma_x_3) +
                            velocity_part_objective *
                                (std::abs(
                                    velocityDiscr_gp(l) * velocityDiscr_gp(l) +
                                    velocityDiscr_gp(m) * velocityDiscr_gp(m) +
                                    velocityDiscr_gp(n) * velocityDiscr_gp(n) -
                                    p_d[4] * p_d[4])) /
                                (2.0 * sigma_v_gp * sigma_v_gp))) +
                    input_data_->shifting_objective;
              } else if (objective_calculation.compare("components") == 0) {
                current_trackPot =
                    -1.0 /
                        std::sqrt(pow(2.0 * M_PI, 6.0) *
                                  pow(sigma_x_1 * sigma_x_2 * sigma_x_3, 2.0) *
                                  pow(sigma_v_gp * sigma_v_gp, 3.0)) *
                        exp(-(std::pow(current_barycenter[0] - p_d[0], 2) /
                                  (2.0 * sigma_x_1 * sigma_x_1) +
                              std::pow(current_barycenter[1] - p_d[1], 2) /
                                  (2.0 * sigma_x_2 * sigma_x_2) +
                              std::pow(current_barycenter[2] - p_d[2], 2) /
                                  (2.0 * sigma_x_3 * sigma_x_3) +
                              velocity_part_objective *
                                  pow(velocityDiscr_gp(l) - p_d[3], 2.0) /
                                  (2.0 * sigma_v_gp * sigma_v_gp) +
                              velocity_part_objective *
                                  pow(velocityDiscr_gp(m) - p_d[4], 2.0) /
                                  (2.0 * sigma_v_gp * sigma_v_gp) +
                              velocity_part_objective *
                                  pow(velocityDiscr_gp(n) - p_d[5], 2.0) /
                                  (2.0 * sigma_v_gp * sigma_v_gp))) +
                    input_data_->shifting_objective;
              }
              if (forwardPDF_time[o].find(coordinate) !=
                  forwardPDF_time[o].end()) {
                objective_time[i] += forwardPDF_time[o].at(coordinate) *
                                     current_trackPot * small_discr_volume *
                                     pow(dv_gp, 3.0) * dt_gp;
              } else {
                std::runtime_error("No such objective calculation rule");
              }
              if (objective_time[o] > 0) {
                // logger::Info("Functional tracking part positiv");
                // std::runtime_error("Functional tracking part positiv");
              }
            }
          }
        }
      }
    }
  }

  for (unsigned int o = 0; o < input_data_->number_cells_position; o++) {
    if (std::isnan(objective_time[o])) {
      std::runtime_error("Encountered NAN in functional calculation");
    }
    objective += objective_time[o];
  }

  InnerProducts product = InnerProducts(input_data_);

  costOfControl += std::sqrt(product.H2InnerProduct(control, control));

  objective += 1.0 / input_data_->C_theta * input_data_->weight_control_gp *
               costOfControl;

  if (objective > 0) {
    logger::Info(
        "Value of functional positiv - influence of control may be to big");
  }

  std::cout << "Value of objective: " << objective << std::endl;

  return objective;
}
