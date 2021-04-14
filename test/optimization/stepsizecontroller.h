#include <gtest/gtest.h>

#include "../../src/objects/MOTIONS.h"
#include "../../src/optimization/stepsizecontroller.h"

TEST(stepdirection, noSuchMethod) {
    std::string Input_directory = "./data/Optim_input_gTest.xml";
    const char *filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);
    auto shared_input_data = std::make_shared<MOTIONS::InputData>(
                MOTIONS::InitializeMotions::Load_MOTIONS(provider));
    StepsizeController size_contr = StepsizeController(shared_input_data);

    shared_input_data->control_update = "FAIL_SAFE";

    unsigned int number_cells_position = shared_input_data->number_cells_position;
    unsigned int ntimesteps_gp = shared_input_data->ntimesteps_gp;

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
    std::string Input_directory = "./data/Optim_input_gTest.xml";
    const char *filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);
    auto shared_input_data = std::make_shared<MOTIONS::InputData>(
                MOTIONS::InitializeMotions::Load_MOTIONS(provider));
    StepsizeController size_contr = StepsizeController(shared_input_data);

    shared_input_data->control_update = "gradient_descent";

    unsigned int number_cells_position = shared_input_data->number_cells_position;
    unsigned int ntimesteps_gp = shared_input_data->ntimesteps_gp;

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


TEST(stepdirection,armijoLinesearchVSTRAPnotFoundThrow) {
    std::string Input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);
    auto shared_input_data = std::make_shared<MOTIONS::InputData>(
                MOTIONS::InitializeMotions::Load_MOTIONS(provider));

    StepsizeController size_contr = StepsizeController(shared_input_data);
    shared_input_data->control_update = "armijo_linesearch";

    unsigned int number_cells_position = shared_input_data->number_cells_position;
    unsigned int
            ntimesteps_gp = shared_input_data->ntimesteps_gp;

    arma::mat gradient(number_cells_position,3,arma::fill::randn);
    arma::mat control_test(number_cells_position,3,arma::fill::randu);
    double J0 = -1.0;
    std::vector<Particle> input_particles(ntimesteps_gp);
    double stepsize0 = 0.1;

    arma::mat stepdirection(number_cells_position,3,arma::fill::randu);
    arma::mat control = control_test;
    double stepsize = 0.1;

    arma::mat updated_control_test = control_test + stepsize*stepdirection;
    EXPECT_ANY_THROW(size_contr.CalculateStepsize(gradient,J0,control,stepdirection,input_particles,stepsize0));
}


TEST(stepdirection, successiveApproximationNoThrow) {
    std::string Input_directory = "./data/Optim_input_gTest.xml";
    const char *filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);
    auto shared_input_data = std::make_shared<MOTIONS::InputData>(
                MOTIONS::InitializeMotions::Load_MOTIONS(provider));
    StepsizeController size_contr = StepsizeController(shared_input_data);

    shared_input_data->control_update = "successive_approximation";

    unsigned int number_cells_position = shared_input_data->number_cells_position;
    unsigned int ntimesteps_gp = shared_input_data->ntimesteps_gp;

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
