#include "abstractcontroller.h"

AbstractController::AbstractController() { }

DataProvider AbstractController::get_DataProviderOptim() const
{
    return data_provider_optim_;
}

void AbstractController::set_DataProviderOptim(const DataProvider &value)
{
    data_provider_optim_ = value;
}
