#ifndef ABSTRACT_CONTROLLER_H
#define ABSTRACT_CONTROLLER_H

#include <armadillo>
#include <exception>

#include "../objects/dataprovider.h"
#include "../objects/MOTIONS.h"

#include "../logger/logger.h"


/**
 * @brief The abstract_controller class
 * is inherited by all controller classes
 */
class AbstractController
{
public:
    AbstractController();

    DataProvider get_DataProviderOptim() const;
    void set_DataProviderOptim(const DataProvider &value);

private:
    DataProvider data_provider_optim_;
};

#endif // ABSTRACT_CONTROLLER_H
