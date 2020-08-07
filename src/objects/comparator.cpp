#include "comparator.h"

comparator::comparator() { }

double comparator::norm_difference_doubleVector(std::vector<double> v1, std::vector<double> v2)
{
    unsigned long length_v1 = v1.size();
    unsigned long length_v2 =v2.size();

    double norm = 0.0;

    if (length_v1 != length_v2) {
        throw std::invalid_argument("Vector lengths much match!");
    }

    for (unsigned long i = 0; i<length_v1; i++) {
        norm += fabs(v1[i]-v2[i]);
    }

    return norm;

}
