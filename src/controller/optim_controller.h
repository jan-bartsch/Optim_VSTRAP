#ifndef OPTIM_CONTROLLER_H
#define OPTIM_CONTROLLER_H

#include <map>
#include <vector>
#include <string>
#include <exception>

#include <future>

#include <iostream>

#include "equation_solving_controller.h"


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

   // static void assemblePDF_thread(std::vector<std::vector<particle>> &particles, std::unordered_map<coordinate_phase_space_time,double> &particlePDF);
   static std::unordered_map<coordinate_phase_space_time,double> assemblePDF_thread(std::vector<std::vector<particle>> &particles, unsigned int equation_type, data_provider data_provider_);

   static int check_input_py(data_provider provider);
   static int interpolate_control(data_provider provider);

};

#endif // OPTIM_CONTROLLER_H
