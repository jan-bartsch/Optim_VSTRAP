#ifndef GRADIENT_VALIDATION_H
#define GRADIENT_VALIDATION_H

#include <map>
#include <vector>

#include "../controller/abstract_validation.h"

#include "../../src/controller/equationsolvingcontroller.h"
#include "../../src/controller/optimcontroller.h"
#include "../../src/controller/pdfcontroller.h"

#include "../../src/io/input.h"
#include "../../src/io/outputdiagnostics.h"

#include "../../src/objects/dataprovider.h"
#include "../../src/objects/MOTIONS.h"

#include "../../src/optimization/gradientcalculator.h"
#include "../../src/optimization/objectivecalculator.h"

#include "../../src/tools/innerproducts.h"

class gradient_validation : public abstract_verification {
public:
  gradient_validation();

  static int landau_validation(int argc, char **argv);
};

#endif // GRADIENT_VALIDATION_H
