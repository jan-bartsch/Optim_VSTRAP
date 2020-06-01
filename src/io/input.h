#ifndef INPUT_H
#define INPUT_H

#include <armadillo>

#include <vector>
#include <fstream>
#include <stdexcept>

#include "../objects/particle.h"
#include "../io/tinyXML/tinyxml.h"

#include "../controller/abstract_controller.h"


class input : public abstract_controller
{
public:
    input();

    unsigned int read_plasma_state_forward(std::vector<std::vector<particle>> &forwardParticles);

    unsigned int read_plasma_state_backward(std::vector<std::vector<particle>> &backwardParticles);

    static std::vector<particle> readParticleVector(std::string filename, std::string delimiter);

    static arma::mat readControl(const char *filename);
};

#endif // INPUT_H
