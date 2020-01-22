#include "test_main.h"

test_main::test_main() { }

void test_main::testing()
{
    /*Test I/O
         *
         */

    test_io testIO = test_io();
    testIO.test_inputController();
    testIO.test_output_ControlUpdate();

    testIO.test_readInParameters();


}
