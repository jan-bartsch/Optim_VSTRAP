#ifndef OPTIM_CONTROLLER_H
#define OPTIM_CONTROLLER_H

#include <map>
#include <string>
#include <exception>

#include <iostream>

#include "../io/tinyXML/tinyxml.h"



class optim_controller
{
public:
    optim_controller();

    std::map<std::string,double> read_parameters(const char *filename);

};

#endif // OPTIM_CONTROLLER_H
