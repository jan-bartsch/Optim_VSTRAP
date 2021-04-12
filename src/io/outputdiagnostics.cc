#include "outputdiagnostics.h"

OutputDiagnostics::OutputDiagnostics(std::shared_ptr<MOTIONS::InputData> &input_data) {
    this->setInput_data(input_data);
}

int OutputDiagnostics::writeGradientMatrixToFile(arma::mat gradient,
                                                 std::string filename) {
  std::map<std::string, std::string> paths =
      this->get_DataProviderOptim().getPaths();
  std::string RESULTS_DIRECTORY = paths.find("RESULTS_DIRECTORY")->second;

  std::ofstream outstream;
  outstream.open(RESULTS_DIRECTORY + filename + ".csv",
                 std::ios_base::app); // append instead of overwrite
  // outstream  << "g_x,g_y,g_z" << std::endl;

  for (unsigned int i = 0; i < gradient.n_rows; i++) {
    outstream << gradient(i, 0) << "," << gradient(i, 1) << ","
              << gradient(i, 2) << std::endl;
  }

  return 0;
}

int OutputDiagnostics::WriteArmaMatrixToFile(arma::mat Input,
                                             std::string filename) {

  std::ofstream outputFile(filename + ".txt");

  for (unsigned int j = 0; j < Input.n_rows; j++) {
    for (unsigned int k = 0; k < Input.n_cols; k++) {
      outputFile << Input(j, k) << " ";
    }
    outputFile << std::endl;
  }

  return 0;
}

int OutputDiagnostics::WriteDoubleToFile(double value, std::string filename) {
  std::ofstream outstream;
  outstream.open(MOTIONS::InputData::results_directory + filename + ".txt",
                 std::ios_base::app); // append instead of overwrite
  outstream << value << std::endl;

  return 0;
}

int OutputDiagnostics::WriteDoubleVectorToFile(std::vector<double> vector,
                                               std::string filename) {
  std::ofstream outstream;
  outstream.open(MOTIONS::InputData::results_directory + filename + ".txt",
                 std::ios_base::app); // append instead of overwrite
  for (unsigned int i = 0; i < vector.size(); i++) {
    outstream << vector[i] << std::endl;
  }

  return 0;
}
