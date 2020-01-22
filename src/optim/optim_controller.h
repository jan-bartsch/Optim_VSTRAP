#ifndef OPTIM_CONTROLLER_H
#define OPTIM_CONTROLLER_H

#include <map>
#include <string>
#include <exception>

#include "src/io/tinyXML/tinyxml.h"



class optim_controller
{
public:
    optim_controller();

    int read_parameters();

};

#endif // OPTIM_CONTROLLER_H
