#ifndef OPTIM_CONTROLLER_H
#define OPTIM_CONTROLLER_H

#include <map>
#include <vector>
#include <string>
#include <exception>

#include <iostream>


#include "../objects/particle.h"
#include "../objects/data_provider.h"
#include "../logger/logger.h"

#include "../optimization/gradient_calculator.h"
#include "../optimization/objective_calculator.h"
#include "../optimization/stepdirection_controller.h"
#include "../optimization/stepsize_controller.h"

#include "../io/input.h"
#include "../io/tinyXML/tinyxml.h"
#include "../io/output_control_update.h"
#include "../io/output_diagnostics.h"


class optim_controller : public abstract_controller
{
public:
    optim_controller();

    static void start_optimizer(int argc, const char** argv);
    static int start_optimization_iteration(arma::mat &control, const char * input_xml_path);

    std::shared_ptr<data_provider> getData_provider() const;
    void setData_provider(const std::shared_ptr<data_provider> &value);


};

#endif // OPTIM_CONTROLLER_H
