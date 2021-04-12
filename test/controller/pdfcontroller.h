#include <gtest/gtest.h>

#include "../../src/controller/pdfcontroller.h"
#include "../../src/io/input.h"
#include "../../src/logger/logger.h"

TEST(pdf, assembling) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();
  DataProvider data_provider_opt = DataProvider(Input_xml_path);
  std::map<std::string, double> optimizationParameters =
      data_provider_opt.getOptimizationParameters();

  bool all_checked(true);

  PdfController pdf_control = PdfController();
  pdf_control.set_DataProviderOptim(data_provider_opt);
  Input Input_control = Input();
  Input_control.set_DataProviderOptim(data_provider_opt);

  unsigned int ntimesteps_gp = static_cast<unsigned int>(
      optimizationParameters.find("ntimesteps_gp")->second);

  std::vector<std::vector<Particle>> forwardParticles(ntimesteps_gp);
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
      forwardPDF_1;
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
      forwardPDF_2;

  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> pdf_time(
      ntimesteps_gp);
  int assembling_flag;

#pragma omp parallel for
  for (unsigned int o = 1; o <= ntimesteps_gp; o++) {
    forwardParticles[o - 1] = Input::ReadParticleVector(
        "./data/vstrap_test_output/"
        "plasma_state_batch_1_forward_particles_CPU_" +
            std::to_string(o) + ".csv",
        ",");
  }

  assembling_flag =
      pdf_control.AssemblingMultidimParallel(forwardParticles, 0, pdf_time);
  forwardPDF_1 = pdf_time;
  assembling_flag =
      pdf_control.AssemblingMultiDim(forwardParticles, 0, pdf_time);
  forwardPDF_2 = pdf_time;

  for (unsigned int i = 0; i < forwardPDF_1.size(); i++) {
    if (forwardPDF_1[i] != forwardPDF_2[i]) {
      all_checked = false;
    }
  }

  ASSERT_TRUE(all_checked);
}
