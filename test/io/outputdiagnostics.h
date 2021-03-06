#include <gtest/gtest.h>

#include "../../src/io/outputdiagnostics.h"
#include "../../src/objects/dataprovider.h"

#include "../../src/objects/MOTIONS.h"

TEST(diagnostics, gradientOutput) {
  bool all_clear(false);

  std::string Input_directory = "./data/Optim_input_gTest.xml";
  const char *filename = Input_directory.c_str();

  DataProvider provider = DataProvider(filename);
  auto shared_input_data = std::make_shared<MOTIONS::InputData>(
      MOTIONS::InitializeMotions::Load_MOTIONS(provider));

  std::string results = shared_input_data->results_directory;

  OutputDiagnostics out = OutputDiagnostics(shared_input_data);

  arma::mat gradient_out(64, 3, arma::fill::randu);

  std::string command_delete_files = "rm ./" + results + "testGradient.csv";
  system(&command_delete_files[0]);

  try {
    out.writeGradientMatrixToFile(gradient_out, "testGradient");
  } catch (std::exception e) {
    ASSERT_TRUE(all_clear);
  }

  arma::mat gradient_in(64, 3, arma::fill::zeros);
  int counter = 0;

  std::ifstream file("./" + results + "testGradient.csv");

  std::string line = "";
  std::string delimiter = ",";

  if (!file.is_open()) {
    throw std::runtime_error("File could not be opened");
  }

  while (std::getline(file, line)) {
    size_t pos = 0;
    std::string token;
    std::vector<std::string> vec;

    while ((pos = line.find(delimiter)) != std::string::npos && counter != 0) {
      token = line.substr(0, pos);
      vec.push_back(token);
      line.erase(0, pos + delimiter.length());
    }
    // push back last element
    vec.push_back(line);

    if (vec.size() != 3 && counter != 0) {
      std::cout << "Line was: " << line << std::endl;
      std::cout << "Wrong number of entries" << std::endl;
      ASSERT_TRUE(all_clear);
    }

    if (counter != 0) {
      gradient_in(counter - 1, 0) = std::stod(vec[0]);
      gradient_in(counter - 1, 1) = std::stod(vec[1]);
      gradient_in(counter - 1, 2) = std::stod(vec[2]);
    }
    counter++;
  }

  // std::cout << gradient_in-gradient_out << std::endl;

  if (arma::norm(gradient_in - gradient_out) < pow(10, -5), "inf") {
    all_clear = true;
  }

  ASSERT_TRUE(all_clear);
}
