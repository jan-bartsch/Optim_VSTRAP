#include "input.h"

#include <iostream>

input::input() { }


std::vector<particle> input::readParticleVector(std::string filename, std::string delimiter)
{
    std::vector<particle> particleVector;
    int counter = 0;

    std::ifstream file(filename);

    std::string line = "";

    if( !file.is_open() ) {
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
            if(vec.size()>=7) {
                particleTemp.setWeight(std::stod(vec[6]));
            }
            particleTemp.setTimestep(0.0);
            particleVector.push_back(particleTemp);
        }
        counter++;
    }

    return particleVector;
}

arma::mat input::readControl(const char *filename)
{
    arma::mat control(64,3,arma::fill::zeros);;

    TiXmlDocument inputFile(filename);
    if (!inputFile.LoadFile()) {
        throw std::runtime_error("File could not be opened. Check if directory"
                                 "and syntax are correct!");
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

