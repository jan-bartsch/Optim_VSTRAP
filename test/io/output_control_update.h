#include <gtest/gtest.h>

#include "../../src/io/input.h"
#include "../../src/io/output_control_update.h"

TEST(io,controlUpdate) {
    std::string input_directory = "/home/jan/Promotion_linuxPC/Optim_VSTRAP/test/data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    output_control_update out = output_control_update(filename);
    input in = input();
    in.setData_provider_optim(provider);

    double norm_difference = 1.0;

    arma::mat control(64,3,arma::fill::randu);

    out.writeControl_XML(control);

    ASSERT_EQ(norm_difference,0);
}
