
#include "basic/test_io.h"

int main() {

    /*Test I/O
         *
         */

    test_io testIO = test_io();
    testIO.test_inputController();
    testIO.test_output_ControlUpdate();

    testIO.test_readInParameters();

    std::string test_python = "python3 ../../optim-vstrap-toolset/toolset/main.py ../../optim-vstrap-toolset/tests/test_data/box_vol_regular_refined.xml Control_field.xml control_new_c.xml";
    system(&test_python[0]);

    return 0;

}
