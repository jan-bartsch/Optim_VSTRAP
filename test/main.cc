#include <gtest/gtest.h>

#include <armadillo>

#include "controller/desiredtrajectorycontroller.h"
#include "controller/equationsolvingcontroller.h"
#include "controller/optimcontroller.h"
#include "controller/pdfcontroller.h"

#include "io/input_test.h"
#include "io/outputcontrolupdate.h"
#include "io/outputdiagnostics.h"

#include "objects/coordinatephasespacetime.h"
#include "objects/dataprovider.h"
#include "objects/particle.h"

#include "optimization/gradientcalculator.h"
#include "optimization/objectivecalculator.h"
#include "optimization/stepdirectioncontroller.h"
#include "optimization/stepsizecontroller.h"

#include "tools/calculus.h"
#include "tools/innerproducts.h"
#include "tools/parametersanity.h"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  //testing::GTEST_FLAG(filter) = "objective.calculationNR1";
   //testing::GTEST_FLAG(filter) = "stepdirection.armijoLinesearchNoThrow";
  // testing::GTEST_FLAG(filter) = "traj.brockett";
  //    testing::GTEST_FLAG(filter) = "io.controlUpdate";
  return RUN_ALL_TESTS();
}
