#include "stepsize_controller.h"

stepsize_controller::stepsize_controller(const char *filename)
{
    this->setData_provider_optim(data_provider(filename));

}

int stepsize_controller::calculate_stepsize(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection, std::vector<particle> &inputParticles, double &stepsize0)
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

int stepsize_controller::armijo_linesearch(arma::mat &gradient, double J0, arma::mat &control, arma::mat &stepdirection,
                                           std::vector<particle> &inputParticles, double &stepsize0)
{
    int return_flag = 0;

    output_control_update outController = output_control_update();
    outController.setData_provider_optim(this->getData_provider_optim());
    objective_calculator objective = objective_calculator();
    objective.setData_provider_optim(this->getData_provider_optim());
    output_diagnostics outDiag = output_diagnostics();
    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(this->getData_provider_optim());
    input input_control = input();
    input_control.setData_provider_optim(this->getData_provider_optim());

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
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;


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
    arma::mat control0 = control;

    //std::vector<particle> inputParticles = dsmc.createInitialDistribution_Gauss_positionVelocity(1.0,0.0);


    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::vector<particle>> forwardParticles0(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF_time;

    double wasserstein_distance;

    outController.writeControl_XML(control);
    std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " + DOMAIN_MESH +
            " " + PATH_TO_SHARED_FILES + "control_field_cells.xml" + " " + PATH_TO_SHARED_FILES + "interpolated_control_field.xml";
    system(&interpolating_control_python[0]);

    int forward_return = system(&START_VSTRAP_FORWARD[0]);

    if (forward_return == 0) {
        /*for(unsigned int k = 1; k<=ntimesteps_gp; k++) {
            forwardParticles0[k-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_batch_1_forward_particles_CPU_"+std::to_string(k)+".csv",",");
        }*/
        input_control.read_plasma_state_forward(forwardParticles0);
    }


    /*
     * Generate new control
     */

    control = control0 + alpha*stepdirection;
    outController.writeControl_XML(control);
    system(&interpolating_control_python[0]);

    forward_return = system(&START_VSTRAP_FORWARD[0]);


    if (forward_return == 0) {
        /*for(unsigned int k = 1; k<=ntimesteps_gp; k++) {
            forwardParticles[k-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_batch_1_forward_particles_CPU_"+std::to_string(k)+".csv",",");
        }*/
        input_control.read_plasma_state_forward(forwardParticles);
        forwardPDF_time = pdf_control.assemblingMultiDim_parallel(forwardParticles,0);
        Jtemp = objective.calculate_objective_L2(forwardPDF_time,control0 + alpha*stepdirection);
    }

    while (Jtemp > J0 + scalarProduct*armijo_descent_fraction && alpha > tolerance
           && counter <= optimizationIteration_max_gp) {
        alpha = pow(1.0,counter)*0.5*alpha;

        control = control0 + alpha*stepdirection;
        outController.writeControl_XML(control0 + alpha*stepdirection);
        system(&interpolating_control_python[0]);

        forward_return = system(&START_VSTRAP_FORWARD[0]);

        if (forward_return == 0) {
            /*for(unsigned int k = 1; k<=ntimesteps_gp; k++) {
                forwardParticles[k-1] = input::readParticleVector(BUILD_DIRECTORY_OPTIM+"plasma_state_batch_1_forward_particles_CPU_"+std::to_string(k)+".csv",",");
            }*/
            input_control.read_plasma_state_forward(forwardParticles);
            forwardPDF_time = pdf_control.assemblingMultiDim_parallel(forwardParticles,0);
            Jtemp = objective.calculate_objective_L2(forwardPDF_time,control0 + alpha*stepdirection);
        }

        std::cout << "Armijo: " << "Jtemp = " << Jtemp << std::endl
                  << "J0 + scalarProduct*armijo_descent_fraction = " << J0 + scalarProduct*armijo_descent_fraction << std::endl
                  << "Stepsize " << alpha << " in " << counter << ". iteration" << std::endl;
        counter++;
        wasserstein_distance = pdf_control.calculate_wasserstein_metric(forwardParticles0,forwardParticles);
        std::cout << "Wasserstein distance: " << wasserstein_distance << std::endl;
    }

    if (alpha < tolerance) {
        //Calculate with old control
        control = control0;
        outController.writeControl_XML(control0);
        system(&interpolating_control_python[0]);

        forward_return = system(&START_VSTRAP_FORWARD[0]);
        std::cout << "Minimum already reached. You may want to decrease your tolerance? (Was " << tolerance << ")" << std::endl;
        return_flag = 1;
        return return_flag;
    } else if (counter > optimizationIteration_max_gp) {
        std::cout << "Maximum interation depth ( " << optimizationIteration_max_gp << " ) reached without decrease!" << std::endl;
        return_flag = 2;
        return return_flag;
    } else {
        std::cout << "Armijo-linesearch found stepsize " << alpha << " after " << counter << " iterations." << std::endl;
    }
    wasserstein_distance = pdf_control.calculate_wasserstein_metric(forwardParticles0,forwardParticles);
    std::cout << "Wasserstein distance: " << wasserstein_distance << std::endl;
    outDiag.writeDoubleToFile(alpha,"stepsizeTrack");
    outDiag.writeDoubleToFile(wasserstein_distance,"wassersteinDistanceTrack");

    stepsize0 = alpha;

    return return_flag;
}

int stepsize_controller::gradient_descent(arma::mat &control, arma::mat &stepdirection, std::vector<particle> &nputParticles, double &stepsize)
{

    int return_flag = 0;
    double stepsize_gradient = static_cast<double>(this->getData_provider_optim().getOptimizationParameters().find("fixed_gradient_descent_stepsize")->second);
    control = control + stepsize_gradient*stepdirection;

    return return_flag;
}
