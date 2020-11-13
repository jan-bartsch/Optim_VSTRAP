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

    input in = input();
    in.setData_provider_optim(optimization_provider);

    output_diagnostics out = output_diagnostics();
    out.setData_provider_optim(optimization_provider);

    int iterations = static_cast<int>(validation_params.find("number_controls")->second);
    std::cout << "Running " << iterations << " validation iterations" << std::endl;

    std::string CONTROLS_DIRECTORY = validation_paths.find("PATH_TO_CONTROLS")->second;
    std::string number = "";
    std::string file = "";


    std::vector<arma::mat> control_vector;
    std::vector<double> control_difference(iterations-1,0.0);

    std::string weight_file = CONTROLS_DIRECTORY + "weight_vector.txt";
    std::vector<double> weight_vector = in.readDoubleVector(weight_file.c_str());

    if(static_cast<int>(weight_vector.size()) != iterations) {
        throw std::invalid_argument("Number of weights and controls does not match");
    }

    for (int i = 0; i < iterations; i++) {
        number = std::to_string(i);
        file = CONTROLS_DIRECTORY +"control_" + number + ".xml";
        std::cout << "Open file with name " << file << std::endl;
        arma::mat control =  in.readControl(file.c_str());
        control_vector.push_back(control);
        std::cout << "norm: " << arma::norm(control) << std::endl;
    }

    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(optimization_provider);

    inner_products pro = inner_products();
    pro.setData_provider_optim(optimization_provider);


    for (int i = 0; i < iterations-1; i++) {
        control_difference[i] = std::sqrt(pro.H1_inner_product(control_vector[i+1]*weight_vector[i+1]-control_vector[i]*weight_vector[i],
                control_vector[i+1]*weight_vector[i+1]-control_vector[i]*weight_vector[i]));
        std::cout << control_difference[i] << std::endl;
    }

    out.writeDoubleVectorToFile(control_difference,"H1-difference");

    for (int i = 0; i < iterations-1; i++) {
        control_difference[i] = std::sqrt(pro.L2_inner_product(control_vector[i+1]*weight_vector[i+1]-control_vector[i]*weight_vector[i],
                control_vector[i+1]*weight_vector[i+1]-control_vector[i]*weight_vector[i]));
        //control_difference[i] = std::sqrt(pro.H2_inner_product(control_vector[i+1]-control_vector[i],control_vector[i+1]-control_vector[i]));
        std::cout << control_difference[i] << std::endl;
    }

    out.writeDoubleVectorToFile(control_difference,"L2-difference");

    for (int i = 0; i < iterations-1; i++) {
        control_difference[i] = std::sqrt(pro.H2_inner_product(control_vector[i+1]*weight_vector[i+1]-control_vector[i]*weight_vector[i],
                control_vector[i+1]*weight_vector[i+1]-control_vector[i]*weight_vector[i]));
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
