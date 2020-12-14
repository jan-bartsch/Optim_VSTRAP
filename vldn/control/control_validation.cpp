#include "control_validation.h"

control_verification::control_verification() { }

int control_verification::start_verification(int argc, char **argv)
{
    data_provider validation_provider = data_provider(argv[1]);
    std::map<std::string,std::string> validation_paths = validation_provider.getPaths();
    std::map<std::string,double> validation_params = validation_provider.getOptimizationParameters();
    std::map<std::string,std::string> validation_routines = validation_provider.getSubroutines();

    std::string DIRECTORY_OPTIM_INPUT = validation_paths.find("DIRECTORY_OPTIM_INPUT")->second;
    data_provider optimization_provider = data_provider(DIRECTORY_OPTIM_INPUT.c_str());
    std::map<std::string, double> optimizationParameters = optimization_provider.getOptimizationParameters();
    std::map<std::string,std::string> paths = optimization_provider.getPaths();

    int pcell_gp = static_cast<int>(optimizationParameters.find("pcell_gp")->second);

    input in = input();
    in.setData_provider_optim(optimization_provider);

    output_diagnostics out = output_diagnostics();
    out.setData_provider_optim(optimization_provider);

    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(optimization_provider);

    inner_products pro = inner_products();
    pro.setData_provider_optim(optimization_provider);


    int iterations = static_cast<int>(validation_params.find("number_controls")->second);
    std::cout << "Running " << iterations << " validation iterations" << std::endl;

    std::string CONTROLS_DIRECTORY = validation_paths.find("PATH_TO_CONTROLS")->second;
    std::string VALIDATION_TYPE = validation_routines.find("type_control_verification")->second;
    std::string number = "";
    std::string file = "";


    std::vector<arma::mat> control_vector;
    std::vector<double> control_difference(iterations-1,0.0);

    std::string discretization_file = CONTROLS_DIRECTORY + "discretization_vector.txt";
    std::vector<double> discretization_vector = in.readDoubleVector(discretization_file.c_str());

    if(static_cast<int>(discretization_vector.size()) != iterations) {
        throw std::invalid_argument("Number of weights and controls does not match");
    }


    std::vector<std::vector<double>> control1 = in.readDoubleMatrix("/afs/ifm/home/bartsch/SPARC/Optim_VSTRAP/vldn/data/controls-20201126-weight/control_1.csv",pcell_gp,",");
    //std::count << control1 << std::endl;

    arma::mat control1_mat(control1.size(),3,arma::fill::zeros);
    for(unsigned int i=0; i<control1.size(); i++) {
        for(unsigned int j=0; j<3; j++) {
            control1_mat(i,j) = control1[i][j];
        }
    }

    std::vector<double> mean_weight6 = calculate_mean_doubleMatrix(control1);

    arma::mat means;
    arma::mat control;

    std::map<int, std::vector<double>> barycenters = optimization_provider.getMesh_barycenters();
    arma::mat bary = data_provider::convert_barycenters_toArmaMat(barycenters);

    std::vector<double> valide_vector;


    for (int i = 0; i < iterations-1; i++) {
        number = std::to_string(i);
        file = CONTROLS_DIRECTORY +"control_" + number + ".xml";
        std::cout << "Open file with name " << file << std::endl;
        if (VALIDATION_TYPE.compare("position")==0) {
            arma::mat control =  in.readControl(file.c_str(),static_cast<int>(discretization_vector[i]));
        } else if (VALIDATION_TYPE.compare("weight")==0) {
            control =  in.readControl(file.c_str(),pcell_gp);
        } else {
            throw std::invalid_argument("No such control verification type");
        }
        //std::cout << control << std::endl;
        control_vector.push_back(control);
        //calculate_cross_error(control,bary,valide_vector);
        //std::cout << arma::mean(calculate_cross_error(control,bary,valide_vector)) << std::endl;
//        means.insert_rows(i,arma::mean(control)*0.001);
        means.insert_rows(i,arma::mean(calculate_cross_error(control,bary,valide_vector)) );
        //std::cout << "L2 norm: " << std::sqrt(pro.L2_inner_product(control,control)) << std::endl;
        //std::cout << "norm: " << arma::norm(control,"fro")*std::sqrt(0.001/discretization_file[i]) << std::endl; //*0.001/discretization_vector[i]
    }

//    means(iterations-1,0) = mean_weight6[0]*0.001;
//    means(iterations-1,1) = mean_weight6[1]*0.001;
//    means(iterations-1,2) = mean_weight6[2]*0.001;
    means.insert_rows(iterations-1,arma::mean(calculate_cross_error(control1_mat,bary,valide_vector)));
    std::cout << means << std::endl;



    out.writeArmaMatrixToFile(means,"Means");
    out.writeDoubleVectorToFile(valide_vector,"Valide");

    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string PATH_TO_SHARED_FILES_ABSOLUTE  = paths.find("PATH_TO_SHARED_FILES_ABSOLUTE")->second;

    std::string visualize_control_pyhton = "python3 " + DIRECTORY_TOOLSET + "vldn/" + "test_controls_mean.py " + PATH_TO_SHARED_FILES_ABSOLUTE;

    logger::Info("Calling command " + visualize_control_pyhton);
    system(&visualize_control_pyhton[0]);



    for (int i = 0; i < iterations-1; i++) {
        //        control_difference[i] = std::sqrt(pro.H1_inner_product(control_vector[i+1]*discretization_vector[i+1]-control_vector[i]*discretization_vector[i],
        //                control_vector[i+1]*discretization_vector[i+1]-control_vector[i]*discretization_vector[i]));
        control_difference[i] = std::sqrt(pro.H1_inner_product(control_vector[i+1]-control_vector[i],control_vector[i+1]-control_vector[i]));
        std::cout << control_difference[i] << std::endl;
    }

    out.writeDoubleVectorToFile(control_difference,"H1-difference");

    for (int i = 0; i < iterations-1; i++) {
        //        control_difference[i] = std::sqrt(pro.L2_inner_product(control_vector[i+1]*discretization_vector[i+1]-control_vector[i]*discretization_vector[i],
        //                control_vector[i+1]*discretization_vector[i+1]-control_vector[i]*discretization_vector[i]));
        control_difference[i] = std::sqrt(pro.L2_inner_product(control_vector[i+1]-control_vector[i],control_vector[i+1]-control_vector[i]));
        std::cout << control_difference[i] << std::endl;
    }

    out.writeDoubleVectorToFile(control_difference,"L2-difference");

    for (int i = 0; i < iterations-1; i++) {
        control_difference[i] = std::sqrt(pro.H2_inner_product(control_vector[i+1]-control_vector[i],
                                          control_vector[i+1]-control_vector[i]));
        //control_difference[i] = std::sqrt(pro.H2_inner_product(control_vector[i+1]-control_vector[i],control_vector[i+1]-control_vector[i]));
        std::cout << control_difference[i] << std::endl;
    }

    out.writeDoubleVectorToFile(control_difference,"H2-difference");

    return 0;
}

double control_verification::calculate_mean(arma::mat control)
{
    std::vector<double> mean(3,0.0);

    std::cout << arma::mean(control)*0.001 << std::endl;


    for(unsigned long j = 0; j < control.n_cols; j++ ) {
        for(unsigned long i = 0; i<control.n_rows; i++) {
            mean[j] += control(i,j);
        }
        mean[j] /= static_cast<double>(control.n_rows);
        std::cout << "Mean in column " << j << ": " << mean[j] << std::endl;
    }
    return (std::abs(mean[0])+(mean[1])+(mean[2]))*0.001/3.0;
}


std::vector<double> control_verification::calculate_mean_doubleMatrix(std::vector<std::vector<double>> control)
{
    std::vector<double> mean(3,0.0);

    int columns = control[0].size();
    int rows = control.size();


    for(unsigned long j = 0; j < columns; j++ ) {
        for(unsigned long i = 0; i< rows; i++) {
            mean[j] += control[i][j];
        }
        mean[j] /= static_cast<double>(rows);
        std::cout << "Mean in column " << j << ": " << mean[j] << std::endl;
    }
    return mean;
}

arma::mat control_verification::calculate_cross_error(arma::mat control, arma::mat barycenters, std::vector<double> &valide_vector)
{
    arma::mat error_vector;
    std::vector<double> orientation(control.n_rows);
    double valide = 1.0;


    for(unsigned int cell_id = 0; cell_id < barycenters.n_rows; cell_id++) {
        //std::cout << control.row(cell_id) << std::endl;
        //std::cout << -barycenters.row(cell_id) << std::endl;
        orientation[cell_id] = arma::dot(control.row(cell_id),-barycenters.row(cell_id));
        if(orientation[cell_id]<0) {
            std::cerr << "Wrong orientation of control vector" << std::endl;
            valide = -1.0;
        }
         error_vector.insert_rows(cell_id, arma::cross(control.row(cell_id),(-barycenters.row(cell_id))));
    }

    valide_vector.push_back(valide);

    //std::cout << error_vector << std::endl;
    return error_vector;

}
