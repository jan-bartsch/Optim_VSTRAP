#ifndef ABSTRACT_CONTROLLER_H
#define ABSTRACT_CONTROLLER_H

#include <armadillo>
#include <exception>

#include "../objects/MOTIONS.h"
#include "../objects/dataprovider.h"

#include "../logger/logger.h"

/**
 * @brief The abstract_controller class
 * is inherited by all controller classes
 */
class AbstractController {
public:
  AbstractController();

  DataProvider get_DataProviderOptim() const;
  void set_DataProviderOptim(const DataProvider &value);

  std::shared_ptr<MOTIONS::InputData> getInput_data() const;
  void setInput_data(const std::shared_ptr<MOTIONS::InputData> &input_data);

protected:
  std::shared_ptr<MOTIONS::InputData> input_data_ = nullptr;
};

#endif // ABSTRACT_CONTROLLER_H
