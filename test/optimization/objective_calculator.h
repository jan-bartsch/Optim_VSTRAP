#include <gtest/gtest.h>

#include "../../src/optimization/objective_calculator.h"
#include "../../src/io/input.h"

TEST(objective,calculationNR1) {

    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);

    objective_calculator calculator = objective_calculator();
    calculator.setData_provider_optim(provider);
    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(provider);
    input input_control = input();
    input_control.setData_provider_optim(provider);

    std::map<std::string, std::string> paths = provider.getPaths();
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF(ntimesteps_gp);


#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        forwardParticles[o-1] = input::readParticleVector("./data/vstrap_test_output/plasma_state_batch_1_forward_particles_CPU_"+std::to_string(o)+".csv",",");
    }

    pdf_control.assemblingMultiDim_parallel(forwardParticles,0,forwardPDF);
    arma::mat control(64,64,arma::fill::zeros);

    try {
        double value = calculator.calculate_objective(forwardPDF,control);
    } catch (std::exception e)  {
        std::cout << e.what() << std::endl;
    }
    EXPECT_NO_THROW();
}
