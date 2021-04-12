#include <gtest/gtest.h>

#include "../../src/optimization/stepsizecontroller.h"

TEST(stepdirection, noSuchMethod) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
  StepsizeController size_contr = StepsizeController(filename);

  std::map<std::string, std::string> subs = provider.getSubroutines();
  subs.erase("control_update");
  subs.insert(
      std::pair<std::string, std::string>("control_update", "FAIL_SAFE"));

  std::map<std::string, double> optimizationParameters =
      provider.getOptimizationParameters();
  unsigned int number_cells_position = static_cast<unsigned int>(
      optimizationParameters.find("number_cells_position")->second);
  unsigned int ntimesteps_gp = static_cast<unsigned int>(
      optimizationParameters.find("ntimesteps_gp")->second);

  provider.setSubroutines(subs);

  size_contr.set_DataProviderOptim(provider);

  arma::mat gradient(number_cells_position, 3, arma::fill::randn);
  arma::mat control(number_cells_position, 3, arma::fill::randu);
  double J0 = -1.0;
  arma::mat stepdirection(number_cells_position, 3, arma::fill::randn);

  std::vector<Particle> Input_particles(ntimesteps_gp);
  double stepsize0 = 0.1;

  int stepsize_flag = size_contr.CalculateStepsize(
      gradient, J0, control, stepdirection, Input_particles, stepsize0);

  EXPECT_EQ(stepsize_flag, 1);
}

TEST(stepdirection, gradientDescentNoThrow) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
  StepsizeController size_contr = StepsizeController(filename);

  std::map<std::string, std::string> subs = provider.getSubroutines();
  subs.erase("control_update");
  subs.insert(std::pair<std::string, std::string>("control_update",
                                                  "gradient_descent"));
  provider.setSubroutines(subs);
  size_contr.set_DataProviderOptim(provider);

  std::map<std::string, double> optimizationParameters =
      provider.getOptimizationParameters();
  unsigned int number_cells_position = static_cast<unsigned int>(
      optimizationParameters.find("number_cells_position")->second);
  unsigned int ntimesteps_gp = static_cast<unsigned int>(
      optimizationParameters.find("ntimesteps_gp")->second);

  arma::mat gradient(number_cells_position, 3, arma::fill::randn);
  arma::mat control_test(number_cells_position, 3, arma::fill::randu);
  double J0 = -1.0;
  std::vector<Particle> Input_particles(ntimesteps_gp);
  double stepsize0 = 0.1;

  arma::mat stepdirection(number_cells_position, 3, arma::fill::randu);
  arma::mat control = control_test;
  double stepsize = 0.1;

  arma::mat updated_control_test = control_test + stepsize * stepdirection;
  int return_flag = size_contr.CalculateStepsize(
      gradient, J0, control, stepdirection, Input_particles, stepsize0);

  EXPECT_EQ(return_flag, 0);
}

/*
TEST(stepdirection,armijoLinesearchNoThrow) {
    std::string Input_directory = "./data/Optim_Input_gTest.xml";
    const char *  filename = Input_directory.c_str();

    data_provider provider = data_provider(filename);
    stepsize_controller size_contr = stepsize_controller(filename);

    std::map<std::string, std::string> subs = provider.getSubroutines();
    subs.erase("control_update");
    subs.insert(std::pair<std::string,std::string>("control_update","armijo_linesearch"));
    provider.setSubroutines(subs);
    size_contr.set_DataProviderOptim(provider);

    std::map<std::string, double> optimizationParameters =
provider.getOptimizationParameters(); unsigned int number_cells_position =
static_cast<unsigned
int>(optimizationParameters.find("number_cells_position")->second); unsigned int
ntimesteps_gp = static_cast<unsigned
int>(optimizationParameters.find("ntimesteps_gp")->second);

    arma::mat gradient(number_cells_position,3,arma::fill::randn);
    arma::mat control_test(number_cells_position,3,arma::fill::randu);
    double J0 = -1.0;
    std::vector<particle> Input_particles(ntimesteps_gp);
    double stepsize0 = 0.1;

    arma::mat stepdirection(number_cells_position,3,arma::fill::randu);
    arma::mat control = control_test;
    double stepsize = 0.1;

    arma::mat updated_control_test = control_test + stepsize*stepdirection;
    EXPECT_NO_THROW(size_contr.calculate_stepsize(gradient,J0,control,stepdirection,Input_particles,stepsize0));
}
*/

TEST(stepdirection, successiveApproximationNoThrow) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
  StepsizeController size_contr = StepsizeController(filename);

  std::map<std::string, std::string> subs = provider.getSubroutines();
  subs.erase("control_update");
  subs.insert(std::pair<std::string, std::string>("control_update",
                                                  "successive_approximation"));
  provider.setSubroutines(subs);
  size_contr.set_DataProviderOptim(provider);

  std::map<std::string, double> optimizationParameters =
      provider.getOptimizationParameters();
  unsigned int number_cells_position = static_cast<unsigned int>(
      optimizationParameters.find("number_cells_position")->second);
  unsigned int ntimesteps_gp = static_cast<unsigned int>(
      optimizationParameters.find("ntimesteps_gp")->second);

  arma::mat gradient(number_cells_position, 3, arma::fill::randn);
  arma::mat control_test(number_cells_position, 3, arma::fill::randu);
  double J0 = -1.0;
  std::vector<Particle> Input_particles(ntimesteps_gp);
  double stepsize0 = 0.1;

  arma::mat stepdirection(number_cells_position, 3, arma::fill::randu);
  arma::mat control = control_test;
  double stepsize = 0.1;

  arma::mat updated_control_test = control_test + stepsize * stepdirection;
  int return_flag = size_contr.CalculateStepsize(
      gradient, J0, control, stepdirection, Input_particles, stepsize0);

  EXPECT_EQ(return_flag, 0);
}
