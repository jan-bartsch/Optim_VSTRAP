#include "output_diagnostics.h"

output_diagnostics::output_diagnostics() { }

int output_diagnostics::writeGradientToFile(arma::mat gradient, std::string filename)
{
    std::ofstream outstream;
    outstream.open(filename + ".csv", std::ios_base::app); // append instead of overwrite
    outstream  << "g_x,g_y,g_z" << std::endl;

    for(unsigned int i = 0; i < gradient.n_rows; i++) {
        outstream << gradient(i,0) << "," << gradient(i,1) << "," << gradient(i,2) << std::endl;
    }

    return 0;
}

int output_diagnostics::writeDoubleToFile(double value, std::string filename)
{
    std::ofstream outstream;
    outstream.open(filename + ".txt", std::ios_base::app); // append instead of overwrite
    outstream << value << std::endl;

    return 0;
}

template <typename T>
int output_diagnostics::writeVectorToFile(std::vector<T> input, std::string filename) {

    std::ofstream outputFile(filename + ".txt");

    for(unsigned int i = 0; i<input.size; i++) {
        outputFile << input[i] << std::endl;
    }
    //TODO error messages
    return 0;
};
