#ifndef ABSTRACT_VERIFICATION_H
#define ABSTRACT_VERIFICATION_H

#include "../../src/controller/abstractcontroller.h"

class abstract_verification {
public:
  abstract_verification();

  DataProvider getData_provider_validation() const;
  void setData_provider_validation(const DataProvider &value);

  DataProvider get_DataProviderOptim() const;
  void set_DataProviderOptim(const DataProvider &value);

private:
  DataProvider data_provider_optim;
  DataProvider data_provider_verification;
};

#endif // ABSTRACT_VERIFICATION_H
