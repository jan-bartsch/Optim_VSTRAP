#ifndef OUTPUT_DIAGNOSTICS_H
#define OUTPUT_DIAGNOSTICS_H

#include <armadillo>

#include <fstream>
#include <vector>

#include "../controller/abstractcontroller.h"

/**
 * @brief The output_diagnostics class writes the value of different objects to
 * txt files
 */
class OutputDiagnostics : public AbstractController {
public:
  OutputDiagnostics(std::shared_ptr<MOTIONS::InputData> &input_data);

  int writeGradientMatrixToFile(arma::mat gradient, std::string filename);

  int WriteArmaMatrixToFile(arma::mat Input, std::string filename);

  int WriteDoubleToFile(double value, std::string filename);

  int WriteDoubleVectorToFile(std::vector<double> vector, std::string filename);
};

#endif // OUTPUT_DIAGNOSTICS_H
