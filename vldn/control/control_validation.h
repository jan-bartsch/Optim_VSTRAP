#ifndef CONTROL_VERIFICATION_H
#define CONTROL_VERIFICATION_H

#include <vector>
#include <map>

#include "../controller/abstract_validation.h"

#include "../../src/controller/equation_solving_controller.h"

#include "../../src/tools/inner_products.h"

#include "../../src/io/input.h"
#include "../../src/io/output_diagnostics.h"

#include "../../src/objects/data_provider.h"



class control_verification : public abstract_verification
{

public:
    control_verification();

    static int start_verification(int argc, char **argv);

    static double calculate_mean(arma::mat control);

    static std::vector<double> calculate_mean_doubleMatrix(std::vector<std::vector<double>> control);

    static arma::mat calculate_cross_error(arma::mat control, arma::mat barycenters, std::vector<double> &valide_vector);
};

#endif // CONTROL_VERIFICATION_H
