#ifndef GRADIENT_CALCULATOR_H
#define GRADIENT_CALCULATOR_H

#include <vector>
#include <map>

#include "../objects/particle.h"
#include "../objects/data_provider.h"
#include "../objects/coordinate_phase_space_time.h"

#include "../controller/abstract_controller.h"
#include "../controller/pdf_controller.h"
#include "../controller/equation_solving_controller.h"
#include "../io/output_control_update.h"
#include "../io/output_diagnostics.h"


/**
 * @brief The gradient_calculator class provides method for assembling to gradient,
 * which is used in the calculation of the new step-direction for controls in H^2
 * Sobolev-space
 * For a reference, see https://www.tandfonline.com/doi/abs/10.1080/23324309.2021.1896552
 */
class gradient_calculator : public abstract_controller
{
public:
    gradient_calculator(const char * filename);

    /**
     * @brief calculateGradient_forceControl_space_Hm_not_parallel calculates the gradient without using any parallelization;
     *
     * @param forwardPDF_time
     * @param backwardPDF_time
     * @param control
     * @return
     */
    arma::mat calculateGradient_forceControl_space_Hm_not_parallel(std::vector<std::unordered_map<coordinate_phase_space_time, double>> forwardPDF_time,
                                                                   std::vector<std::unordered_map<coordinate_phase_space_time, double>> backwardPDF_time,
                                                                   arma::mat control);

    /**
     * @brief calculateGradient_forceControl_space_Hm calculates the gradient with parallelization
     *
     * @param forwardPDF_time
     * @param backwardPDF_time
     * @param control
     * @return
     */
    arma::mat calculateGradient_forceControl_space_Hm(std::vector<std::unordered_map<coordinate_phase_space_time, double>> forwardPDF_time,
                                                      std::vector<std::unordered_map<coordinate_phase_space_time, double>> backwardPDF_time,
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
    arma::mat calculateGradient_forceControl_space_Hm_plasma(std::vector<std::unordered_map<coordinate_phase_space_time, double>> forwardPDF_time,
                                                             std::vector<std::unordered_map<coordinate_phase_space_time, double>> backwardPDF_time,
                                                             std::vector<std::unordered_map<coordinate_phase_space_time, double>> forwardPDF_time_electrons,
                                                             std::vector<std::unordered_map<coordinate_phase_space_time, double>> backwardPDF_time_electrons,
                                                             arma::mat control);


};

#endif // GRADIENT_CALCULATOR_H
