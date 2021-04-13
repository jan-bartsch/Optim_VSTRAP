#include <gtest/gtest.h>

#include "../../src/controller/pdfcontroller.h"
#include "../../src/io/input.h"
#include "../../src/logger/logger.h"

#include "../../src/objects/MOTIONS.h"

TEST(pdf, assembling) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();
  DataProvider provider = DataProvider(Input_xml_path);
   auto shared_input_data = std::make_shared<MOTIONS::InputData>(MOTIONS::InitializeMotions::Load_MOTIONS(provider));


  bool all_checked(true);

  PdfController pdf_control = PdfController(shared_input_data);

  unsigned int ntimesteps_gp = shared_input_data->ntimesteps_gp;

  std::vector<std::vector<Particle>> forwardParticles(ntimesteps_gp);
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
      forwardPDF_1;
  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>>
      forwardPDF_2;

  std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> pdf_time(
      ntimesteps_gp);

#pragma omp parallel for
  for (unsigned int o = 1; o <= ntimesteps_gp; o++) {
    forwardParticles[o - 1] = Input::ReadParticleVector(
        "./data/vstrap_test_output/"
        "plasma_state_batch_1_forward_particles_CPU_" +
            std::to_string(o) + ".csv",
        ",");
  }

 int assembling_flag_parallel =
      pdf_control.AssemblingMultidimParallel(forwardParticles, 0, pdf_time);
  forwardPDF_1 = pdf_time;
  int assembling_flag_notparallel =
      pdf_control.AssemblingMultiDim(forwardParticles, 0, pdf_time);
  forwardPDF_2 = pdf_time;

  if (assembling_flag_notparallel != 0 || assembling_flag_parallel != 0) {
      all_checked = false;
  }

  for (unsigned int i = 0; i < forwardPDF_1.size(); i++) {
    if (forwardPDF_1[i] != forwardPDF_2[i]) {
      all_checked = false;
    }
  }

  ASSERT_TRUE(all_checked);
}
