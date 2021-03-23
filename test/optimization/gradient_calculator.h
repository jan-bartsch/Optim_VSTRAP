#include <gtest/gtest.h>

#include <exception>

#include "../../src/optimization/gradient_calculator.h"
#include "../../src/io/input.h"
#include "../../src/controller/pdf_controller.h"
#include "../../src/io/output_diagnostics.h"
#include "../../src/controller/equation_solving_controller.h"
#include "../../src/optimization/objective_calculator.h"
#include "../../src/tools/inner_products.h"


TEST(gradient,calculationNR1) {

    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    inner_products pro = inner_products();
    pro.setData_provider_optim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int number_cells_position = static_cast<unsigned int>(optimizationParameters.find("number_cells_position")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    gradient_calculator gradient_calculator_opt = gradient_calculator(filename);
    input input_control = input();
    input_control.setData_provider_optim(provider);
    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(provider);

    arma::mat gradient(number_cells_position,3,arma::fill::zeros);
    arma::mat gradient_not_parallel(number_cells_position,3,arma::fill::zeros);


    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF;
    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> backwardPDF;

    std::vector<std::unordered_map<coordinate_phase_space_time, double>> pdf_time(ntimesteps_gp);
    int assembling_flag_forward;
    int assembling_flag_backward;

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        forwardParticles[o-1] = input::readParticleVector("./data/vstrap_test_output/plasma_state_batch_1_forward_particles_CPU_"+std::to_string(o)+".csv",",");
    }

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        backwardParticles[ntimesteps_gp - o] = input::readParticleVector("./data/vstrap_test_output/plasma_state_batch_1_adjoint_particles_CPU_"+std::to_string(o)+".csv",",");
    }

    assembling_flag_forward = pdf_control.assemblingMultiDim_parallel(forwardParticles,0,pdf_time);
    forwardPDF = pdf_time;
    assembling_flag_backward = pdf_control.assemblingMultiDim_parallel(backwardParticles,1,pdf_time);
    backwardPDF = pdf_time;


    arma::mat control(number_cells_position,3,arma::fill::zeros);

    try {
        gradient = gradient_calculator_opt.calculateGradient_forceControl_space_Hm(forwardPDF,backwardPDF,control);
    } catch (std::exception e)  {
        std::cout << "Error in calculateGradient_forceControl_space_Hm" << std::endl;
        std::cout << e.what() << std::endl;
    }
    try {
        gradient_not_parallel = gradient_calculator_opt.calculateGradient_forceControl_space_Hm_not_parallel(forwardPDF,backwardPDF,control);
    } catch (std::exception e)  {
        std::cout << "Error in calculateGradient_forceControl_space_Hm_not_parallel" << std::endl;
        std::cout << e.what() << std::endl;
    }

    double norm_diff = pro.L2_inner_product(gradient-gradient_not_parallel,gradient-gradient_not_parallel);

    ASSERT_LE(norm_diff,pow(10,-5));
}

