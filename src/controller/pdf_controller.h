#ifndef PDF_CONTROLLER_H
#define PDF_CONTROLLER_H

#include <unordered_map>
#include <vector>

#include "../objects/coordinate_phase_space_time.h"
#include "../objects/particle.h"


#include "abstract_controller.h"

/**
 * @brief The pdf_controller class provides important method for assembling and smoothing
 * of probability density functions (pdfs)
 */
class pdf_controller : public abstract_controller
{
public:
    pdf_controller();

    /**
     * @brief assemblingMultiDim performs the assembling of the probability density
     * distribution from the particles vector in phase-time-space
     *
     * @param particlesTime
     * @param equationType
     * @param pdf_time
     * @return
     */
    int assemblingMultiDim(std::vector<std::vector<particle> > &particlesTime, unsigned int equationType,
                           std::vector<std::unordered_map<coordinate_phase_space_time, double> > &pdf_time);

    /**
     * @brief assemblingMultiDim_parallel performs the assembling of the probability density
     * distribution from the particles vector in phase-time-space in parallel
     *
     * @param particlesTime
     * @param equationType
     * @param pdf_time
     * @return
     */
    int assemblingMultiDim_parallel(std::vector<std::vector<particle> > &particlesTime, unsigned int equationType,
                                    std::vector<std::unordered_map<coordinate_phase_space_time, double> > &pdf_time );


    /**
     * @brief relaxating_GaussSeidel_4D relaxes/smoothes the probability distribution according to
     * denoising techniques using partial differential equations (https://ieeexplore.ieee.org/document/6175261)
     * and the the GaussSeidel algorithm
     * This is important for calculating the derivative with respect to velocity in the gradient.
     *
     * @param pdf
     * @param numberOfRelaxationSteps
     * @return
     */
    std::vector<std::vector<std::vector<std::vector<double> > > > relaxating_GaussSeidel_4D(std::vector<std::vector<std::vector<std::vector<double> > > > pdf,
                                                                                         unsigned int numberOfRelaxationSteps);

    /** currently not used*/

    double calculate_wasserstein_metric(std::vector<std::vector<particle> > dist1, std::vector<std::vector<particle> > dist2);

    double calculate_wasserstein_metric_histogramm(std::vector<std::unordered_map<coordinate_phase_space_time,double> > dist1,
                                                   std::vector<std::unordered_map<coordinate_phase_space_time,double> > dist2);



};

#endif // PDF_CONTROLLER_H
