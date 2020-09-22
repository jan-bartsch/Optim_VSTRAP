#include <armadillo>

#include "control/control_validation.h"
#include "gradient/gradient_validation.h"

int main(int argc, char **argv) {


    gradient_validation::landau_validation(argc,argv);

    control_validation::start_validation(argc,argv);

    return 0;
}
