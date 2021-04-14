#include "parametersanity.h"

ParameterSanity::ParameterSanity(
    std::shared_ptr<MOTIONS::InputData> &input_data) {
  this->setInput_data(input_data);
}

int ParameterSanity::CheckAdjointVelocity() {
  std::ostringstream streamObj;

  double kb = 1.38064852 * std::pow(10, -23);

  double sigma_v_forward = std::sqrt(2.0 * kb * input_data_->temperature_x_val /
                                     input_data_->mass_forward);
  double lowerBound_expectedSpeed = sigma_v_forward / std::sqrt(M_PI / 8.0);
  double upperBound_expectedSpeed = input_data_->vmax_gp - sigma_v_forward;

  std::cout << "For the expected adjoint speed it shoud hold: "
            << lowerBound_expectedSpeed << " <= " << input_data_->expected_speed
            << " <= " << upperBound_expectedSpeed << std::endl;

  if (lowerBound_expectedSpeed >= input_data_->expected_speed ||
      upperBound_expectedSpeed <= input_data_->expected_speed) {
    std::string error_string =
        "Warning! Expected speed does not obey its bounds!";
    std::cout << error_string << std::endl;
    return 1;
  }

  return 0;
}

int ParameterSanity::CheckVelocityDiscretization() {
  std::ostringstream streamObj;

  double max = input_data_->dv_gp * input_data_->vcell_gp;

  if (std::fabs(max - 2.0 * input_data_->vmax_gp) > input_data_->fabs_tol_gp) {
    std::cerr << "Discretization of velocity space is erroneous" << std::endl;
    return 1;
  }

  return 0;
}
