#include <gtest/gtest.h>

#include "../../src/optimization/gradient_calculator.h"
#include "../../src/io/input.h"

TEST(gradient,calculationNR1) {

    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    gradient_calculator gradient_calculator_opt = gradient_calculator(filename);
    input input_control = input();
    input_control.setData_provider_optim(provider);
    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(provider);

    arma::mat gradient(dimensionOfControl_gp,3,arma::fill::zeros);

    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF;
    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> backwardPDF;

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        forwardParticles[o-1] = input::readParticleVector("./data/vstrap_test_output/plasma_state_batch_1_forward_particles_CPU_"+std::to_string(o)+".csv",",");
    }

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        backwardParticles[ntimesteps_gp - o] = input::readParticleVector("./data/vstrap_test_output/plasma_state_batch_1_adjoint_particles_CPU_"+std::to_string(o)+".csv",",");
    }


    forwardPDF = pdf_control.assemblingMultiDim_parallel(forwardParticles,0);
    backwardPDF = pdf_control.assemblingMultiDim_parallel(backwardParticles,1);

    arma::mat control(dimensionOfControl_gp,dimensionOfControl_gp,arma::fill::zeros);

    gradient = gradient_calculator_opt.calculateGradient_forceControl_space_Hm(forwardPDF,backwardPDF,control);
    double norm = arma::norm(gradient,"fro");
    double norm_target = 0.0058309765376422761;

    double diff = std::abs(norm-norm_target);

    EXPECT_LE(diff,pow(10,-5));
}
