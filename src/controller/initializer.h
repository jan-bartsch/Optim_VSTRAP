#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <map>
#include <string>
#include <exception>

#include <iostream>

#include "../io/tinyXML/tinyxml.h"

class initializer
{
public:
    initializer();

    static std::map<std::string, double> read_optimization_parameters(const char *filename);
    static std::map<std::string, std::string> read_input_parameters(const char *filename);



};

#endif // INITIALIZER_H
