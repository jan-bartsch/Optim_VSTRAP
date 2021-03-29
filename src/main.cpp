#include <iostream>
#include <string>

#include "controller/optimcontroller.h"

int main(int argc, const char** argv) {

    OptimController driver;
    int optim_flag = driver.StartOptimizer(argc, argv);

}
