#ifndef GRADIENT_VALIDATION_H
#define GRADIENT_VALIDATION_H

#include <map>
#include <vector>

#include "../controller/abstract_validation.h"

#include "../../src/objects/data_provider.h"

#include "../../src/io/input.h"

class gradient_validation : public abstract_validation
{
public:
    gradient_validation();

    int landau_validation(int argc, char **argv);
};

#endif // GRADIENT_VALIDATION_H
