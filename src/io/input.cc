#include "input.h"

#include <iostream>

Input::Input(std::shared_ptr<MOTIONS::InputData> &input_data) {
  this->setInput_data(input_data);
}

unsigned int Input::ReadPlasmaStateForward(
    std::vector<std::vector<Particle>> &forward_particles,
    std::string file_name) {

  std::string build_directory_optim = input_data_->build_directory_optim;
  std::string results_directory = input_data_->results_directory;

  unsigned int ntimesteps_gp = input_data_->ntimesteps_gp;
  unsigned int plasma_state_output_interval =
      input_data_->plasma_state_output_interval;

#pragma omp parallel for
  for (unsigned int o = 1; o <= ntimesteps_gp; o++) {
    try {
      forward_particles[o - 1] = Input::ReadParticleVector(
          build_directory_optim + results_directory + file_name +
              std::to_string(o * plasma_state_output_interval) + ".csv",
          ",");
    } catch (std::exception e) {
      logger::Warning("Iteration: " + std::to_string(o));
      logger::Warning("BUILD_DIRECTORY_OPTIM: " + build_directory_optim);
      logger::Warning("RESULTS_DIRECTORY: " + results_directory);
      throw std::invalid_argument("Could not read VSTRAP output forward");
    }
  }

  return 0;
}

unsigned int Input::ReadPlasmaStateBackward(
    std::vector<std::vector<Particle>> &backward_particles,
    std::string file_name) {

  std::string build_directory_optim = input_data_->build_directory_optim;
  std::string results_directory = input_data_->results_directory;

  unsigned int ntimesteps_gp = input_data_->ntimesteps_gp;
  unsigned int plasma_state_output_interval =
      input_data_->plasma_state_output_interval;

#pragma omp parallel for
  for (unsigned int o = 1; o <= ntimesteps_gp; o++) {
    try {
      backward_particles[ntimesteps_gp - o] = Input::ReadParticleVector(
          build_directory_optim + results_directory + file_name +
              std::to_string(o * plasma_state_output_interval) + ".csv",
          ",");
    } catch (std::exception e) {
      logger::Warning("BUILD_DIRECTORY_OPTIM: " + build_directory_optim);
      logger::Warning("RESULTS_DIRECTORY: " + results_directory);
      throw std::invalid_argument("Could not read VSTRAP output backward");
    }
  }

  return 0;
}

std::vector<Particle> Input::ReadParticleVector(std::string filename,
                                                std::string delimiter) {
  std::vector<Particle> particleVector;
  int counter = 0;

  std::ifstream file(filename);

  std::string line = "";

  if (!file.is_open()) {
    logger::Warning("File not found: " + filename);
    throw std::runtime_error("File could not be opened");
  }

  while (std::getline(file, line)) {

    // std::cout << line << std::endl;

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

    if (vec.size() < 11 && counter != 0) {
      std::cout << "Line was: " << line << std::endl;
      throw std::length_error(
          "Too less particle attributes for writing particle vector");
    }

    if (counter != 0) {
      Particle particleTemp = Particle();
      particleTemp.setPx(std::stod(vec[0]));
      particleTemp.setPy(std::stod(vec[1]));
      particleTemp.setPz(std::stod(vec[2]));
      particleTemp.setVx(std::stod(vec[3]));
      particleTemp.setVy(std::stod(vec[4]));
      particleTemp.setVz(std::stod(vec[5]));
      particleTemp.setCell_id(std::stoi(vec[11]));
      particleVector.push_back(particleTemp);
    }
    counter++;
  }

  return particleVector;
}

arma::mat Input::ReadControl(const char *filename, int number_cells_position) {
  arma::mat control(static_cast<unsigned long long>(number_cells_position), 3,
                    arma::fill::zeros);

  TiXmlDocument InputFile(filename);
  if (!InputFile.LoadFile()) {
    std::cout << filename << std::endl;
    throw std::runtime_error(
        "File could not be opened. Check if directory and syntax are correct!");
  }

  TiXmlElement *fieldRoot = InputFile.RootElement();
  // std::cout << parameterRoot->Value() << std::endl;
  TiXmlElement *value_control = fieldRoot->FirstChildElement("value");
  int i = 0; // for sorting the entries

  size_t pos = 0;
  std::string token;
  std::vector<std::string> vec;
  std::string delimiter = ",";
  while (value_control) {
    TiXmlAttribute *node_number = value_control->FirstAttribute();
    std::string name_string(node_number->Value());
    unsigned long long cell_id_int =
        static_cast<unsigned long long>(std::stol(name_string));

    std::string coordinate_string = value_control->GetText();
    std::vector<std::string> vec;
    std::vector<double> control_value_vector(3);

    while ((pos = coordinate_string.find(delimiter)) != std::string::npos) {
      token = coordinate_string.substr(0, pos);
      vec.push_back(token);
      coordinate_string.erase(0, pos + delimiter.length());
    }
    vec.push_back(coordinate_string); // push back last element

    control_value_vector[0] = std::stod(vec[0]);
    control_value_vector[1] = std::stod(vec[1]);
    control_value_vector[2] = std::stod(vec[2]);

    control(cell_id_int - 1, 0) = control_value_vector[0];
    control(cell_id_int - 1, 1) = control_value_vector[1];
    control(cell_id_int - 1, 2) = control_value_vector[2];

    value_control = value_control->NextSiblingElement();
    i++;
  }

  return control;
}

std::vector<std::vector<double>>
Input::ReadDoubleMatrix(std::string filename, int number_cells_position,
                        std::string delimiter) {
  std::vector<std::vector<double>> matrix;
  matrix.resize(number_cells_position);

  int counter = 0;

  std::ifstream file(filename);

  std::string line = "";

  if (!file.is_open()) {
    logger::Warning("File not found: " + filename);
    throw std::runtime_error("File could not be opened");
  }

  while (std::getline(file, line)) {

    // std::cout << line << std::endl;

    size_t pos = 0;
    std::string token;
    std::vector<std::string> vec;

    while ((pos = line.find(delimiter)) != std::string::npos) {
      token = line.substr(0, pos);
      vec.push_back(token);
      line.erase(0, pos + delimiter.length());
    }
    // push back last element
    vec.push_back(line);

    if (vec.size() < 3) {
      std::cout << "Line was: " << line << std::endl;
      throw std::length_error("Too less attributes for reading 3d matrix");
    }

    matrix[counter].resize(3);

    for (int i = 0; i < vec.size(); i++) {
      matrix[counter][i] = std::stod(vec[i]);
      // str(mu_x)+","+str(mu_y)+","+str(mu_z)+","+str(s_x)+","+str(s_y)+","+str(s_z)+","+str(v_x)+","+str(v_y)+","+str(v_z)+","+str(v_s_x)+","+str(v_s_y)+","+str(v_s_z)+"\n")
    }
    counter++;
  }

  return matrix;
}

std::vector<double> Input::ReadDoubleVector(const char *filename) {
  std::ifstream ifile(filename, std::ios::in);
  std::vector<double> out;

  // check to see that the file was opened correctly:
  if (!ifile.is_open()) {
    std::cout << filename << std::endl;
    std::cerr << "There was a problem opening the Input file with name!\n";
  }

  double num = 0.0;
  // keep storing values from the text file so long as data exists:
  while (ifile >> num) {
    out.push_back(num);
  }

  std::cout << "Read in vector: " << std::endl;

  // verify that the scores were stored correctly:
  for (int i = 0; i < out.size(); ++i) {
    std::cout << out[i] << std::endl;
  }

  return out;
}

std::vector<std::vector<double>> Input::ReadBrockettFile(std::string filename,
                                                         std::string delimiter,
                                                         unsigned int lines) {
  std::vector<std::vector<double>> brockettVector;
  brockettVector.resize(lines);

  unsigned int long counter = 0;

  std::ifstream file(filename);

  std::string line = "";

  if (!file.is_open()) {
    logger::Warning("File not found: " + filename);
    throw std::runtime_error("File could not be opened");
  }

  while (std::getline(file, line) && counter < lines) {

    // std::cout << line << std::endl;

    size_t pos = 0;
    std::string token;
    std::vector<std::string> vec;

    while ((pos = line.find(delimiter)) != std::string::npos) {
      token = line.substr(0, pos);
      vec.push_back(token);
      line.erase(0, pos + delimiter.length());
    }
    // push back last element
    vec.push_back(line);

    if (vec.size() < 12) {
      std::cout << "Line was: " << line << std::endl;
      throw std::length_error(
          "Too less attributes for writing brockett-vector");
    }

    brockettVector[counter].resize(12);

    for (unsigned int i = 0; i < vec.size(); i++) {
      double current_value = std::stod(vec[i]);
      brockettVector[counter][i] = current_value;
      // str(mu_x)+","+str(mu_y)+","+str(mu_z)+","+str(s_x)+","+str(s_y)+","+str(s_z)+","+str(v_x)+","+str(v_y)+","+str(v_z)+","+str(v_s_x)+","+str(v_s_y)+","+str(v_s_z)+"\n")
    }
    counter++;
  }

  return brockettVector;
}
