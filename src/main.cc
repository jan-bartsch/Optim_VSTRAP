#include <iostream>
#include <string>

#include "controller/optimcontroller.h"

#include "objects/MOTIONS.h"
#include "objects/dataprovider.h"

int main(int argc, const char **argv) {

  OptimController driver;

  // Set parameters

  DataProvider provider =
      DataProvider(MOTIONS::InitializeMotions::GenerateInputPath(argc, argv));
  MOTIONS::InputData input_data =
      MOTIONS::InitializeMotions::Load_MOTIONS(provider);
  input_data.input_path_xml =
      MOTIONS::InitializeMotions::GenerateInputPath(argc, argv);

  auto shared_input_data = std::make_shared<MOTIONS::InputData>(input_data);

  int optim_flag = driver.StartOptimizer(shared_input_data);
}
