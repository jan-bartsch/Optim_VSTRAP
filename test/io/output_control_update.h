#include <gtest/gtest.h>

#include "../../src/io/input.h"
#include "../../src/io/output_control_update.h"

TEST(io,controlUpdate) {
    bool all_clear(false);
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    std::string input_directory_control = "./data/control_field_cells.xml";
    const char * filename_control_in = input_directory_control.c_str();

    data_provider provider = data_provider(filename);
    output_control_update out = output_control_update(filename);
    input in = input();
    in.setData_provider_optim(provider);

    double norm_difference = 1.0;

    arma::mat control_out(64,3,arma::fill::randu);

    out.writeControl_XML(control_out);
    arma::mat control_in = in.readControl(filename_control_in);

    norm_difference = arma::norm(control_in-control_out,"inf");

    if(norm_difference < pow(10,-5)) {
        all_clear = true;
    }

    ASSERT_TRUE(all_clear);
}

