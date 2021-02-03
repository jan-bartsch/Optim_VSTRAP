#ifndef INPUT_H
#define INPUT_H

#include <armadillo>

#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstdlib>

#include "../objects/particle.h"
#include "../io/tinyXML/tinyxml.h"

#include "../controller/abstract_controller.h"


class input : public abstract_controller
{
public:
    input();

    unsigned int read_plasma_state_forward(std::vector<std::vector<particle>> &forwardParticles, std::string file_name);

    unsigned int read_plasma_state_backward(std::vector<std::vector<particle>> &backwardParticles, std::string file_name);

    static std::vector<particle> readParticleVector(std::string filename, std::string delimiter);

    /**
     * @brief readControl reads in control cells (control in volume, xml format)
     * @param filename
     * @param pcell_gp
     * @return
     */
    arma::mat readControl(const char *filename, int pcell_gp);

    static std::vector<std::vector<double> > readDoubleMatrix(std::string filename, int pcell_gp, std::string delimiter);

    static std::vector<double> readDoubleVector(const char *filename);

    /**
     * @brief readBrockettFile reads file with time-dependent desired trajectory of the mean
     *
     * @param filename
     * @param delimiter
     * @param lines
     * @return
     */
    static std::vector<std::vector<double> > readBrockettFile(std::string filename, std::string delimiter, unsigned int lines);
};

#endif // INPUT_H
