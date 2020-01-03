#include "output_diagnostics.h"

output_diagnostics::output_diagnostics()
{

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
