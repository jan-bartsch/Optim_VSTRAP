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

class gradient_calculator : public abstract_controller
{
public:
    gradient_calculator(const char * filename);

    arma::mat calculateGradient_forceControl_space_Hm_not_parallel(std::vector<std::unordered_map<coordinate_phase_space_time, double>> forwardPDF_time,
                                                                   std::vector<std::unordered_map<coordinate_phase_space_time, double>> backwardPDF_time,
                                                                   arma::mat control);

    arma::mat calculateGradient_forceControl_space_Hm(std::vector<std::unordered_map<coordinate_phase_space_time, double>> forwardPDF_time,
                                                      std::vector<std::unordered_map<coordinate_phase_space_time, double>> backwardPDF_time,
                                                      arma::mat control);

    arma::mat calculateGradient_forceControl_space_Hm_plasma(std::vector<std::unordered_map<coordinate_phase_space_time, double>> forwardPDF_time,
                                                             std::vector<std::unordered_map<coordinate_phase_space_time, double>> backwardPDF_time,
                                                             std::vector<std::unordered_map<coordinate_phase_space_time, double>> forwardPDF_time_electrons,
                                                             std::vector<std::unordered_map<coordinate_phase_space_time, double>> backwardPDF_time_electrons,
                                                             arma::mat control);


};

#endif // GRADIENT_CALCULATOR_H
