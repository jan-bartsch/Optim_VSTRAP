#ifndef GRADIENT_CALCULATOR_H
#define GRADIENT_CALCULATOR_H

#include <vector>
#include <map>

#include "../objects/coordinatephasespacetime.h"
#include "../objects/dataprovider.h"
#include "../objects/doublepdf.h"
#include "../objects/MOTIONS.h"
#include "../objects/particle.h"

#include "../controller/abstractcontroller.h"
#include "../controller/pdfcontroller.h"
#include "../controller/equationsolvingcontroller.h"
#include "../io/outputcontrolupdate.h"
#include "../io/outputdiagnostics.h"


/**
 * @brief The gradient_calculator class provides method for assembling to gradient,
 * which is used in the calculation of the new step-direction for controls in H^2
 * Sobolev-space
 * For a reference, see https://www.tandfonline.com/doi/abs/10.1080/23324309.2021.1896552
 */
class GradientCalculator : public AbstractController
{
public:
    GradientCalculator(const char * filename);

    /**
     * @brief calculateGradient_forceControl_space_Hm_not_parallel calculates the gradient without using any parallelization;
     *
     * @param forwardPDF_time
     * @param backwardPDF_time
     * @param control
     * @return
     */
    arma::mat CalculategradientForcecontrolSpaceHmNotParallel(std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> forwardPDF_time,
                                                                   std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> backwardPDF_time,
                                                                   arma::mat control);

    /**
     * @brief calculateGradient_forceControl_space_Hm calculates the gradient with parallelization
     *
     * @param forwardPDF_time
     * @param backwardPDF_time
     * @param control
     * @return
     */
    arma::mat CalculategradientForcecontrolSpaceHm(std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> forwardPDF_time,
                                                      std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> backwardPDF_time,
                                                      arma::mat control);

    /**
     * @brief calculateGradient_forceControl_space_Hm_plasma calculates the gradient with two different species (ions, electrons)
     * present
     *
     * @param forwardPDF_time
     * @param backwardPDF_time
     * @param forwardPDF_time_electrons
     * @param backwardPDF_time_electrons
     * @param control
     * @return
     */
    [[deprecated]]
    arma::mat CalculategradientForcecontrolSpaceHmPlasma(std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> forwardPDF_time,
                                                             std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> backwardPDF_time,
                                                             std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> forwardPDF_time_electrons,
                                                             std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> backwardPDF_time_electrons,
                                                             arma::mat control);


};

#endif // GRADIENT_CALCULATOR_H
