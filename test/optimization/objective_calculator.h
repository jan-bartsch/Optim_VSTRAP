#include <gtest/gtest.h>

#include "../../src/optimization/objective_calculator.h"
#include "../../src/io/input.h"

TEST(objective,calculation1) {

    std::string input_directory = "/home/jan/Promotion_linuxPC/Optim_VSTRAP/test/data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);

    objective_calculator calculator = objective_calculator();
    pdf_controller pdf_control = pdf_controller();
    input input_control = input();

    //    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    //    input_control.read_plasma_state_forward(forwardParticles);

    //    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF;
    //    forwardPDF = pdf_control.assemblingMultiDim_parallel(forwardParticles,0);

    //    arma::mat control(64,64,arma::fill::zeros);

    double value = 0.0;
    //    calculator.calculate_objective_L2(forwardPDF,control);

    double target_value = pow(-2.27442,-50);

    EXPECT_EQ(value,target_value);



}
