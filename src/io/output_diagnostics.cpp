#include "output_diagnostics.h"

output_diagnostics::output_diagnostics() { }

int output_diagnostics::writeGradientToFile(arma::mat gradient, std::string filename)
{
    std::map<std::string, std::string> paths = this->getData_provider_optim().getPaths();
    std::string RESULTS_DIRECTORY = paths.find("RESULTS_DIRECTORY")->second;

    std::ofstream outstream;
    outstream.open(RESULTS_DIRECTORY + filename + ".csv", std::ios_base::app); // append instead of overwrite
    outstream  << "g_x,g_y,g_z" << std::endl;

    for(unsigned int i = 0; i < gradient.n_rows; i++) {
        outstream << gradient(i,0) << "," << gradient(i,1) << "," << gradient(i,2) << std::endl;
    }

    return 0;
}

int output_diagnostics::writeDoubleToFile(double value, std::string filename)
{

    std::map<std::string, std::string> paths = this->getData_provider_optim().getPaths();
    std::string RESULTS_DIRECTORY = paths.find("RESULTS_DIRECTORY")->second;

    std::ofstream outstream;
    outstream.open(RESULTS_DIRECTORY + filename + ".txt", std::ios_base::app); // append instead of overwrite
    outstream << value << std::endl;

    return 0;
}

int output_diagnostics::writeDoubleVectorToFile(std::vector<double> vector, std::string filename)
{
    std::map<std::string, std::string> paths = this->getData_provider_optim().getPaths();
    std::string RESULTS_DIRECTORY = paths.find("RESULTS_DIRECTORY")->second;

    std::ofstream outstream;
    outstream.open(RESULTS_DIRECTORY + filename + ".txt", std::ios_base::app); // append instead of overwrite
    for (unsigned int i = 0; i < vector.size(); i++) {
        outstream << vector[i] << std::endl;
    }

    return 0;
}

