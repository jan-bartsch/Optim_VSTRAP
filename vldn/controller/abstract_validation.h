#ifndef ABSTRACT_VALIDATION_H
#define ABSTRACT_VALIDATION_H

#include "../../src/controller/abstract_controller.h"

class abstract_validation
{
public:
    abstract_validation();

    data_provider getData_provider_validation() const;
    void setData_provider_validation(const data_provider &value);

    data_provider getData_provider_optim() const;
    void setData_provider_optim(const data_provider &value);

private:
    data_provider data_provider_optim;
    data_provider data_provider_validation;
};

#endif // ABSTRACT_VALIDATION_H
