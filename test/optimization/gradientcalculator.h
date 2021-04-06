#include <gtest/gtest.h>

#include <exception>

#include "../../src/optimization/gradientcalculator.h"
#include "../../src/io/input.h"
#include "../../src/controller/pdfcontroller.h"
#include "../../src/io/outputdiagnostics.h"
#include "../../src/controller/equationsolvingcontroller.h"
#include "../../src/optimization/objectivecalculator.h"
#include "../../src/tools/innerproducts.h"


TEST(gradient,calculationNR1) {

    std::string Input_directory = "./data/Optim_Input_gTest.xml";
    const char *  filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);
    InnerProducts pro = InnerProducts();
    pro.set_DataProviderOptim(provider);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int number_cells_position = static_cast<unsigned int>(optimizationParameters.find("number_cells_position")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    GradientCalculator gradient_calculator_opt = GradientCalculator(filename);
    Input Input_control = Input();
    Input_control.set_DataProviderOptim(provider);
    PdfController pdf_control = PdfController();
    pdf_control.set_DataProviderOptim(provider);

    arma::mat gradient(number_cells_position,3,arma::fill::zeros);
    arma::mat gradient_not_parallel(number_cells_position,3,arma::fill::zeros);


    std::vector<std::vector<Particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double>> forwardPDF;
    std::vector<std::vector<Particle>> backwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double>> backwardPDF;

    std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> pdf_time(ntimesteps_gp);
    int assembling_flag_forward;
    int assembling_flag_backward;

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        forwardParticles[o-1] = Input::ReadParticleVector("./data/vstrap_test_output/plasma_state_batch_1_forward_particles_CPU_"+std::to_string(o)+".csv",",");
    }

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        backwardParticles[ntimesteps_gp - o] = Input::ReadParticleVector("./data/vstrap_test_output/plasma_state_batch_1_adjoint_particles_CPU_"+std::to_string(o)+".csv",",");
    }

    assembling_flag_forward = pdf_control.AssemblingMultidimParallel(forwardParticles,0,pdf_time);
    forwardPDF = pdf_time;
    assembling_flag_backward = pdf_control.AssemblingMultidimParallel(backwardParticles,1,pdf_time);
    backwardPDF = pdf_time;


    arma::mat control(number_cells_position,3,arma::fill::zeros);

    try {
        gradient = gradient_calculator_opt.CalculategradientForcecontrolSpaceHm(forwardPDF,backwardPDF,control);
    } catch (std::exception e)  {
        std::cout << "Error in calculateGradient_forceControl_space_Hm" << std::endl;
        std::cout << e.what() << std::endl;
    }
    try {
        gradient_not_parallel = gradient_calculator_opt.CalculategradientForcecontrolSpaceHmNotParallel(forwardPDF,backwardPDF,control);
    } catch (std::exception e)  {
        std::cout << "Error in calculateGradient_forceControl_space_Hm_not_parallel" << std::endl;
        std::cout << e.what() << std::endl;
    }

    double norm_diff = pro.L2InnerProduct(gradient-gradient_not_parallel,gradient-gradient_not_parallel);

    ASSERT_LE(norm_diff,pow(10,-5));
}

