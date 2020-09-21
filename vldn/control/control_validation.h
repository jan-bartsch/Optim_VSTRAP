#ifndef CONTROL_VALIDATION_H
#define CONTROL_VALIDATION_H

#include "../controller/abstract_validation.h"
#include "../../src/io/input.h"

class control_validation : public abstract_validation
{
public:
    control_validation();

    static int start_validation(int argc, char **argv);
};

#endif // CONTROL_VALIDATION_H
