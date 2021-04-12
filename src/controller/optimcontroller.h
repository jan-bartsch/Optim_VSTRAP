#ifndef OPTIM_CONTROLLER_H
#define OPTIM_CONTROLLER_H

#include <exception>
#include <map>
#include <string>
#include <vector>

#include <future>

#include <iostream>

#include "equationsolvingcontroller.h"

#include "../objects/particle.h"

#include "../optimization/gradientcalculator.h"
#include "../optimization/objectivecalculator.h"
#include "../optimization/stepdirectioncontroller.h"
#include "../optimization/stepsizecontroller.h"

#include "../io/input.h"
#include "../io/outputcontrolupdate.h"
#include "../io/outputdiagnostics.h"
#include "../io/tinyXML/tinyxml.h"

#include "../logger/logger.h"
#include "../objects/dataprovider.h"

#include "../tools/calculus.h"
#include "../tools/parametersanity.h"

/**
 * @brief The optim_controller class provides methods for starting and
 * the optimizer and visualizing the results
 */
class OptimController : public AbstractController {
public:
  OptimController();

  /**
   * @brief start_optimizer reads in the command line command and starts the
   * optimizer
   *
   * @param argc
   * @param argv
   * @return
   */
  int StartOptimizer(std::shared_ptr<MOTIONS::InputData> &input_data);

  /**
   * @brief main_optimization_algorithm is the core optimization algorithm which
   * uses the paramteres defined in the Input file for the optimizer
   *
   * @param Input_xml_path
   * @return
   */
  static int MainOptimizationAlgorithm(std::shared_ptr<MOTIONS::InputData> &input_data);

  static int Initialize(std::shared_ptr<MOTIONS::InputData> &input_data, arma::mat &control);

  /**
   * @brief start_with_zero_control deletes the whole folder containing the
   * files from the last optimization run, creates a new initial guess that is
   * zero everywhere
   *
   * @param Input_xml_path
   * @return
   */
  static arma::mat StartWithZeroControl(std::shared_ptr<MOTIONS::InputData> &input_data);

  /**
   * @brief start_with_given_control delets only the txt and csv files and
   * starts with the control that is currently in the results folder
   *
   * @param Input_xml_path
   * @return
   */
  static arma::mat StartWithGivenControl(std::shared_ptr<MOTIONS::InputData> &input_data);

  /**
   * @brief generate_Input_files starts the generation of the VSTRAP Input files
   * for forward, backward and adjoint_creation
   *
   * @param Input_xml_path
   * @return
   */
  static int GenerateInputFiles(std::shared_ptr<MOTIONS::InputData> &input_data);

  /**
   * @brief post_processing_convergence starts the python post-processing method
   *
   * @param provider
   * @return
   */
  static int PostProcessingConvergence(std::shared_ptr<MOTIONS::InputData> &input_data);

  /**
   * @brief visualize_control starts the python method for visualizing the
   * control in three dimensions
   *
   * @param provider
   * @return
   */
  static int VisualizeControl(std::shared_ptr<MOTIONS::InputData> &input_data);

  /**
   * @brief paraview_plot_forward starts the python routine for displaying the
   * evolution of the solution to the forward equation using a python script
   * for paraview
   *
   * @param provider
   * @return
   */
  static int ParaviewPlotForward();
};

#endif // OPTIM_CONTROLLER_H
