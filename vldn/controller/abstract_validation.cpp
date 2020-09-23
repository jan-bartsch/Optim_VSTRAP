#include "abstract_validation.h"

abstract_validation::abstract_validation(){ }

data_provider abstract_validation::getData_provider_validation() const
{
    return data_provider_validation;
}

void abstract_validation::setData_provider_validation(const data_provider &value)
{
    data_provider_validation = value;
}

data_provider abstract_validation::getData_provider_optim() const
{
    return data_provider_optim;
}

void abstract_validation::setData_provider_optim(const data_provider &value)
{
    data_provider_optim = value;
}
