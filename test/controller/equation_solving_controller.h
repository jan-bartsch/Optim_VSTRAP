#include <gtest/gtest.h>

#include "../../src/controller/equation_solving_controller.h"
#include "../../src/logger/logger.h"


TEST(solver, LaplaciansSymmetry){
    std::string input_directory = "../../Optim_VSTRAP/data/box_shifting/Optim_input.xml";
    const char *  input_xml_path = input_directory.c_str();

    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(data_provider(input_xml_path));
    arma::mat Laplacian;
    arma::mat Laplacian_Squared;
    bool all_checked(true);

    try {
        Laplacian = solver.Laplacian_3D();
    } catch (std::out_of_range e) {
        std::cout << "Error while generating Laplacian" << std::endl;
        std::cout << e.what() << std::endl;
        all_checked = false;
    }

    try {
        std::cout << "Error while generating Laplacian_Squared" << std::endl;
        Laplacian_Squared = solver.Laplacian_Squared_3D();
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
        all_checked = false;
    }

    if(!all_checked) {
        ASSERT_TRUE(all_checked);
    }

    if(!arma::approx_equal(Laplacian,Laplacian.t(),"both",pow(10,-5),pow(10,-5)) ||
            !arma::approx_equal(Laplacian_Squared,Laplacian_Squared.t(),"both",pow(10,-5),pow(10,-5))) {
        all_checked = false;
    }

    ASSERT_TRUE(all_checked);
}
