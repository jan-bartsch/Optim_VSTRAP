#ifndef ABSTRACT_VERIFICATION_H
#define ABSTRACT_VERIFICATION_H

#include "../../src/controller/abstract_controller.h"

class abstract_verification
{
public:
    abstract_verification();

    data_provider getData_provider_validation() const;
    void setData_provider_validation(const data_provider &value);

    data_provider getData_provider_optim() const;
    void setData_provider_optim(const data_provider &value);

private:
    data_provider data_provider_optim;
    data_provider data_provider_verification;
};

#endif // ABSTRACT_VERIFICATION_H
