#include "abstract_validation.h"

abstract_verification::abstract_verification() {}

DataProvider abstract_verification::getData_provider_validation() const {
  return data_provider_verification;
}

void abstract_verification::setData_provider_validation(
    const DataProvider &value) {
  data_provider_verification = value;
}

DataProvider abstract_verification::get_DataProviderOptim() const {
  return data_provider_optim;
}

void abstract_verification::set_DataProviderOptim(const DataProvider &value) {
  data_provider_optim = value;
}
