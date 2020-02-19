#include "abstract_controller.h"

abstract_controller::abstract_controller()
{

}

data_provider abstract_controller::getData_provider_optim() const
{
    return data_provider_optim;
}

void abstract_controller::setData_provider_optim(const data_provider &value)
{
    data_provider_optim = value;
}
