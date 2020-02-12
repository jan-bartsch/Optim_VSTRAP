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

    static void start_optimizer(int argc, const char** argv);
    static int start_optimization_iteration(std::vector<double> &control, const char * input_xml_path);


    std::map<std::string, double> getOPTIMIZATION_PARAMETERS() const;
    void setOPTIMIZATION_PARAMETERS(const std::map<std::string, double> &value);

    std::map<std::string, std::string> getINPUT_PARAMETERS() const;
    void setINPUT_PARAMETERS(const std::map<std::string, std::string> &value);

private:
    std::map<std::string,double> OPTIMIZATION_PARAMETERS;
    std::map<std::string,std::string> INPUT_PARAMETERS;

};

#endif // OPTIM_CONTROLLER_H
