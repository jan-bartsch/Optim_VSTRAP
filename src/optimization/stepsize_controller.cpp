#include "stepsize_controller.h"

stepsize_controller::stepsize_controller(const char *filename)
{
    this->setData_provider_optim(data_provider(filename));

}

double stepsize_controller::get_stepsize(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles, double stepsize0)
{
    std::map<std::string,std::string> subroutines = this->getData_provider_optim().getSubroutines();
    std::string control_update = subroutines.find("control_update")->second;

    if(control_update.compare("armijo_linesearch")==0) {
        return armijo_linesearch(gradient,J0,control,stepdirection,inputParticles,stepsize0);
    } else if (control_update.compare("gradient_descent")==0) {
        return gradient_descent(control,stepdirection,inputParticles,stepsize0);
    } else {
        std::invalid_argument("No such control update subroutine");
        return 1;
    }
}

double stepsize_controller::armijo_linesearch(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles, double stepsize0)
{
    output_control_update outController = output_control_update();
    outController.setData_provider_optim(this->getData_provider_optim());
    objective_calculator objective = objective_calculator();
    objective.setData_provider_optim(this->getData_provider_optim());
    output_diagnostics outDiag = output_diagnostics();
    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(this->getData_provider_optim());

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    std::map<std::string, std::string> paths = this->getData_provider_optim().getPaths();

    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    unsigned int optimizationIteration_max_gp = static_cast<unsigned int>(optimizationParameters.find("optimizationIteration_max_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double armijo_descent_fraction = static_cast<double>(optimizationParameters.find("armijo_descent_fraction")->second);
    double tolerance = static_cast<double>(optimizationParameters.find("tolerance_gp")->second);

    std::string BUILD_DIRECTORY_VSTRAP = paths.find("BUILD_DIRECTORY_VSTRAP")->second;
    std::string BUILD_DIRECTORY_OPTIM = paths.find("BUILD_DIRECTORY_OPTIM")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;

    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;

    std::string START_VSTRAP_FORWARD = BUILD_DIRECTORY_VSTRAP + "vstrap" + " " + PATH_TO_SHARED_FILES + "input_forward.xml";


    double scalarProduct = (arma::dot(gradient.col(0),stepdirection.col(0)) + arma::dot(gradient.col(1),stepdirection.col(1)))*dt_gp;

    std::cout << "scalarProduct: " << scalarProduct << std::endl;
    std::cout << "Stepdirection: " << stepdirection << std::endl;

    if (scalarProduct>0) {
        std::cout << "No descent direction!" << std::endl;
    }

    unsigned int counter = 0;
    double Jtemp = std::numeric_limits<double>::max();


    double alpha = stepsize0; //stepsize

    //std::vector<particle> inputParticles = dsmc.createInitialDistribution_Gauss_positionVelocity(1.0,0.0);


    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::unordered_map<coordinate_phase_space_time,double> forwardPDF;

    /*
     * Generate new control
     */

    control = control + alpha*stepdirection;
    outController.writeControl_XML(control);
    outDiag.writeDoubleToFile(arma::norm(control,"fro"),"normControlTrack");
    std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + PATH_TO_SHARED_FILES + "box_coarse.xml" +
            " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field.xml";
    system(&interpolating_control_python[0]);

    int forward_return = system(&START_VSTRAP_FORWARD[0]);

    if (forward_return == 0) {
        for(unsigned int k = 1; k<=ntimesteps_gp; k++) {
            forwardParticles[k-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_batch_1_forward_particles_CPU_"+std::to_string(k)+".csv",",");
        }
        forwardPDF = pdf_control.assemblingMultiDim(forwardParticles,0);
        Jtemp = objective.calculate_objective_L2(forwardPDF,control+alpha*stepdirection);
    }

    while (Jtemp > J0 + scalarProduct*armijo_descent_fraction && alpha > tolerance
           && counter <= optimizationIteration_max_gp) {
        alpha = pow(1.0,counter)*0.5*alpha;

        control = control + alpha*stepdirection;
        outController.writeControl_XML(control);
        outDiag.writeDoubleToFile(arma::norm(control,"fro"),"normControlTrack");
        std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + PATH_TO_SHARED_FILES + "box_coarse.xml" +
                " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field.xml";
        system(&interpolating_control_python[0]);

        forward_return = system(&START_VSTRAP_FORWARD[0]);

        if (forward_return == 0) {
            for(unsigned int k = 1; k<=ntimesteps_gp; k++) {
                forwardParticles[k-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_batch_1_forward_particles_CPU_"+std::to_string(k)+".csv",",");
            }
            forwardPDF = pdf_control.assemblingMultiDim(forwardParticles,0);
            Jtemp = objective.calculate_objective_L2(forwardPDF,control+alpha*stepdirection);
        }

        std::cout << "Armijo: " << "Jtemp = " << Jtemp << std::endl
                  << "J0 + scalarProduct*armijo_descent_fraction = " << J0 + scalarProduct*armijo_descent_fraction << std::endl;
        counter++;
    }

    if (alpha < tolerance) {
        std::cout << "Minimum already reached. You may want to decrease your tolerance?" << std::endl;
        std::system_error();
    } else if (counter > optimizationIteration_max_gp) {
        std::cout << "Maximum interation depth reached without decrease!" << std::endl;
        std::system_error();
    } else {
        std::cout << "Armijo-linesearch found stepsize " << alpha << " after " << counter << " iterations." << std::endl;
    }
    return alpha;
}

double stepsize_controller::gradient_descent(arma::mat &control, arma::mat &stepdirection, std::vector<particle> &nputParticles, double stepsize)
{

    return static_cast<double>(this->getData_provider_optim().getOptimizationParameters().find("fixed_gradient_descent_stepsize")->second);

}
