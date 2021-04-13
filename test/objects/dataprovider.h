#include <gtest/gtest.h>

#include "../../src/objects/dataprovider.h"
#include <cmath>

#include "../../src/objects/MOTIONS.h"

TEST(dataprovider, amount_InputData) {
    bool all_clear(true);

    std::string Input_directory = "./data/Optim_input_gTest.xml";
    const char *filename = Input_directory.c_str();

    DataProvider provider = DataProvider(filename);

    ASSERT_NO_THROW(MOTIONS::InitializeMotions::Load_MOTIONS(provider));


}
