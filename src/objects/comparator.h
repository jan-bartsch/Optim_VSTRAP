#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <vector>
#include <stdexcept>
#include <cmath>

#include "../controller/abstract_controller.h"

class comparator : public abstract_controller
{
public:
    comparator();

    double norm_difference_doubleVector(std::vector<double> v1, std::vector<double> v2);
};

#endif // COMPARATOR_H
