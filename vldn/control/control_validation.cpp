#include "control_validation.h"

control_validation::control_validation() { }

int control_validation::start_validation(int argc, char **argv)
{
    if (argc == 0) {
        throw std::invalid_argument("Too less arguments for starting control validation");
    }

    input in = input();

    int iterations = std::atoi(argv[1]);
    std::cout << "Running " << iterations << " validation iterations" << std::endl;

    std::string file = "";
    std::string number = "";
    const char *filename = argv[2];
    file += filename;


    std::vector<arma::mat> control_vector;
    std::vector<double> control_difference(iterations-1,0.0);

    std::string weight_file = "";
    weight_file += argv[2];
    weight_file += "weight_vector.txt";
    std::vector<double> weight_vector = in.readDoubleVector(weight_file.c_str());

    for (int i = 0; i < iterations; i++) {
        number = std::to_string(i);
        file = file +"control_" + number + ".xml";
        std::cout << "Open file with name " << file << std::endl;
        arma::mat control =  in.readControl(file.c_str());
        control_vector.push_back(control);
        file = ""; file += filename;
    }

    for (int i = 0; i < iterations-1; i++) {
        control_difference[i] = arma::norm(control_vector[i+1]/weight_vector[i+1]-control_vector[i]/weight_vector[i]);
        std::cout << control_difference[i] << std::endl;
    }


    return 0;
}
