#include <armadillo>

#include "../src/objects/data_provider.h"

#include "control/control_validation.h"
#include "gradient/gradient_validation.h"

int main(int argc, char **argv) {

    data_provider validation_provider = data_provider(argv[1]);
    std::map<std::string,std::string> validation_routines = validation_provider.getSubroutines();

    if (validation_routines.find("gradient_landau_validation")->second.compare("TRUE")==0) {
        gradient_validation::landau_validation(argc,argv);
    }

    if (validation_routines.find("control_validation")->second.compare("TRUE")==0) {
        control_validation::start_validation(argc,argv);
    }

    return 0;
}
