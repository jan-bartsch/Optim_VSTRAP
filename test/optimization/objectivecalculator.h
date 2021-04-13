#include <gtest/gtest.h>

#include "../../src/io/input.h"
#include "../../src/optimization/objectivecalculator.h"

#include "../../src/objects/MOTIONS.h"

TEST(objective, calculationNR1) {

  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
   auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  ObjectiveCalculator calculator = ObjectiveCalculator(shared_input_data);
  PdfController pdf_control = PdfController(shared_input_data);
  Input Input_control = Input(shared_input_data);

  unsigned int ntimesteps_gp = shared_input_data->ntimesteps_gp;

  std::vector<std::vector<Particle>> forwardParticles(ntimesteps_gp);
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> forwardPDF(
      ntimesteps_gp);

#pragma omp parallel for
  for (unsigned int o = 1; o <= ntimesteps_gp; o++) {
    forwardParticles[o - 1] = Input::ReadParticleVector(
        "./data/vstrap_test_output/"
        "plasma_state_batch_1_forward_particles_CPU_" +
            std::to_string(o) + ".csv",
        ",");
  }

  pdf_control.AssemblingMultidimParallel(forwardParticles, 0, forwardPDF);
  arma::mat control(64, 64, arma::fill::zeros);

  try {
    double value = calculator.CalculateObjective(forwardPDF, control);
  } catch (std::exception e) {
    std::cout << e.what() << std::endl;
  }
  EXPECT_NO_THROW();
}
