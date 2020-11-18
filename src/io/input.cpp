#include "input.h"

#include <iostream>

input::input() { }

unsigned int input::read_plasma_state_forward(std::vector<std::vector<particle> > &forwardParticles, std::string file_name)
{
    std::map<std::string, std::string> paths = this->getData_provider_optim().getPaths();
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;
    std::string RESULTS_DIRECTORY = paths.find("RESULTS_DIRECTORY")->second;

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        try {
            forwardParticles[o-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+RESULTS_DIRECTORY+file_name+std::to_string(o)+".csv",",");
        } catch (std::exception e) {
            logger::Warning("Iteration: " + std::to_string(o));
            logger::Warning("BUILD_DIRECTORY_OPTIM: " + BUILD_DIRECTORY_OPTIM);
            logger::Warning("RESULTS_DIRECTORY: " + RESULTS_DIRECTORY);
            throw std::invalid_argument("Could not read VSTRAP output forward");
        }

    }

    return 0;
}

unsigned int input::read_plasma_state_backward(std::vector<std::vector<particle> > &backwardParticles, std::string file_name)
{
    std::map<std::string, std::string> paths = this->getData_provider_optim().getPaths();
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;
    std::string RESULTS_DIRECTORY = paths.find("RESULTS_DIRECTORY")->second;

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        try {
            backwardParticles[ntimesteps_gp - o] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+RESULTS_DIRECTORY+file_name+std::to_string(o)+".csv",",");
        } catch (std::exception e) {
            logger::Warning("BUILD_DIRECTORY_OPTIM: " + BUILD_DIRECTORY_OPTIM);
            logger::Warning("RESULTS_DIRECTORY: " + RESULTS_DIRECTORY);
            throw std::invalid_argument("Could not read VSTRAP output backward");
        }
    }

    return 0;
}


std::vector<particle> input::readParticleVector(std::string filename, std::string delimiter)
{
    std::vector<particle> particleVector;
    int counter = 0;

    std::ifstream file(filename);

    std::string line = "";

    if( !file.is_open() ) {
        logger::Warning("File not found: " + filename);
        throw  std::runtime_error("File could not be opened");
    }


    while(std::getline(file,line) ) {

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

        if (vec.size() < 11 && counter != 0 ) {
            std::cout << "Line was: " << line << std::endl;
            throw std::length_error("Too less particle attributes for writing particle vector");
        }

        if (counter != 0) {
            particle particleTemp = particle();
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

arma::mat input::readControl(const char *filename, int pcell_gp)
{       
    arma::mat control(pcell_gp,3,arma::fill::zeros);

    TiXmlDocument inputFile(filename);
    if (!inputFile.LoadFile()) {
        std::cout << filename << std::endl;
        throw std::runtime_error("File could not be opened. Check if directory and syntax are correct!");
    }

    TiXmlElement *fieldRoot = inputFile.RootElement();
    //std::cout << parameterRoot->Value() << std::endl;
    TiXmlElement *value_control = fieldRoot->FirstChildElement("value");
    int i = 0; // for sorting the entries

    size_t pos = 0;
    std::string token;
    std::vector<std::string> vec;
    std::string delimiter = ",";
    while(value_control)
    {
        TiXmlAttribute *node_number = value_control->FirstAttribute();
        std::string name_string(node_number->Value());
        int cell_id_int = std::stoi(name_string);

        std::string coordinate_string = value_control->GetText();
        std::vector<std::string> vec;
        std::vector<double> control_value_vector(3);

        while ((pos = coordinate_string.find(delimiter)) != std::string::npos) {
            token = coordinate_string.substr(0, pos);
            vec.push_back(token);
            coordinate_string.erase(0, pos + delimiter.length());
        }
        vec.push_back(coordinate_string); //push back last element

        control_value_vector[0] = std::stod(vec[0]);
        control_value_vector[1] = std::stod(vec[1]);
        control_value_vector[2] = std::stod(vec[2]);


        control(cell_id_int-1,0) = control_value_vector[0];
        control(cell_id_int-1,1) = control_value_vector[1];
        control(cell_id_int-1,2) = control_value_vector[2];

        value_control = value_control->NextSiblingElement();
        i++;
    }

    return control;
}

std::vector<double> input::readDoubleVector(const char *filename)
{
    std::ifstream ifile(filename, std::ios::in);
    std::vector<double> out;

    //check to see that the file was opened correctly:
    if (!ifile.is_open()) {
        std::cout << filename << std::endl;
        std::cerr << "There was a problem opening the input file with name!\n";
    }

    double num = 0.0;
    //keep storing values from the text file so long as data exists:
    while (ifile >> num) {
        out.push_back(num);
    }

    std::cout << "Read in vector: " << std::endl;

    //verify that the scores were stored correctly:
    for (int i = 0; i < out.size(); ++i) {
        std::cout << out[i] << std::endl;
    }

    return out;
}

