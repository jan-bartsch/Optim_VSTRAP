#include "abstract_validation.h"

abstract_verification::abstract_verification(){ }

data_provider abstract_verification::getData_provider_validation() const
{
    return data_provider_verification;
}

void abstract_verification::setData_provider_validation(const data_provider &value)
{
    data_provider_verification = value;
}

data_provider abstract_verification::getData_provider_optim() const
{
    return data_provider_optim;
}

void abstract_verification::setData_provider_optim(const data_provider &value)
{
    data_provider_optim = value;
}
