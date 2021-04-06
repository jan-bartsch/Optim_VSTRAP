#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <vector>
#include <stdexcept>
#include <cmath>

#include "../controller/abstractcontroller.h"

class Comparator : public AbstractController
{
public:
    Comparator();

    double NormDifferenceDoublevector(std::vector<double> v1, std::vector<double> v2);
};

#endif // COMPARATOR_H
