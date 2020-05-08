#ifndef PDF_CONTROLLER_H
#define PDF_CONTROLLER_H

#include <unordered_map>
#include <vector>

#include <tbb/tbb.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#include "../objects/coordinate_phase_space_time.h"
#include "../objects/particle.h"

#include "abstract_controller.h"

class pdf_controller : public abstract_controller
{
public:
    pdf_controller();


    std::unordered_map<coordinate_phase_space_time,double> assemblingMultiDim(std::vector<std::vector<particle> > &particlesTime, unsigned int equationType);

    std::vector<std::unordered_map<coordinate_phase_space_time,double>> assemblingMultiDim_parallel(std::vector<std::vector<particle> > &particlesTime, unsigned int equationType);


    std::vector<std::vector<std::vector<std::vector<double>>>> relaxating_GaussSeidel_4D(std::vector<std::vector<std::vector<std::vector<double>>>> pdf,
                                                                                         unsigned int numberOfRelaxationSteps);

    double calculate_wasserstein_metric(std::vector<std::vector<particle>> dist1, std::vector<std::vector<particle>> dist2);


};

#endif // PDF_CONTROLLER_H
