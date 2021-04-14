#include "abstractcontroller.h"

AbstractController::AbstractController() {}

std::shared_ptr<MOTIONS::InputData> AbstractController::getInput_data() const {
  return input_data_;
}

void AbstractController::setInput_data(
    const std::shared_ptr<MOTIONS::InputData> &input_data) {
  input_data_ = input_data;
}
