#ifndef OPTIM_CONTROLLER_H
#define OPTIM_CONTROLLER_H

#include <map>
#include <vector>
#include <string>
#include <exception>

#include <future>

#include <iostream>

#include "equationsolvingcontroller.h"


#include "../objects/particle.h"

#include "../optimization/gradientcalculator.h"
#include "../optimization/objectivecalculator.h"
#include "../optimization/stepdirectioncontroller.h"
#include "../optimization/stepsizecontroller.h"

#include "../io/input.h"
#include "../io/tinyXML/tinyxml.h"
#include "../io/outputcontrolupdate.h"
#include "../io/outputdiagnostics.h"

#include "../logger/logger.h"
#include "../objects/dataprovider.h"

#include "../tools/parametersanity.h"
#include "../tools/calculus.h"

/**
 * @brief The optim_controller class provides methods for starting and
 * the optimizer and visualizing the results
 */
class OptimController : public AbstractController
{
public:
    OptimController();

    /**
     * @brief start_optimizer reads in the command line command and starts the optimizer
     *
     * @param argc
     * @param argv
     * @return
     */
    int StartOptimizer(int argc, const char** argv);

    /**
     * @brief main_optimization_algorithm is the core optimization algorithm which uses the
     * paramteres defined in the Input file for the optimizer
     *
     * @param Input_xml_path
     * @return
     */
    static int MainOptimizationAlgorithm(const char * Input_xml_path);


    //static int check_Input_py(data_provider provider, const char * filePathOptimInput);

    /**
     * @brief start_with_zero_control deletes the whole folder containing the files from
     * the last optimization run, creates a new initial guess that is zero everywhere
     *
     * @param Input_xml_path
     * @return
     */
    static arma::mat StartWithZeroControl(const char * Input_xml_path);

    /**
     * @brief start_with_given_control delets only the txt and csv files and
     * starts with the control that is currently in the results folder
     *
     * @param Input_xml_path
     * @return
     */
    static arma::mat StartWithGivenControl(const char * Input_xml_path);

    /**
     * @brief generate_Input_files starts the generation of the VSTRAP Input files for
     * forward, backward and adjoint_creation
     *
     * @param Input_xml_path
     * @return
     */
    static int GenerateInputFiles(const char * Input_xml_path);

    /**
     * @brief post_processing_convergence starts the python post-processing method
     *
     * @param provider
     * @return
     */
    static int PostProcessingConvergence(DataProvider provider);

    /**
     * @brief visualize_control starts the python method for visualizing the control
     * in three dimensions
     *
     * @param provider
     * @return
     */
    static int VisualizeControl(DataProvider provider);

    /**
     * @brief paraview_plot_forward starts the python routine for displaying the
     * evolution of the solution to the forward equation using a python script
     * for paraview
     *
     * @param provider
     * @return
     */
    static int ParaviewPlotForward(DataProvider provider);
};

#endif // OPTIM_CONTROLLER_H
