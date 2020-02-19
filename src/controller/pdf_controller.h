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


    std::unordered_map<coordinate_phase_space_time,double> assemblingMultiDim(std::vector<std::vector<particle> > &particlesTime, unsigned int equationType);

};

#endif // PDF_CONTROLLER_H
