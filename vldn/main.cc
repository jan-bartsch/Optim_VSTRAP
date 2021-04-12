#include <armadillo>

#include "../src/objects/dataprovider.h"

#include "control/control_validation.h"
#include "gradient/gradient_validation.h"

int main(int argc, char **argv) {

  DataProvider verification_provider = DataProvider(argv[1]);
  std::map<std::string, std::string> verification_routines =
      verification_provider.getSubroutines();

  if (verification_routines.find("gradient_landau_verification")
          ->second.compare("TRUE") == 0) {
    gradient_validation::landau_validation(argc, argv);
  }

  if (verification_routines.find("control_verification")
          ->second.compare("TRUE") == 0) {
    control_verification::start_verification(argc, argv);
  }

  return 0;
}
