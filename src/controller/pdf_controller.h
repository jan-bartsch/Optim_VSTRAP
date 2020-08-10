#ifndef PDF_CONTROLLER_H
#define PDF_CONTROLLER_H

#include <unordered_map>
#include <vector>

#include "../objects/coordinate_phase_space_time.h"
#include "../objects/particle.h"


#include "abstract_controller.h"

class pdf_controller : public abstract_controller
{
public:
    pdf_controller();

    int assemblingMultiDim(std::vector<std::vector<particle> > &particlesTime, unsigned int equationType,
                           std::unordered_map<coordinate_phase_space_time, double> &pdf);

    int assemblingMultiDim_parallel(std::vector<std::vector<particle> > &particlesTime, unsigned int equationType,
                                    std::vector<std::unordered_map<coordinate_phase_space_time, double> > &pdf_time );


    std::vector<std::vector<std::vector<std::vector<double>>>> relaxating_GaussSeidel_4D(std::vector<std::vector<std::vector<std::vector<double>>>> pdf,
                                                                                         unsigned int numberOfRelaxationSteps);

    double calculate_wasserstein_metric(std::vector<std::vector<particle>> dist1, std::vector<std::vector<particle>> dist2);

    double calculate_wasserstein_metric_histogramm(std::vector<std::unordered_map<coordinate_phase_space_time,double>> dist1,
                                                   std::vector<std::unordered_map<coordinate_phase_space_time,double>> dist2);



};

#endif // PDF_CONTROLLER_H
