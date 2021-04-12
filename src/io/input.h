#ifndef Input_H
#define Input_H

#include <armadillo>

#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "../io/tinyXML/tinyxml.h"
#include "../objects/particle.h"

#include "../controller/abstractcontroller.h"

class Input : public AbstractController {
public:
  Input(std::shared_ptr<MOTIONS::InputData> &input_data);

  unsigned int
  ReadPlasmaStateForward(std::vector<std::vector<Particle>> &forward_particles,
                         std::string file_name);

  unsigned int ReadPlasmaStateBackward(
      std::vector<std::vector<Particle>> &backward_particles,
      std::string file_name);

  static std::vector<Particle> ReadParticleVector(std::string filename,
                                                  std::string delimiter);

  /**
   * @brief ReadControl reads in control cells (control in volume, xml format)
   * @param filename
   * @param number_cells_position
   * @return
   */
  arma::mat ReadControl(const char *filename, int number_cells_position);

  static std::vector<std::vector<double>>
  ReadDoubleMatrix(std::string filename, int number_cells_position,
                   std::string delimiter);

  static std::vector<double> ReadDoubleVector(const char *filename);

  /**
   * @brief readBrockettFile reads file with time-dependent desired trajectory
   * of the mean
   *
   * @param filename
   * @param delimiter
   * @param lines
   * @return
   */
  static std::vector<std::vector<double>>
  ReadBrockettFile(std::string filename, std::string delimiter,
                   unsigned int lines);
};

#endif // Input_H
