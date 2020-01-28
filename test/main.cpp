
#include "basic/test_io.h"

int main() {

    /*Test I/O
         *
         */

    test_io testIO = test_io();
    testIO.test_inputController();
    testIO.test_output_ControlUpdate();

    testIO.test_readInParameters();

    return 0;

}
