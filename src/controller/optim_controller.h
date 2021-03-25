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

#include "../optimization/gradient_calculator.h"
#include "../optimization/objective_calculator.h"
#include "../optimization/stepdirection_controller.h"
#include "../optimization/stepsize_controller.h"

#include "../io/input.h"
#include "../io/tinyXML/tinyxml.h"
#include "../io/output_control_update.h"
#include "../io/output_diagnostics.h"

#include "../logger/logger.h"
#include "../objects/data_provider.h"

#include "../tools/parameter_sanity.h"
#include "../tools/calculus.h"

/**
 * @brief The optim_controller class provides methods for starting and
 * the optimizer and visualizing the results
 */
class optim_controller : public abstract_controller
{
public:
    optim_controller();

    /**
     * @brief start_optimizer reads in the command line command and starts the optimizer
     *
     * @param argc
     * @param argv
     * @return
     */
    int start_optimizer(int argc, const char** argv);

    /**
     * @brief main_optimization_algorithm is the core optimization algorithm which uses the
     * paramteres defined in the input file for the optimizer
     *
     * @param input_xml_path
     * @return
     */
    static int main_optimization_algorithm(const char * input_xml_path);


    //static int check_input_py(data_provider provider, const char * filePathOptimInput);

    /**
     * @brief start_with_zero_control deletes the whole folder containing the files from
     * the last optimization run, creates a new initial guess that is zero everywhere
     *
     * @param input_xml_path
     * @return
     */
    static arma::mat start_with_zero_control(const char * input_xml_path);

    /**
     * @brief start_with_given_control delets only the txt and csv files and
     * starts with the control that is currently in the results folder
     *
     * @param input_xml_path
     * @return
     */
    static arma::mat start_with_given_control(const char * input_xml_path);

    /**
     * @brief generate_input_files starts the generation of the VSTRAP input files for
     * forward, backward and adjoint_creation
     *
     * @param input_xml_path
     * @return
     */
    static int generate_input_files(const char * input_xml_path);

    /**
     * @brief post_processing_convergence starts the python post-processing method
     *
     * @param provider
     * @return
     */
    static int post_processing_convergence(data_provider provider);

    /**
     * @brief visualize_control starts the python method for visualizing the control
     * in three dimensions
     *
     * @param provider
     * @return
     */
    static int visualize_control(data_provider provider);

    /**
     * @brief paraview_plot_forward starts the python routine for displaying the
     * evolution of the solution to the forward equation using a python script
     * for paraview
     *
     * @param provider
     * @return
     */
    static int paraview_plot_forward(data_provider provider);
};

#endif // OPTIM_CONTROLLER_H
