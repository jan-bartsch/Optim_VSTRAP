#include "control_validation.h"

control_validation::control_validation() { }

int control_validation::start_validation(int argc, char **argv)
{
    data_provider validation_provider = data_provider(argv[1]);
    std::map<std::string,std::string> validation_paths = validation_provider.getPaths();
    std::map<std::string,double> validation_params = validation_provider.getOptimizationParameters();

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
    std::string number = "";
    std::string file = "";


    std::vector<arma::mat> control_vector;
    std::vector<double> control_difference(iterations-1,0.0);

    std::string discretization_file = CONTROLS_DIRECTORY + "discretization_vector.txt";
    std::vector<double> discretization_vector = in.readDoubleVector(discretization_file.c_str());

    if(static_cast<int>(discretization_vector.size()) != iterations) {
        throw std::invalid_argument("Number of weights and controls does not match");
    }

    arma::mat means;

    for (int i = 0; i < iterations; i++) {
        number = std::to_string(i);
        file = CONTROLS_DIRECTORY +"control_" + number + ".xml";
        std::cout << "Open file with name " << file << std::endl;
       arma::mat control =  in.readControl(file.c_str(),static_cast<int>(discretization_vector[i]));
       // arma::mat control =  in.readControl(file.c_str(),pcell_gp);
        //std::cout << control << std::endl;
        control_vector.push_back(control);
        std::cout << calculate_mean(control) << std::endl;
        means.insert_rows(i,arma::mean(control)*0.001);
        //std::cout << "L2 norm: " << std::sqrt(pro.L2_inner_product(control,control)) << std::endl;
        //std::cout << "norm: " << arma::norm(control,"fro")*std::sqrt(0.001/discretization_file[i]) << std::endl; //*0.001/discretization_vector[i]
    }

    std::cout << means << std::endl;
    out.writeGradientToFile(means,"Means");

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


    //    std::cout << solver.D1_second_order() << std::endl;
    //    arma::mat test(32,3,arma::fill::ones);

    //    std::cout << -solver.Laplacian_3D()/(static_cast<double>(optimizationParameters.find("db_gp")->second)*static_cast<double>(optimizationParameters.find("db_gp")->second))*test << std::endl;



    //    std::cout << "L2 norm " << std::sqrt(pro.L2_inner_product(control_vector[0],control_vector[0])) << std::endl;
    //    std::cout << "Arma norm " << arma::norm((control_vector[0]),"fro")*std::sqrt(static_cast<double>(optimizationParameters.find("dp_gp")->second)) << std::endl;
    //    std::cout << std::sqrt(pro.H1_inner_product(control_vector[0],control_vector[0])) << std::endl;
    //    std::cout << std::sqrt(pro.H2_inner_product(control_vector[0],control_vector[0])) << std::endl;

    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string PATH_TO_SHARED_FILES_ABSOLUTE  = paths.find("PATH_TO_SHARED_FILES_ABSOLUTE")->second;

    std::string visualize_control_pyhton = "python3 " + DIRECTORY_TOOLSET + "vldn/" + "test_controls.py " + PATH_TO_SHARED_FILES_ABSOLUTE + " 0.5";

    logger::Info("Calling command " + visualize_control_pyhton);
    system(&visualize_control_pyhton[0]);


    return 0;
}

double control_validation::calculate_mean(arma::mat control)
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
