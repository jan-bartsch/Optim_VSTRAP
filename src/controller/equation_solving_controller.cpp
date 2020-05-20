#include "equation_solving_controller.h"

equation_solving_controller::equation_solving_controller()
{

}

int equation_solving_controller::start_solving_forward(std::string start_forward)
{
    return system(&start_forward[0]);
}

int equation_solving_controller::start_solving_backward(std::string start_backward)
{
    return  system(&start_backward[0]);
}


