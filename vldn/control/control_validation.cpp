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
    in.setData_provider_optim(validation_provider);

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

    for (int i = 0; i < iterations-1; i++) {
//        control_difference[i] = arma::norm(control_vector[i+1].col(0)-control_vector[i].col(0))
//                +arma::norm(control_vector[i+1].col(1)-control_vector[i].col(1))
//                +arma::norm(control_vector[i+1].col(2)-control_vector[i].col(2));
        control_difference[i] = arma::norm(control_vector[i+1]-control_vector[i])*(static_cast<double>(optimizationParameters.find("dp_gp")->second));
        std::cout << control_difference[i] << std::endl;
    }


    return 0;
}
