#include <gtest/gtest.h>

#include "../../src/controller/optim_controller.h"
#include "../../src/objects/data_provider.h"

TEST(optimContr,startup) {
    optim_controller contr = optim_controller();

    int argc = 3;
    const char **argv;
    argv[0] = "/home/jan/Promotion_linuxPC/build-Optim/src/Optim_VSTRAP_CMAKE";
    argv[1] = "../../Optim_VSTRAP/data/box_shifting_tests/Optim_input.xml";
    argv[2] = "too much";

    ASSERT_ANY_THROW(contr.start_optimizer(argc,argv));
}

TEST(optContr,interpolateControl) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    optim_controller contr = optim_controller();

    ASSERT_NO_THROW(contr.interpolate_control(provider));

}
