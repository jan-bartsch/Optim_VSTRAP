#include <gtest/gtest.h>

#include "../../src/optimization/objectivecalculator.h"
#include "../../src/io/input.h"

TEST(objective,calculationNR1) {

    std::string Input_directory = "./data/Optim_Input_gTest.xml";
    const char *  filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);

    ObjectiveCalculator calculator = ObjectiveCalculator();
    calculator.set_DataProviderOptim(provider);
    PdfController pdf_control = PdfController();
    pdf_control.set_DataProviderOptim(provider);
    Input Input_control = Input();
    Input_control.set_DataProviderOptim(provider);

    std::map<std::string, std::string> paths = provider.getPaths();
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    std::vector<std::vector<Particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double>> forwardPDF(ntimesteps_gp);


#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        forwardParticles[o-1] = Input::ReadParticleVector("./data/vstrap_test_output/plasma_state_batch_1_forward_particles_CPU_"+std::to_string(o)+".csv",",");
    }

    pdf_control.AssemblingMultidimParallel(forwardParticles,0,forwardPDF);
    arma::mat control(64,64,arma::fill::zeros);

    try {
        double value = calculator.CalculateObjective(forwardPDF,control);
    } catch (std::exception e)  {
        std::cout << e.what() << std::endl;
    }
    EXPECT_NO_THROW();
}
