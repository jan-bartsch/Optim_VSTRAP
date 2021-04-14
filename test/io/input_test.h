#include <gtest/gtest.h>

#include "../../src/io/input.h"
#include "../../src/logger/logger.h"

#include "../../src/objects/MOTIONS.h"

TEST(io, readParticlesLengthError) {
  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);

  auto shared_input_data = std::make_shared<MOTIONS::InputData>(
      MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  bool expected_exception(false);
  Input InputController = Input(shared_input_data);

  try {
    std::vector<Particle> particlesInput = InputController.ReadParticleVector(
        "./data/test_particle_reader_wrong.csv", ",");
  } catch (std::length_error e) {
    logger::Info(e.what());
    expected_exception = true;
  }
  ASSERT_TRUE(expected_exception);
}

TEST(io, readParticleCellId) {
  bool all_correct(true);

  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);

  auto shared_input_data = std::make_shared<MOTIONS::InputData>(
      MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  Input InputController = Input(shared_input_data);

  std::vector<Particle> particlesInput = InputController.ReadParticleVector(
      "./data/test_particle_reader.csv", ",");
  std::vector<Particle> desiredParticles(5);

  desiredParticles[0] = Particle(0.16482, -0.0726986, -0.0112297, -9.74732,
                                 -83.9902, -58.6333, 42);
  desiredParticles[1] = Particle(0.357986, 0.0713176, 0.0595182, -23.8611,
                                 -24.1882, -45.0728, 55);
  desiredParticles[2] =
      Particle(0.0721225, 0.256823, 0.0435328, 14.0792, 66.3771, -38.3898, 35);
  desiredParticles[3] = Particle(-0.0221536, 0.183199, -0.0510191, 31.0885,
                                 -35.2915, 111.895, 22);
  desiredParticles[4] = Particle(-0.206877, -0.0417323, -0.0708388, 5.94053,
                                 -134.414, -141.23, 26);

  for (unsigned int i = 0; i < particlesInput.size(); i++) {
    if (!(particlesInput[i] == desiredParticles[i])) {
      all_correct = false;
    }
  }

  ASSERT_TRUE(all_correct);
}
