#ifndef EQUATION_SOLVING_CONTROLLER_H
#define EQUATION_SOLVING_CONTROLLER_H

#include "abstract_controller.h"


class equation_solving_controller : abstract_controller
{
public:
    equation_solving_controller();

    int start_solving_forward(std::string start_forward);

    int start_solving_backward(std::string start_backward);
};

#endif // EQUATION_SOLVING_CONTROLLER_H
