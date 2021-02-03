#ifndef ABSTRACT_CONTROLLER_H
#define ABSTRACT_CONTROLLER_H

#include <armadillo>
#include <exception>

#include "../objects/data_provider.h"
#include "../logger/logger.h"


/**
 * @brief The abstract_controller class
 * is inherited by all controller classes
 */
class abstract_controller
{
public:
    abstract_controller();

    data_provider getData_provider_optim() const;
    void setData_provider_optim(const data_provider &value);

private:
    data_provider data_provider_optim;
};

#endif // ABSTRACT_CONTROLLER_H
