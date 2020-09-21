#ifndef CONTROL_VALIDATION_H
#define CONTROL_VALIDATION_H

#include <vector>
#include <map>

#include "../controller/abstract_validation.h"
#include "../../src/io/input.h"

#include "../../src/objects/data_provider.h"

class control_validation : public abstract_validation
{
public:
    control_validation();

    static int start_validation(int argc, char **argv);
};

#endif // CONTROL_VALIDATION_H
