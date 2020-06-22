#include <gtest/gtest.h>

#include "../../src/io/input.h"
#include "../../src/logger/logger.h"

TEST(io, readParticlesLengthError) {
    bool expected_exception(false);
    input inputController = input();

    try {
        std::vector<particle> particlesInput = inputController.readParticleVector("./data/test_particle_reader_wrong.csv",",");
    } catch (std::length_error e) {
        logger::Info(e.what());
        expected_exception = true;
    }
    ASSERT_TRUE(expected_exception);
}

TEST(io, readParticleCellId) {
    bool all_correct(true);
    input inputController = input();

    std::vector<particle> particlesInput = inputController.readParticleVector("./data/test_particle_reader.csv",",");
    std::vector<particle> desiredParticles(5);

    desiredParticles[0] = particle(0.16482,-0.0726986,-0.0112297,-9.74732,-83.9902,-58.6333,42);
    desiredParticles[1] = particle(0.357986,0.0713176,0.0595182,-23.8611,-24.1882,-45.0728,55);
    desiredParticles[2] = particle(0.0721225,0.256823,0.0435328,14.0792,66.3771,-38.3898,35);
    desiredParticles[3] = particle(-0.0221536,0.183199,-0.0510191,31.0885,-35.2915,111.895,22);
    desiredParticles[4] = particle(-0.206877,-0.0417323,-0.0708388,5.94053,-134.414,-141.23,26);

    for(unsigned int i = 0; i<particlesInput.size(); i++) {
        if (!(particlesInput[i] == desiredParticles[i])) {
            all_correct = false;
        }
    }

    ASSERT_TRUE(all_correct);
}
