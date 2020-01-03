#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <fstream>
#include <stdexcept>

#include "src/objects/particle.h"


class input
{
public:
    input();

    std::vector<particle> readParticleVector(std::string filename, std::string delimiter);


};

#endif // INPUT_H
