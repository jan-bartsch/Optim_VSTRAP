#ifndef OUTPUT_DIAGNOSTICS_H
#define OUTPUT_DIAGNOSTICS_H

#include <armadillo>

#include <vector>
#include <fstream>

#include "../controller/abstract_controller.h"



/**
 * @brief The output_diagnostics class writes the value of different objects to txt files
 */
class output_diagnostics : public abstract_controller
{
public:
    output_diagnostics();

    int writeGradientMatrixToFile(arma::mat gradient, std::string filename);

    int writeArmaMatrixToFile(arma::mat input, std::string filename);

    int writeDoubleToFile(double value, std::string filename);

    int writeDoubleVectorToFile(std::vector<double> vector, std::string filename);
};

#endif // OUTPUT_DIAGNOSTICS_H
