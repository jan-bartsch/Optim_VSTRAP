#include <gtest/gtest.h>

#include <cmath>
#include "../../src/objects/data_provider.h"

TEST(dataprovider,amount_inputData) {
    bool all_clear(true);

    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);


    std::map<std::string,double> parameters = provider.read_optimization_parameters(filename);
    std::map<std::string,std::string> paths = provider.read_paths(filename);
    std::map<std::string,std::string> routines = provider.read_subroutines(filename);

    if (parameters.size() != 73) {
        std::cout << "Parameters size wrong; Expected 64 but was " + std::to_string(parameters.size()) << std::endl;
        all_clear = false;
    }
    if (paths.size() != 19) {
        std::cout << "Parameters size wrong; Expected 15 but was " + std::to_string(paths.size()) << std::endl;
        all_clear = false;
    }
    if (routines.size() != 4) {
        std::cout << "Parameters size wrong; Expected 4 but was " + std::to_string(routines.size()) << std::endl;
        all_clear = false;
    }
    ASSERT_TRUE(all_clear);
}
