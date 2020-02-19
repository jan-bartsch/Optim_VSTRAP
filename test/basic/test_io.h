#ifndef TEST_IO_H
#define TEST_IO_H

#include <cmath>
#include <vector>
#include <map>
#include <random>


#include "../../src/io/input.h"
#include "../../src/io/output_control_update.h"
#include "../../src/controller/initializer.h"

class test_io
{
public:
    test_io();

    int test_inputController();

    int test_output_ControlUpdate();

    int test_readInParameters();
};

#endif // TEST_IO_H
