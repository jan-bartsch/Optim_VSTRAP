#include <gtest/gtest.h>

#include "../../src/controller/equationsolvingcontroller.h"
#include "../../src/controller/optimcontroller.h"
#include "../../src/io/outputcontrolupdate.h"
#include "../../src/logger/logger.h"
#include "../../src/objects/dataprovider.h"

TEST(solver, D1forwardBackward) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);

  EquationSolvingController solver = EquationSolvingController();
  solver.set_DataProviderOptim(DataProvider(Input_xml_path));

  std::map<std::string, double> optimizationParameters =
      provider.getOptimizationParameters();
  unsigned int dimensionOfControl_gp = static_cast<unsigned int>(
      optimizationParameters.find("dimensionOfControl_gp")->second);

  arma::mat D1_forward;
  arma::mat D1_backward;

  bool all_checked(true);

  try {
    D1_forward = solver.D1Forward();
    std::cout << "D1_forward:" << std::endl;
    std::cout << D1_forward << std::endl;

    D1_backward = solver.D1Backward();
    std::cout << "D1_backward:" << std::endl;
    std::cout << D1_backward << std::endl;
  } catch (std::out_of_range e) {
    std::cout << "Error while generating first derivative" << std::endl;
    std::cout << e.what() << std::endl;
    all_checked = false;
  }

  arma::mat control(dimensionOfControl_gp, 3, arma::fill::ones);
  arma::mat first_derivative_forward = D1_forward * control;
  arma::mat first_derivative_backward = D1_backward * control;

  std::cout << "First derivative of constant one vector (forward)" << std::endl;
  std::cout << first_derivative_forward << std::endl;

  std::cout << "First derivative of constant one vector (backward)"
            << std::endl;
  std::cout << first_derivative_backward << std::endl;

  if (arma::norm(first_derivative_forward, "fro") != 0 ||
      arma::norm(first_derivative_backward, "fro") != 0) {
    all_checked = false;
  }

  ASSERT_TRUE(all_checked);
}

// TEST(solver,checkSecondDerivativeforwardbackward) {
//    std::string Input_directory = "./data/Optim_Input_gTest.xml";
//    const char *  Input_xml_path = Input_directory.c_str();

//    data_provider provider = data_provider(Input_xml_path);

//    equation_solving_controller solver = equation_solving_controller();
//    solver.set_DataProviderOptim(data_provider(Input_xml_path));

//    std::map<std::string, double> optimizationParameters =
//    provider.getOptimizationParameters(); unsigned int dimensionOfControl_gp =
//    static_cast<unsigned
//    int>(optimizationParameters.find("dimensionOfControl_gp")->second);

//    arma::mat D1_forward;
//    arma::mat D1_backward;

//    bool all_checked(true);

//    try {
//        D1_forward = solver.D1_forward();
//        D1_backward = solver.D1_backward();
//    } catch (std::out_of_range e) {
//        std::cout << "Error while generating first derivative" << std::endl;
//        std::cout << e.what() << std::endl;
//        all_checked = false;
//    }

//    arma::mat control(dimensionOfControl_gp,3,arma::fill::ones);

//    arma::mat D2_fb = D1_forward*D1_backward;
//    arma::mat D2_model = solver.Laplacian_3D();

//    if (arma::norm(D2_fb - D2_model,"fro") != 0) {
//        all_checked = false;
//    }

//    EXPECT_TRUE(all_checked);
//}

TEST(solver, checksecondDerivativeCalculation) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);

  EquationSolvingController solver = EquationSolvingController();
  solver.set_DataProviderOptim(DataProvider(Input_xml_path));

  std::map<std::string, double> optimizationParameters =
      provider.getOptimizationParameters();
  unsigned int dimensionOfControl_gp = static_cast<unsigned int>(
      optimizationParameters.find("dimensionOfControl_gp")->second);
  unsigned int number_cells_position = static_cast<unsigned int>(
      optimizationParameters.find("number_cells_position")->second);
  double small_discr_sidelength = static_cast<double>(
      optimizationParameters.find("small_discr_sidelength")->second);

  std::map<int, std::vector<double>> barycenters =
      provider.getMeshBarycenters();
  int start_control =
      static_cast<int>(optimizationParameters.find("start_control_gp")->second);
  int end_control =
      static_cast<int>(optimizationParameters.find("end_control_gp")->second);

  arma::mat D1_forward;
  arma::mat D1_backward;

  bool all_checked(true);

  try {
    D1_forward = solver.D1Forward();
    D1_backward = solver.D1Backward();
  } catch (std::out_of_range e) {
    std::cout << "Error while generating first derivative" << std::endl;
    std::cout << e.what() << std::endl;
    all_checked = false;
  }

  arma::mat control(number_cells_position, 3, arma::fill::ones);

  arma::mat D2_fb = D1_forward * D1_backward;
  arma::mat D2_model = solver.Laplacian3D();

  std::vector<double> current_barycenter;
  double paraboloid = 0.0;

  for (int i = 1; i <= number_cells_position; i++) {
    // for(int i = 1; i<=dimensionOfControl_gp; i++) {
    current_barycenter = barycenters.find(static_cast<int>(i))->second;

    paraboloid = pow(current_barycenter[0], 2) + pow(current_barycenter[1], 2) +
                 pow(current_barycenter[2], 2);

    control(i - 1, 0) = paraboloid;
    control(i - 1, 1) = paraboloid;
    control(i - 1, 2) = paraboloid;
  }

  std::cout << control << std::endl;

  OutputControlUpdate outContr = OutputControlUpdate();
  outContr.set_DataProviderOptim(provider);
  outContr.WritecontrolXml(control);
  outContr.InterpolateControl(provider);

  std::cout << 1.0 / (small_discr_sidelength)*D1_forward * control << std::endl;

  outContr.WritecontrolXml(1.0 / (small_discr_sidelength)*D1_forward * control);
  outContr.InterpolateControl(provider);

  std::cout << 1.0 / (small_discr_sidelength * small_discr_sidelength) *
                   D1_backward * D1_forward * control
            << std::endl;

  outContr.WritecontrolXml(1.0 /
                           (small_discr_sidelength * small_discr_sidelength) *
                           D1_backward * D1_forward * control);
  outContr.InterpolateControl(provider);

  ASSERT_TRUE(all_checked);
}

TEST(solver, D1SecondOrder) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  DataProvider provider = DataProvider(Input_xml_path);

  EquationSolvingController solver = EquationSolvingController();
  solver.set_DataProviderOptim(DataProvider(Input_xml_path));

  std::map<std::string, double> optimizationParameters =
      provider.getOptimizationParameters();
  unsigned int dimensionOfControl_gp = static_cast<unsigned int>(
      optimizationParameters.find("dimensionOfControl_gp")->second);

  arma::mat D1;

  bool all_checked(true);

  try {
    D1 = solver.D1SecondOrder();
  } catch (std::out_of_range e) {
    std::cout << "Error while generating first derivative" << std::endl;
    std::cout << e.what() << std::endl;
    all_checked = false;
  }

  arma::mat control(dimensionOfControl_gp, 3, arma::fill::ones);
  arma::mat first_derivative = D1 * control;

  if (arma::norm(first_derivative, "fro") != 0) {
    all_checked = false;
  }

  ASSERT_TRUE(all_checked);
}

TEST(solver, LaplaciansSymmetry) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  EquationSolvingController solver = EquationSolvingController();
  solver.set_DataProviderOptim(DataProvider(Input_xml_path));
  arma::mat Laplacian;
  arma::mat Laplacian_Squared;
  bool all_checked(true);

  try {
    Laplacian = solver.Laplacian3D();
  } catch (std::out_of_range e) {
    std::cout << "Error while generating Laplacian" << std::endl;
    std::cout << e.what() << std::endl;
    all_checked = false;
  }

  try {

    Laplacian_Squared = solver.LaplacianSquared3D();
  } catch (std::exception e) {
    std::cout << "Error while generating Laplacian_Squared" << std::endl;
    std::cout << e.what() << std::endl;
    all_checked = false;
  }

  if (!all_checked) {
    ASSERT_TRUE(all_checked);
  }

  if (!arma::approx_equal(Laplacian, Laplacian.t(), "both", pow(10, -5),
                          pow(10, -5))) {
    std::cout << "Laplace not symmetric!" << std::endl;
    std::cout << Laplacian << std::endl;
    all_checked = false;
  }

  if (!arma::approx_equal(Laplacian_Squared, Laplacian_Squared.t(), "both",
                          pow(10, -5), pow(10, -5))) {
    std::cout << "Laplacian Squared not symmetric!" << std::endl;
    all_checked = false;
  }

  ASSERT_TRUE(all_checked);
}

TEST(solver, LaplaciansDefiniteness) {
  std::string Input_directory = "./data/Optim_Input_gTest.xml";
  const char *Input_xml_path = Input_directory.c_str();

  EquationSolvingController solver = EquationSolvingController();
  DataProvider data_provider_opt = DataProvider(Input_xml_path);
  solver.set_DataProviderOptim(data_provider_opt);
  std::map<std::string, double> optimizationParameters =
      data_provider_opt.getOptimizationParameters();

  bool all_checked(true);

  arma::mat Laplace = solver.Laplacian3D();
  arma::mat Laplace_Squared = solver.LaplacianSquared3D();

  arma::cx_vec eigval;
  arma::cx_mat eigvec;
  std::complex<double> temp_ev;
  double real_min = 0.0;
  double imag = 0.0;

  arma::eig_gen(eigval, eigvec, -Laplace);

  for (unsigned int i = 0; i < eigval.size(); i++) {
    temp_ev = eigval(i);
    if (temp_ev.real() < 0) {
      all_checked = false;
      std::cout << "Negative real part of " << i << "-th eigenvalue"
                << std::endl;
    }
    if (fabs(temp_ev.imag()) > pow(10, -7)) {
      all_checked = false;
      std::cout << "Non-zero imaginary part of " << i << "-th eigenvalue"
                << std::endl;
    }
  }

  arma::eig_gen(eigval, eigvec, Laplace_Squared);

  for (unsigned int i = 0; i < eigval.size(); i++) {
    temp_ev = eigval(i);
    if (temp_ev.real() < 0) {
      all_checked = false;
      std::cout << "Negative real part of " << i << "-th eigenvalue"
                << std::endl;
    }
    if (fabs(temp_ev.imag()) > pow(10, -7)) {
      all_checked = false;
      std::cout << "Non-zero imaginary part of " << i << "-th eigenvalue"
                << std::endl;
    }
  }

  unsigned int number_cells_position = static_cast<unsigned int>(
      optimizationParameters.find("number_cells_position")->second);
  unsigned int vcell_gp = static_cast<unsigned int>(
      optimizationParameters.find("vcell_gp")->second);
  unsigned int dimensionOfControl_gp = static_cast<unsigned int>(
      optimizationParameters.find("dimensionOfControl_gp")->second);
  double dv_gp =
      static_cast<double>(optimizationParameters.find("dv_gp")->second);
  double dt_gp =
      static_cast<double>(optimizationParameters.find("dt_gp")->second);
  double small_discr_sidelength = static_cast<double>(
      optimizationParameters.find("small_discr_sidelength")->second);
  double weight_control_gp = static_cast<double>(
      optimizationParameters.find("weight_control_gp")->second);
  double local_control_x_min_gp = static_cast<double>(
      optimizationParameters.find("local_control_x_min_gp")->second);
  double local_control_x_max_gp = static_cast<double>(
      optimizationParameters.find("local_control_x_max_gp")->second);

  // arma::mat Riesz =
  // weight_control_gp*(arma::eye(dimensionOfControl_gp,dimensionOfControl_gp)
  // - 1.0/(pow(small_discr_sidelength,2))*Laplace
  // + 1.0/(pow(small_discr_sidelength,4))*Laplace_Squared);
  arma::mat Riesz = (arma::eye(dimensionOfControl_gp, dimensionOfControl_gp) -
                     Laplace + Laplace_Squared);
  arma::eig_gen(eigval, eigvec, Riesz);

  for (unsigned int i = 0; i < eigval.size(); i++) {
    temp_ev = eigval(i);
    if (temp_ev.real() < 0) {
      all_checked = false;
      std::cout << "Negative real part of " << i << "-th eigenvalue"
                << std::endl;
    }
    if (fabs(temp_ev.imag()) > pow(10, -7)) {
      all_checked = false;
      std::cout << "Non-zero imaginary part of " << i << "-th eigenvalue"
                << std::endl;
    }
  }

  if (!all_checked) {
    std::cout << "Ries Matrix may not be positive definite" << std::endl;
  }

  ASSERT_TRUE(all_checked);
}
