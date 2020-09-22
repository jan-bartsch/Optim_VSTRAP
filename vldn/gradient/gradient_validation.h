#ifndef GRADIENT_VALIDATION_H
#define GRADIENT_VALIDATION_H

#include <map>
#include <vector>

#include "../controller/abstract_validation.h"

#include "../../src/controller/pdf_controller.h"
#include "../../src/controller/equation_solving_controller.h"
#include "../../src/controller/optim_controller.h"

#include "../../src/io/input.h"
#include "../../src/io/output_diagnostics.h"

#include "../../src/objects/data_provider.h"

#include "../../src/optimization/gradient_calculator.h"
#include "../../src/optimization/objective_calculator.h"



class gradient_validation : public abstract_validation
{
public:
    gradient_validation();

    static int landau_validation(int argc, char **argv);
};

#endif // GRADIENT_VALIDATION_H
