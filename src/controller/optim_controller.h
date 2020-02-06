#ifndef OPTIM_CONTROLLER_H
#define OPTIM_CONTROLLER_H

#include <map>
#include <vector>
#include <string>
#include <exception>

#include <iostream>

#include "../io/tinyXML/tinyxml.h"
#include "../objects/particle.h"
#include "../logger/logger.h"

#include "../io/input.h"

#include "initializer.h"


class optim_controller
{
public:
    optim_controller();

    static void start_optimizer();
    static int start_optimization_iteration(std::vector<double> &control );


    std::map<std::string, double> getOPTIMIZATION_PARAMETERS() const;
    void setOPTIMIZATION_PARAMETERS(const std::map<std::string, double> &value);

private:
    std::map<std::string,double> OPTIMIZATION_PARAMETERS;

};

#endif // OPTIM_CONTROLLER_H
