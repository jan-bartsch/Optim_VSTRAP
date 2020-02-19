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
            particleTemp.setCell_id(std::stoi(vec[10]));
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

