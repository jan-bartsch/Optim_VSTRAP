#include <cmath>
#include <gtest/gtest.h>

#include "../../src/objects/particle.h"

TEST(Particle, constructor) {
  double px = 0.1;
  double py = -0.1;
  double pz = -0.4;

  double vx = 10.0;
  double vy = -123.3;
  double vz = -0.2;

  bool all_clear(true);

  Particle p1_vel = Particle(vx, vy, vz);
  Particle p2_vel_pos = Particle(px, py, pz, vx, vy, vz);

  if (p1_vel.getVx() != vx || p1_vel.getVy() != vy || p1_vel.getVz() != vz) {
    all_clear = false;
  }

  if (p2_vel_pos.getPx() != px || p2_vel_pos.getPy() != py ||
      p2_vel_pos.getPy() != py || p2_vel_pos.getVx() != vx ||
      p2_vel_pos.getVy() != vy || p2_vel_pos.getVz() != vz) {
    all_clear = false;
  }
  ASSERT_TRUE(all_clear);
}

TEST(Particle, magnitudeVelocity) {
  double px = 0.1;
  double py = -0.1;
  double pz = -0.4;

  double vx = 10.0;
  double vy = -123.3;
  double vz = -0.2;
  double velocityMagnitude_target = std::sqrt(vx * vx + vy * vy + vz * vz);

  bool all_clear(true);

  Particle p1_vel = Particle(vx, vy, vz);
  Particle p2_vel_pos = Particle(px, py, pz, vx, vy, vz);

  double p1_velocityMag = p1_vel.getVelocityMagnitudeParticle();
  double p2_velocityMag = p2_vel_pos.getVelocityMagnitudeParticle();

  if (p1_velocityMag != velocityMagnitude_target ||
      p2_velocityMag != velocityMagnitude_target) {
    all_clear = false;
  }

  ASSERT_TRUE(all_clear);
}
