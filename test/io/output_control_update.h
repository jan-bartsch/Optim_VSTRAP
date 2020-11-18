#include <gtest/gtest.h>

#include "../../src/io/input.h"
#include "../../src/io/output_control_update.h"

TEST(io,controlUpdate) {
    bool all_clear(false);
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);
    output_control_update out = output_control_update(filename);
    input in = input();
    in.setData_provider_optim(provider);

    double norm_difference = 1.0;

    arma::mat control_out(64,3,arma::fill::randu);

    out.writeControl_XML(control_out);

    std::cout << control_out << std::endl;
    std::string PATH_TO_SHARED_FILES = provider.getPaths().find("PATH_TO_SHARED_FILES")->second;
    std::string CONTROL_FIELD_CELLS_NAME =provider.getPaths().find("CONTROL_FIELD_CELLS_NAME")->second;
    std::string input_directory_control = PATH_TO_SHARED_FILES + CONTROL_FIELD_CELLS_NAME;

    int pcell_gp = static_cast<int>(provider.getOptimizationParameters().find("pcell_gp")->second);

    const char * filename_control_in = input_directory_control.c_str();

    arma::mat control_in = in.readControl(filename_control_in,pcell_gp);
    std::cout << control_in << std::endl;
    std::cout << control_in- control_out << std::endl;

    norm_difference = arma::norm(control_in-control_out,"inf");

    if(norm_difference < pow(10,-5)) {
        all_clear = true;
    }

    ASSERT_TRUE(all_clear);
}

