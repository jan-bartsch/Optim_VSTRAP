#include <gtest/gtest.h>

#include "../../src/io/input.h"
#include "../../src/io/output_control_update.h"

TEST(io,controlUpdate) {
    std::string input_directory = "/home/jan/Promotion_linuxPC/Optim_VSTRAP/test/data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    std::string input_directory_control = "/home/jan/Promotion_linuxPC/Optim_VSTRAP/test/data/control_field_cells.xml";
    const char * filename_control_in = input_directory_control.c_str();

    data_provider provider = data_provider(filename);
    output_control_update out = output_control_update(filename);
    input in = input();
    in.setData_provider_optim(provider);

    double norm_difference = 1.0;

    arma::mat control_out(64,3,arma::fill::randu);

    out.writeControl_XML(control_out);
    arma::mat control_in = in.readControl(filename_control_in);

    std::cout << control_out << std::endl;
    std::cout << control_in << std::endl;

    norm_difference = arma::norm(control_in-control_out);

    ASSERT_EQ(norm_difference,0.0);
}
