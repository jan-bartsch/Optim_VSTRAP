#include <gtest/gtest.h>

#include <exception>

#include "../../src/optimization/gradient_calculator.h"
#include "../../src/io/input.h"
#include "../../src/controller/pdf_controller.h"
#include "../../src/io/output_diagnostics.h"
#include "../../src/controller/equation_solving_controller.h"
#include "../../src/optimization/objective_calculator.h"


TEST(gradient,checkLandauApproximatio) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);

    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(provider);

    input input_control = input();
    input_control.setData_provider_optim(provider);

    output_diagnostics outDiag = output_diagnostics();
    outDiag.setData_provider_optim(provider);

    equation_solving_controller model_solver = equation_solving_controller();
    model_solver.setData_provider_optim(provider);

    gradient_calculator gradient_calculator_opt = gradient_calculator(filename);
    objective_calculator objective = objective_calculator(filename);
    output_control_update outController = output_control_update(filename);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    std::map<std::string,std::string> paths = provider.getPaths();

    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    double pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);

    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::vector<particle>> forwardParticles_initialControl(ntimesteps_gp);
    std::unordered_map<coordinate_phase_space_time,double> forwardPDF_map;
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF;
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF_initial;

    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);
    std::unordered_map<coordinate_phase_space_time,double> backwardPDF_map;
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

    arma::mat control0(pcell_gp,3,arma::fill::ones);
    control0 = -10.0*control0;
    arma::mat gradient(pcell_gp,3,arma::fill::zeros);

    outController.writeControl_XML(control0);
    outDiag.writeDoubleToFile(arma::norm(control0,"fro"),"normControlTrack");
    outController.interpolate_control(provider);


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
    input_control.read_plasma_state_forward(forwardParticles);

    logger::Info("Finished reading files...");
    logger::Info("Starting VSTRAP (backward)...");

    backward_return = model_solver.start_solving_backward(START_VSTRAP_BACKWARD);
    if (backward_return != 0)  {
        logger::Info("Backward VSTRAP returned non-zero value: " + std::to_string(backward_return));
        throw std::system_error();
    }

    logger::Info("Reading particle files...");
    input_control.read_plasma_state_backward(backwardParticles);


    logger::Info("Assembling pdfs...");

    assembling_flag = pdf_control.assemblingMultiDim_parallel(forwardParticles,0,pdf_time);
    forwardPDF = pdf_time;
    assembling_flag = pdf_control.assemblingMultiDim_parallel(backwardParticles,1,pdf_time);
    backwardPDF = pdf_time;

    double functional_value0 = objective.calculate_objective_L2(forwardPDF,control0);


    logger::Info("Building gradient...");
    arma::mat gradient0 = gradient_calculator_opt.calculateGradient_forceControl_space_Hm(forwardPDF,backwardPDF,control0);
    outDiag.writeDoubleToFile(arma::norm(gradient,"fro"),"normGradientTrack");

    arma::mat delta_control(pcell_gp,3,arma::fill::ones);
    double t = 10.0;

    int iteration_number = 4;

    std::vector<double> functional_values(iteration_number,0.0);
    std::vector<double> difference(iteration_number,0.0);
    arma::mat control_temp;

    for(int i = 0; i<iteration_number; i++) {
        control_temp = control0 + pow(0.25,i+1)*t*delta_control;

        logger::Info("Starting VSTRAP (foward)... ");
        forward_return = model_solver.start_solving_forward(START_VSTRAP_FORWARD);
        if (forward_return != 0) {
            logger::Info("Forward VSTRAP returned non-zero value: " + std::to_string(forward_return));
            throw  std::system_error();
        }
        logger::Info("Finished VSTRAP... Reading particle files");
        input_control.read_plasma_state_forward(forwardParticles);

        assembling_flag = pdf_control.assemblingMultiDim_parallel(forwardParticles,0,pdf_time);
        forwardPDF = pdf_time;

        functional_values[i] = objective.calculate_objective_L2(forwardPDF,control_temp);
        std::cout << std::to_string(functional_values[i]) << std::endl;
        difference[i] = (functional_values[i]-functional_value0)-pow(0.25,i+1)*t*(arma::dot(gradient0.col(0),delta_control.col(0))
                                                                                  + arma::dot(gradient0.col(1),delta_control.col(1))
                                                                                  + arma::dot(gradient0.col(2),delta_control.col(2)));
    }

    std::cout << "Difference: " << std::endl;
    for(int i = 0; i< iteration_number; i++) {
        std::cout << std::to_string(difference[i]) << std::endl;
    }


}

TEST(gradient,calculationNR1) {

    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  filename = input_directory.c_str();

    data_provider provider = data_provider(filename);

    std::map<std::string, double> optimizationParameters = provider.getOptimizationParameters();
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    gradient_calculator gradient_calculator_opt = gradient_calculator(filename);
    input input_control = input();
    input_control.setData_provider_optim(provider);
    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(provider);

    arma::mat gradient(dimensionOfControl_gp,3,arma::fill::zeros);

    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF;
    std::vector<std::vector<particle>> backwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> backwardPDF;

    std::vector<std::unordered_map<coordinate_phase_space_time, double>> pdf_time(ntimesteps_gp);
    int assembling_flag_forward;
    int assembling_flag_backward;

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        forwardParticles[o-1] = input::readParticleVector("./data/vstrap_test_output/plasma_state_batch_1_forward_particles_CPU_"+std::to_string(o)+".csv",",");
    }

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        backwardParticles[ntimesteps_gp - o] = input::readParticleVector("./data/vstrap_test_output/plasma_state_batch_1_adjoint_particles_CPU_"+std::to_string(o)+".csv",",");
    }

    assembling_flag_forward = pdf_control.assemblingMultiDim_parallel(forwardParticles,0,pdf_time);
    forwardPDF = pdf_time;
    assembling_flag_backward = pdf_control.assemblingMultiDim_parallel(backwardParticles,1,pdf_time);
    backwardPDF = pdf_time;


    arma::mat control(dimensionOfControl_gp,dimensionOfControl_gp,arma::fill::zeros);

    try {
        gradient = gradient_calculator_opt.calculateGradient_forceControl_space_Hm(forwardPDF,backwardPDF,control);
    } catch (std::exception e)  {
        std::cout << e.what() << std::endl;
    }
    EXPECT_NO_THROW();
}

