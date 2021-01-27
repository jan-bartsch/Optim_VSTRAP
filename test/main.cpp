#include <gtest/gtest.h>

#include <armadillo>

#include "controller/pdf_controller.h"
#include "controller/optim_controller.h"
#include "controller/equation_solving_controller.h"

#include "io/input_test.h"
#include "io/output_control_update.h"
#include "io/output_diagnostics.h"

#include "objects/coordinate_phase_space_time.h"
#include "objects/data_provider.h"
#include "objects/particle.h"

#include "optimization/gradient_calculator.h"
#include "optimization/objective_calculator.h"
#include "optimization/stepdirection_controller.h"
#include "optimization/stepsize_controller.h"

#include "tools/inner_products.h"
#include "tools/calculus.h"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    //testing::GTEST_FLAG(filter) = "optContr.runIterationMaxDepth";
    //testing::GTEST_FLAG(filter) = "gradient.calculationNR1";
    return RUN_ALL_TESTS();
}
