#ifndef OUTPUT_DIAGNOSTICS_H
#define OUTPUT_DIAGNOSTICS_H

#include <armadillo>

#include <vector>
#include <fstream>

#include "../controller/abstract_controller.h"




class output_diagnostics : public abstract_controller
{
public:
    output_diagnostics();

    int writeGradientToFile(arma::mat gradient, std::string filename);

    int writeDoubleToFile(double value, std::string filename);
};

#endif // OUTPUT_DIAGNOSTICS_H
