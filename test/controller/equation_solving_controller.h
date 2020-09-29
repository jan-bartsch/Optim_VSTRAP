#include <gtest/gtest.h>

#include "../../src/controller/optim_controller.h"
#include "../../src/controller/equation_solving_controller.h"
#include "../../src/logger/logger.h"
#include "../../src/objects/data_provider.h"
#include "../../src/io/output_control_update.h"



TEST(solver,D1forwardBackward) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  input_xml_path = input_directory.c_str();

    data_provider provider = data_provider(input_xml_path);

    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(data_provider(input_xml_path));

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);


    arma::mat D1_forward;
    arma::mat D1_backward;

    bool all_checked(true);

    try {
        D1_forward = solver.D1_forward();
        std::cout << "D1_forward:" << std::endl;
        std::cout << D1_forward << std::endl;

        D1_backward = solver.D1_backward();
        std::cout << "D1_backward:" << std::endl;
        std::cout << D1_backward << std::endl;
    } catch (std::out_of_range e) {
        std::cout << "Error while generating first derivative" << std::endl;
        std::cout << e.what() << std::endl;
        all_checked = false;
    }

    arma::mat control(dimensionOfControl_gp,3,arma::fill::ones);
    arma::mat first_derivative_forward = D1_forward*control;
    arma::mat first_derivative_backward = D1_backward*control;

    std::cout << "First derivative of constant one vector (forward)" << std::endl;
    std::cout << first_derivative_forward << std::endl;

    std::cout << "First derivative of constant one vector (backward)" << std::endl;
    std::cout << first_derivative_backward << std::endl;

    if (arma::norm(first_derivative_forward,"fro") != 0 || arma::norm(first_derivative_backward,"fro") != 0) {
        all_checked = false;
    }

    ASSERT_TRUE(all_checked);
}

TEST(solver,checkSecondDerivativeforwardbackward) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  input_xml_path = input_directory.c_str();

    data_provider provider = data_provider(input_xml_path);

    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(data_provider(input_xml_path));

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);


    arma::mat D1_forward;
    arma::mat D1_backward;

    bool all_checked(true);

    try {
        D1_forward = solver.D1_forward();
        D1_backward = solver.D1_backward();
    } catch (std::out_of_range e) {
        std::cout << "Error while generating first derivative" << std::endl;
        std::cout << e.what() << std::endl;
        all_checked = false;
    }

    arma::mat control(dimensionOfControl_gp,3,arma::fill::ones);

    arma::mat D2_fb = D1_forward*D1_backward;
    arma::mat D2_model = solver.Laplacian_3D();

    if (arma::norm(D2_fb - D2_model,"fro") != 0) {
        all_checked = false;
    }

    EXPECT_TRUE(all_checked);
}

TEST(solver,checksecondDerivativeCalculation) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  input_xml_path = input_directory.c_str();

    data_provider provider = data_provider(input_xml_path);

    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(data_provider(input_xml_path));

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    double db_gp = static_cast<double>(optimizationParameters.find("db_gp")->second);

    std::map<int,std::vector<double>> barycenters = provider.getMesh_barycenters();
    int start_control = static_cast<int>(optimizationParameters.find("start_control_gp")->second);
    int end_control = static_cast<int>(optimizationParameters.find("end_control_gp")->second);


    arma::mat D1_forward;
    arma::mat D1_backward;

    bool all_checked(true);

    try {
        D1_forward = solver.D1_forward();
        D1_backward = solver.D1_backward();
    } catch (std::out_of_range e) {
        std::cout << "Error while generating first derivative" << std::endl;
        std::cout << e.what() << std::endl;
        all_checked = false;
    }

    arma::mat control(pcell_gp,3,arma::fill::ones);

    arma::mat D2_fb = D1_forward*D1_backward;
    arma::mat D2_model = solver.Laplacian_3D();

    std::vector<double> current_barycenter;
    double paraboloid = 0.0;

    for(int i = 1; i<=pcell_gp; i++) {
        //for(int i = 1; i<=dimensionOfControl_gp; i++) {
        current_barycenter = barycenters.find(static_cast<int>(i))->second;

        paraboloid = pow(current_barycenter[0],2) + pow(current_barycenter[1],2) + pow(current_barycenter[2],2);

        control(i-1,0) = paraboloid;
        control(i-1,1) = paraboloid;
        control(i-1,2) = paraboloid;
    }

    std::cout << control << std::endl;

    output_control_update outContr = output_control_update();
    outContr.setData_provider_optim(provider);
    outContr.writeControl_XML(control);
    outContr.interpolate_control(provider);



    std::cout << 1.0/(db_gp)*D1_forward*control << std::endl;

    outContr.writeControl_XML(1.0/(db_gp)*D1_forward*control);
    outContr.interpolate_control(provider);

    std::cout << 1.0/(db_gp*db_gp)*D1_backward*D1_forward*control << std::endl;

    outContr.writeControl_XML(1.0/(db_gp*db_gp)*D1_backward*D1_forward*control);
    outContr.interpolate_control(provider);


    ASSERT_TRUE(all_checked);
}

TEST(solver,D1SecondOrder) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  input_xml_path = input_directory.c_str();

    data_provider provider = data_provider(input_xml_path);

    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(data_provider(input_xml_path));

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);


    arma::mat D1;

    bool all_checked(true);

    try {
        D1 = solver.D1_second_order();
    } catch (std::out_of_range e) {
        std::cout << "Error while generating first derivative" << std::endl;
        std::cout << e.what() << std::endl;
        all_checked = false;
    }

    arma::mat control(dimensionOfControl_gp,3,arma::fill::ones);
    arma::mat first_derivative = D1*control;

    if (arma::norm(first_derivative,"fro") != 0) {
        all_checked = false;
    }

    ASSERT_TRUE(all_checked);
}

TEST(solver, LaplaciansSymmetry){
    std::string input_directory = "./data/Optim_input_gTest.xml";
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

        Laplacian_Squared = solver.Laplacian_Squared_3D();
    } catch (std::exception e) {
        std::cout << "Error while generating Laplacian_Squared" << std::endl;
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

TEST(solver,LaplaciansDefiniteness) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  input_xml_path = input_directory.c_str();

    equation_solving_controller solver = equation_solving_controller();
    data_provider data_provider_opt = data_provider(input_xml_path);
    solver.setData_provider_optim(data_provider_opt);
    std::map<std::string, double> optimizationParameters = data_provider_opt.getOptimizationParameters();

    bool all_checked(true);

    arma::mat Laplace = solver.Laplacian_3D();
    arma::mat Laplace_Squared = solver.Laplacian_Squared_3D();

    arma::cx_vec eigval;
    arma::cx_mat eigvec;
    std::complex<double> temp_ev;
    double real_min = 0.0; double imag = 0.0;

    arma::eig_gen(eigval, eigvec, -Laplace);

    for (unsigned int i = 0; i<eigval.size(); i++) {
        temp_ev = eigval(i);
        if(temp_ev.real()<0) {
            all_checked = false;
            std::cout << "Negative real part of "<< i << "-th eigenvalue" << std::endl;
        }
        if (fabs(temp_ev.imag()) > pow(10,-7)) {
            all_checked = false;
            std::cout << "Non-zero imaginary part of " << i << "-th eigenvalue" << std::endl;
        }
    }

    arma::eig_gen(eigval, eigvec, Laplace_Squared);

    for (unsigned int i = 0; i<eigval.size(); i++) {
        temp_ev = eigval(i);
        if(temp_ev.real()<0) {
            all_checked = false;
            std::cout << "Negative real part of "<< i << "-th eigenvalue" << std::endl;
        }
        if (fabs(temp_ev.imag()) > pow(10,-7)) {
            all_checked = false;
            std::cout << "Non-zero imaginary part of " << i << "-th eigenvalue" << std::endl;
        }
    }


    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int vcell_gp = static_cast<unsigned int>(optimizationParameters.find("vcell_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    double dv_gp = static_cast<double>(optimizationParameters.find("dv_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double db_gp = static_cast<double>(optimizationParameters.find("db_gp")->second);
    double weight_control_gp = static_cast<double>(optimizationParameters.find("weight_control_gp")->second);
    double local_control_x_min_gp = static_cast<double>(optimizationParameters.find("local_control_x_min_gp")->second);
    double local_control_x_max_gp = static_cast<double>(optimizationParameters.find("local_control_x_max_gp")->second);

    //arma::mat Riesz = weight_control_gp*(arma::eye(dimensionOfControl_gp,dimensionOfControl_gp) - 1.0/(pow(db_gp,2))*Laplace + 1.0/(pow(db_gp,4))*Laplace_Squared);
    arma::mat Riesz = (arma::eye(dimensionOfControl_gp,dimensionOfControl_gp) - Laplace + Laplace_Squared);
    arma::eig_gen(eigval, eigvec, Riesz);

    for (unsigned int i = 0; i<eigval.size(); i++) {
        temp_ev = eigval(i);
        if(temp_ev.real()<0) {
            all_checked = false;
            std::cout << "Negative real part of "<< i << "-th eigenvalue" << std::endl;
        }
        if (fabs(temp_ev.imag()) > pow(10,-7)) {
            all_checked = false;
            std::cout << "Non-zero imaginary part of " << i << "-th eigenvalue" << std::endl;
        }
    }

    if(!all_checked) {
        std::cout << "Ries Matrix may not be positive definite" << std::endl;
    }

    ASSERT_TRUE(all_checked);
}
