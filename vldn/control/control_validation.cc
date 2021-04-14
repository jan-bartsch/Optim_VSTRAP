#include "control_validation.h"

control_verification::control_verification() {}

int control_verification::start_verification(int argc, char **argv) {
  DataProvider validation_provider = DataProvider(argv[1]);
  std::map<std::string, std::string> validation_paths =
      validation_provider.getPaths();
  std::map<std::string, std::string> validation_params =
      validation_provider.getOptimizationParameters();
  std::map<std::string, std::string> validation_routines =
      validation_provider.getSubroutines();

  std::string directory_optim_input =
      validation_paths.at("directory_optim_input");
  DataProvider optimization_provider =
      DataProvider(directory_optim_input.c_str());

  auto shared_optim_input_data = std::make_shared<MOTIONS::InputData>(
      MOTIONS::InitializeMotions::Load_MOTIONS(optimization_provider));

  int number_cells_position = shared_optim_input_data->number_cells_position;

  Input in = Input(shared_optim_input_data);
  OutputDiagnostics out = OutputDiagnostics(shared_optim_input_data);
  EquationSolvingController solver =
      EquationSolvingController(shared_optim_input_data);
  InnerProducts pro = InnerProducts(shared_optim_input_data);

  int iterations = std::stoi(validation_params.at("number_controls"));
  std::cout << "Running " << iterations << " validation iterations"
            << std::endl;

  std::string controls_directory =
      validation_paths.find("path_to_controls")->second;
  std::string validation_type =
      validation_routines.find("type_control_verification")->second;
  std::string number = "";
  std::string file = "";

  std::vector<arma::mat> control_vector;
  std::vector<double> control_difference(iterations - 1, 0.0);

  std::string discretization_file =
      controls_directory + "discretization_vector.txt";
  std::vector<double> discretization_vector =
      in.ReadDoubleVector(discretization_file.c_str());

  if (static_cast<int>(discretization_vector.size()) !=
      static_cast<int>(iterations)) {
    throw std::invalid_argument(
        "Number of weights and controls does not match");
  }

  //    std::vector<std::vector<double>> control1 =
  //    in.readDoubleMatrix("/afs/ifm/home/bartsch/SPARC/Optim_VSTRAP/vldn/data/controls-20201126-weight/control_1.csv",number_cells_position,",");
  //    //std::count << control1 << std::endl;

  //    arma::mat control1_mat(control1.size(),3,arma::fill::zeros);
  //    for(unsigned int i=0; i<control1.size(); i++) {
  //        for(unsigned int j=0; j<3; j++) {
  //            control1_mat(i,j) = control1[i][j];
  //        }
  //    }

  // std::vector<double> mean_weight6 = calculate_mean_doubleMatrix(control1);

  arma::mat means;
  std::vector<double> norms;
  arma::mat control;

  std::map<int, std::vector<double>> barycenters =
      optimization_provider.getMeshBarycenters();
  arma::mat bary = DataProvider::ConvertBarycentersToArmaMat(barycenters);

  std::vector<double> valide_vector;

  std::string barycenter_mesh_path = "";
  std::map<int, std::vector<double>> current_barycenters =
      optimization_provider.getMeshBarycenters();

  for (unsigned long i = 0; i < iterations; i++) {
    number = std::to_string(i);
    file = controls_directory + "control_" + number + ".xml";
    std::cout << "Open file with name " << file << std::endl;
    if (validation_type.compare("position") == 0) {
      arma::mat control = in.ReadControl(
          file.c_str(), static_cast<int>(discretization_vector[i]));
      barycenter_mesh_path =
          "../../Optim_VSTRAP/data/global/mesh_barycenters_small_" +
          std::to_string(static_cast<int>(discretization_vector[i])) + ".xml";
      current_barycenters =
          optimization_provider.ReadMeshBarycenters(&barycenter_mesh_path[0]);
      bary = DataProvider::ConvertBarycentersToArmaMat(current_barycenters);
      // means.insert_rows(i,arma::mean(calculate_cross_error(control,bary,valide_vector))*1.0/(static_cast<double>(discretization_vector[i]))
      // );
      means.insert_rows(i, arma::mean(control) * 0.001);
      // calculate_mean(control);
      std::cout << means << std::endl;
      norms.push_back(arma::norm(means.row(i)) * 0.001);
      valide_vector.push_back(1.0);
    } else if (validation_type.compare("weight") == 0) {
      control = in.ReadControl(file.c_str(), number_cells_position);
      means.insert_rows(
          i, arma::mean(calculate_cross_error(control, bary, valide_vector)));
      norms.push_back(arma::norm(means.row(i)) * valide_vector[i]);
    } else {
      throw std::invalid_argument("No such control verification type");
    }
    // std::cout << control << std::endl;
    control_vector.push_back(control);
    // calculate_cross_error(control,bary,valide_vector);
    // std::cout <<
    // arma::mean(calculate_cross_error(control,bary,valide_vector)) <<
    // std::endl; std::cout << "L2 norm: " <<
    // std::sqrt(pro.L2InnerProduct(control,control)) << std::endl; std::cout <<
    // "norm: " <<
    // arma::norm(control,"fro")*std::sqrt(0.001/discretization_file[i]) <<
    // std::endl; //*0.001/discretization_vector[i]
  }

  //    means(iterations-1,0) = mean_weight6[0]*0.001;
  //    means(iterations-1,1) = mean_weight6[1]*0.001;
  //    means(iterations-1,2) = mean_weight6[2]*0.001;
  // means.insert_rows(iterations-1,arma::mean(calculate_cross_error(control1_mat,bary,valide_vector)));
  // norms.push_back(arma::norm(means.row(iterations-1))*valide_vector[iterations-1]);
  std::cout << means << std::endl;

  out.WriteDoubleVectorToFile(norms, "Norms");
  out.writeGradientMatrixToFile(means, "Means");
  out.WriteDoubleVectorToFile(valide_vector, "Valide");

  std::string visualize_control_pyhton =
      "python3 " + shared_optim_input_data->directory_toolset + "vldn/" +
      "test_controls_mean.py " +
      shared_optim_input_data->path_to_shared_files_absolute;

  logger::Info("Calling command " + visualize_control_pyhton);
  system(&visualize_control_pyhton[0]);

  for (unsigned long i = 0; i < iterations - 1; i++) {
    //        control_difference[i] =
    //        std::sqrt(pro.H1InnerProduct(control_vector[i+1]*discretization_vector[i+1]-control_vector[i]*discretization_vector[i],
    //                control_vector[i+1]*discretization_vector[i+1]-control_vector[i]*discretization_vector[i]));
    control_difference[i] = std::sqrt(
        pro.H1InnerProduct(control_vector[i + 1] - control_vector[i],
                           control_vector[i + 1] - control_vector[i]));
    std::cout << control_difference[i] << std::endl;
  }

  out.WriteDoubleVectorToFile(control_difference, "H1-difference");

  for (unsigned long i = 0; i < iterations - 1; i++) {
    //        control_difference[i] =
    //        std::sqrt(pro.L2InnerProduct(control_vector[i+1]*discretization_vector[i+1]-control_vector[i]*discretization_vector[i],
    //                control_vector[i+1]*discretization_vector[i+1]-control_vector[i]*discretization_vector[i]));
    control_difference[i] = std::sqrt(
        pro.L2InnerProduct(control_vector[i + 1] - control_vector[i],
                           control_vector[i + 1] - control_vector[i]));
    std::cout << control_difference[i] << std::endl;
  }

  out.WriteDoubleVectorToFile(control_difference, "L2-difference");

  for (unsigned long i = 0; i < iterations - 1; i++) {
    control_difference[i] = std::sqrt(
        pro.H2InnerProduct(control_vector[i + 1] - control_vector[i],
                           control_vector[i + 1] - control_vector[i]));
    // control_difference[i] =
    // std::sqrt(pro.H2_inner_product(control_vector[i+1]-control_vector[i],control_vector[i+1]-control_vector[i]));
    std::cout << control_difference[i] << std::endl;
  }

  out.WriteDoubleVectorToFile(control_difference, "H2-difference");

  return 0;
}

std::vector<double> control_verification::calculate_mean(arma::mat control) {
  std::vector<double> mean(3, 0.0);

  std::cout << arma::mean(control) * 0.001 << std::endl;

  std::cout << control << std::endl;

  for (unsigned long j = 0; j < control.n_cols; j++) {
    for (unsigned long i = 0; i < control.n_rows; i++) {
      mean[j] += control(i, j);
    }
    mean[j] /= static_cast<double>(control.n_rows);
    std::cout << "Mean in column " << j << ": " << mean[j] << std::endl;
  }
  // return (std::abs(mean[0])+(mean[1])+(mean[2]))*0.001/3.0;
  return mean;
}

std::vector<double> control_verification::calculate_mean_doubleMatrix(
    std::vector<std::vector<double>> control) {
  std::vector<double> mean(3, 0.0);

  unsigned long columns = control[0].size();
  unsigned long rows = control.size();

  for (unsigned long j = 0; j < columns; j++) {
    for (unsigned long i = 0; i < rows; i++) {
      mean[j] += control[i][j];
    }
    mean[j] /= static_cast<double>(rows);
    std::cout << "Mean in column " << j << ": " << mean[j] << std::endl;
  }
  return mean;
}

arma::mat control_verification::calculate_cross_error(
    arma::mat control, arma::mat barycenters,
    std::vector<double> &valide_vector) {
  arma::mat error_vector;
  std::vector<double> orientation(control.n_rows);
  double valide = 1.0;

  for (unsigned int cell_id = 0; cell_id < barycenters.n_rows; cell_id++) {
    std::cout << control.row(cell_id) << std::endl;
    std::cout << -barycenters.row(cell_id) << std::endl;
    orientation[cell_id] =
        arma::dot(control.row(cell_id), -barycenters.row(cell_id));
    if (orientation[cell_id] < 0) {
      std::cerr << "Wrong orientation of control vector" << std::endl;
      valide = -1.0;
    }
    error_vector.insert_rows(cell_id, arma::cross(control.row(cell_id),
                                                  (-barycenters.row(cell_id))));
  }

  valide_vector.push_back(valide);

  // std::cout << error_vector << std::endl;
  return error_vector;
}
