#ifndef DOUBLEPDF_H
#define DOUBLEPDF_H

#include <vector>

#include "../objects/MOTIONS.h"

using uint = unsigned int;

class DoublePDF
{
public:
    DoublePDF(uint timesteps, uint vcell);

    double &at(uint o, uint l, uint m, uint n);

private:
    std::vector<std::vector<std::vector<std::vector<double>>>> pdf_;
};

#endif // DOUBLEPDF_H
