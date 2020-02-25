#ifndef OUTPUT_DIAGNOSTICS_H
#define OUTPUT_DIAGNOSTICS_H

#include <armadillo>

#include <vector>
#include <fstream>




class output_diagnostics
{
public:
    output_diagnostics();

    template<typename T> int writeVectorToFile(std::vector<T> input, std::string filename);

    int writeGradientToFile(arma::mat gradient, std::string filename);

    int writeDoubleToFile(double value, std::string filename);
};

#endif // OUTPUT_DIAGNOSTICS_H
