#ifndef CONTROL_VERIFICATION_H
#define CONTROL_VERIFICATION_H

#include <map>
#include <vector>

#include "../controller/abstract_validation.h"

#include "../../src/controller/equationsolvingcontroller.h"

#include "../../src/tools/innerproducts.h"

#include "../../src/io/input.h"
#include "../../src/io/outputdiagnostics.h"

#include "../../src/objects/dataprovider.h"
#include "../../src/objects/MOTIONS.h"

class control_verification : public abstract_verification {

public:
  control_verification();

  static int start_verification(int argc, char **argv);

  static std::vector<double> calculate_mean(arma::mat control);

  static std::vector<double>
  calculate_mean_doubleMatrix(std::vector<std::vector<double>> control);

  static arma::mat calculate_cross_error(arma::mat control,
                                         arma::mat barycenters,
                                         std::vector<double> &valide_vector);
};

#endif // CONTROL_VERIFICATION_H
