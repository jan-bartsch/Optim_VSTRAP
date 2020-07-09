#include <iostream>
#include <string>

#include "controller/optim_controller.h"

int main(int argc, const char** argv) {

    optim_controller driver;
    int optim_flag = driver.start_optimizer(argc, argv);

}
