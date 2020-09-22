#include <gtest/gtest.h>

#include <exception>

#include "../../src/optimization/gradient_calculator.h"
#include "../../src/io/input.h"
#include "../../src/controller/pdf_controller.h"
#include "../../src/io/output_diagnostics.h"
#include "../../src/controller/equation_solving_controller.h"
#include "../../src/optimization/objective_calculator.h"


TEST(gradient,calculationNR1) {

    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    gradient_calculator gradient_calculator_opt = gradient_calculator(filename);
    input input_control = input();
    input_control.setData_provider_optim(provider);
    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(provider);

    arma::mat gradient(pcell_gp,3,arma::fill::zeros);

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


    arma::mat control(pcell_gp,3,arma::fill::zeros);

    try {
        gradient = gradient_calculator_opt.calculateGradient_forceControl_space_Hm(forwardPDF,backwardPDF,control);
    } catch (std::exception e)  {
        std::cout << e.what() << std::endl;
    }
    EXPECT_NO_THROW();
}

