#ifndef PdfController_H
#define PdfController_H

#include <unordered_map>
#include <vector>

#include "../objects/coordinatephasespacetime.h"
#include "../objects/particle.h"

#include "abstractcontroller.h"

using uint = unsigned int;

/**
 * @brief The PdfController class provides important method for assembling and smoothing
 * of probability density functions (pdfs)
 */
class PdfController : public AbstractController
{
public:
    PdfController();

    /**
     * @brief assemblingMultiDim performs the assembling of the probability density
     * distribution from the particles vector in phase-time-space
     *
     * @param particles_time
     * @param equation_type
     * @param pdf_time
     * @return
     */
    int AssemblingMultiDim(std::vector<std::vector<Particle> > &particles_time, unsigned int equation_type,
                           std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double> > &pdf_time);

    /**
     * @brief AssemblingMultidimParallel performs the assembling of the probability density
     * distribution from the particles vector in phase-time-space in parallel
     *
     * @param particles_time
     * @param equation_type
     * @param pdf_time
     * @return
     */
    int AssemblingMultidimParallel(std::vector<std::vector<Particle> > &particles_time, unsigned int equation_type,
                                    std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double> > &pdf_time );


    /**
     * @brief relaxating_GaussSeidel_4D relaxes/smoothes the probability distribution according to
     * denoising techniques using partial differential equations (https://ieeexplore.ieee.org/document/6175261)
     * and the the GaussSeidel algorithm
     * This is important for calculating the derivative with respect to velocity in the gradient.
     *
     * @param pdf
     * @param number_relaxation_steps
     * @return
     */
    std::vector<std::vector<std::vector<std::vector<double> > > > RelaxatingGaussseidel4D(std::vector<std::vector<std::vector<std::vector<double> > > > pdf,
                                                                                         unsigned int number_relaxation_steps);

    /** currently not used*/

    [[deprecated]]
    double CalculateWassersteinMetric(std::vector<std::vector<Particle> > dist1, std::vector<std::vector<Particle> > dist2);

    [[deprecated]]
    double CalculateWassersteinMetricHistogramm(std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double> > dist1,
                                                   std::vector<std::unordered_map<CoordinatePhaseSpaceTime,double> > dist2);



};

#endif // PdfController_H
