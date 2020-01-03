#ifndef OUTPUT_DIAGNOSTICS_H
#define OUTPUT_DIAGNOSTICS_H


#include <vector>
#include <fstream>




class output_diagnostics
{
public:
    output_diagnostics();

    template<typename T> int writeVectorToFile(std::vector<T> input, std::string filename);
};

#endif // OUTPUT_DIAGNOSTICS_H
