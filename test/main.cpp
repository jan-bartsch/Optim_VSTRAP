#include <gtest/gtest.h>

#include "controller/pdf_controller.h"
#include "controller/equation_solving_controller.h"

#include "io/input_test.h"
#include "io/output_control_update.h"

#include "objects/coordinate_phase_space_time.h"
#include "objects/data_provider.h"
#include "objects/particle.h"

#include "optimization/objective_calculator.h"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
