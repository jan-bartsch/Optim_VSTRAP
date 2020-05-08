#ifndef INPUT_H
#define INPUT_H

#include <armadillo>

#include <vector>
#include <fstream>
#include <stdexcept>

#include "../objects/particle.h"
#include "../io/tinyXML/tinyxml.h"


class input
{
public:
    input();

    static std::vector<particle> readParticleVector(std::string filename, std::string delimiter);

    static arma::mat readControl(const char *filename);


};

#endif // INPUT_H
