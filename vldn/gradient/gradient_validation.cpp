#include "gradient_validation.h"

gradient_validation::gradient_validation(){ }

int gradient_validation::landau_validation(int argc, char **argv) {
    data_provider validation_provider = data_provider(argv[1]);
    std::map<std::string,std::string> validation_paths = validation_provider.getPaths();
    std::map<std::string,double> validation_params = validation_provider.getOptimizationParameters();

    std::string DIRECTORY_OPTIM_INPUT = validation_paths.find("DIRECTORY_OPTIM_INPUT")->second;
    data_provider optimization_provider = data_provider(DIRECTORY_OPTIM_INPUT.c_str());

    input in = input();
    in.setData_provider_optim(optimization_provider);

    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(optimization_provider);

    output_diagnostics outDiag = output_diagnostics();
    outDiag.setData_provider_optim(optimization_provider);

    equation_solving_controller model_solver = equation_solving_controller();
    model_solver.setData_provider_optim(optimization_provider);

    optim_controller contr = optim_controller();
    contr.setData_provider_optim(optimization_provider);

    inner_products product = inner_products();
    product.setData_provider_optim(optimization_provider);

    //contr.generate_input_files(DIRECTORY_OPTIM_INPUT.c_str());

    gradient_calculator gradient_calculator_opt = gradient_calculator(DIRECTORY_OPTIM_INPUT.c_str());
    objective_calculator objective = objective_calculator(DIRECTORY_OPTIM_INPUT.c_str());
    output_control_update outController = output_control_update(DIRECTORY_OPTIM_INPUT.c_str());

    std::map<std::string, double> optimizationParameters = optimization_provider.getOptimizationParameters();
    std::map<std::string,std::string> paths = optimization_provider.getPaths();

    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    double pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);

    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF;

    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> backwardPDF;

    std::vector<std::unordered_map<coordinate_phase_space_time,double>> pdf_time(ntimesteps_gp);
    int assembling_flag;

    std::string BUILD_DIRECTORY_VSTRAP = paths.find("BUILD_DIRECTORY_VSTRAP")->second;
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string INPUT_FORWARD = paths.find("INPUT_FORWARD")->second;
    std::string INPUT_BACKWARD = paths.find("INPUT_BACKWARD")->second;

    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;

    std::string START_VSTRAP_FORWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + INPUT_FORWARD;
    int forward_return = 0;
    std::string START_VSTRAP_BACKWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + INPUT_BACKWARD;
    int backward_return = 0;

    arma::mat control0(pcell_gp,3,arma::fill::zeros);
//    control0 = -10.0*control0;
    arma::mat gradient(pcell_gp,3,arma::fill::zeros);

    outController.writeControl_XML(control0);
    outDiag.writeDoubleToFile(arma::norm(control0,"fro"),"normControlTrack");
    outController.interpolate_control(optimization_provider);

    /*
     * calculat J^(u0)
     */
    logger::Info("Starting VSTRAP (foward)... ");
    forward_return = model_solver.start_solving_forward(START_VSTRAP_FORWARD);
    if (forward_return != 0) {
        logger::Info("Forward VSTRAP returned non-zero value: " + std::to_string(forward_return));
        throw  std::system_error();
    }
    logger::Info("Finished VSTRAP... Reading particle files");
    in.read_plasma_state_forward(forwardParticles);

    logger::Info("Finished reading files...");
    logger::Info("Starting VSTRAP (backward)...");

    backward_return = model_solver.start_solving_backward(START_VSTRAP_BACKWARD);
    if (backward_return != 0)  {
        logger::Info("Backward VSTRAP returned non-zero value: " + std::to_string(backward_return));
        throw std::system_error();
    }

    logger::Info("Reading particle files...");
    in.read_plasma_state_backward(backwardParticles);


    logger::Info("Assembling pdfs...");

    assembling_flag = pdf_control.assemblingMultiDim_parallel(forwardParticles,0,pdf_time);
    forwardPDF = pdf_time;
    assembling_flag = pdf_control.assemblingMultiDim_parallel(backwardParticles,1,pdf_time);
    backwardPDF = pdf_time;

    if (assembling_flag != 0) {
        throw std::runtime_error("Too many too fast particles!");
    }

    double functional_value0 = objective.calculate_objective_L2(forwardPDF,control0);


    logger::Info("Building gradient...");
    arma::mat gradient0 = gradient_calculator_opt.calculateGradient_forceControl_space_Hm(forwardPDF,backwardPDF,control0);
    outDiag.writeDoubleToFile(arma::norm(gradient,"fro"),"normGradientTrack");

    arma::mat delta_control(pcell_gp,3,arma::fill::ones);
    double t = -10.0;
    delta_control = t*delta_control;

    unsigned long iteration_number = 10;
    double reducing_factor = 0.5;
    double dp_gp = static_cast<double>(optimizationParameters.find("dp_gp")->second);

    std::vector<double> functional_values(iteration_number,0.0);
    std::vector<double> difference(iteration_number,0.0);
    std::vector<double> difference_Landau(iteration_number,0.0);
    arma::mat control_temp;

    for(unsigned int i = 0; i< static_cast<unsigned long>(iteration_number); i++) {
        control_temp = control0 + pow(reducing_factor,i)*delta_control;
        outController.writeControl_XML(control_temp);
        outController.interpolate_control(optimization_provider);

        logger::Info("Starting VSTRAP (foward)... ");
        forward_return = model_solver.start_solving_forward(START_VSTRAP_FORWARD);
        if (forward_return != 0) {
            logger::Info("Forward VSTRAP returned non-zero value: " + std::to_string(forward_return));
            throw  std::system_error();
        }
        logger::Info("Finished VSTRAP... Reading particle files");
        in.read_plasma_state_forward(forwardParticles);

        assembling_flag = pdf_control.assemblingMultiDim_parallel(forwardParticles,0,pdf_time);
        forwardPDF = pdf_time;

        if (assembling_flag != 0) {
            throw std::runtime_error("Too many too fast particles!");
        }

        functional_values[i] = objective.calculate_objective_L2(forwardPDF,control_temp);
        std::cout << std::to_string(functional_values[i]) << std::endl;
        std::cout << "Stepsize: " << pow(reducing_factor,i) << std::endl;
        difference[i] = (functional_values[i]-functional_value0)-(pow(reducing_factor,i))*product.L2_inner_product(gradient0,delta_control);

        difference_Landau[i] = std::abs(difference[i])/(pow(pow(reducing_factor,i),2));

        outDiag.writeDoubleVectorToFile(difference,"Difference");
        outDiag.writeDoubleVectorToFile(difference_Landau,"Difference_Landau");
        outDiag.writeDoubleVectorToFile(functional_values,"FunctionalValues");
    }

    std::cout << "Difference: " << std::endl;
    for(int i = 0; i< iteration_number; i++) {
        std::cout << std::to_string(difference[i]) << std::endl;
    }
    std::cout << "Difference Landau: " << std::endl;
    for(int i = 0; i< iteration_number; i++) {
        std::cout << std::to_string(difference_Landau[i]) << std::endl;
    }
}
