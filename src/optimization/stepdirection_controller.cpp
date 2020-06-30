#include "stepdirection_controller.h"

stepdirection_controller::stepdirection_controller(const char *filename)
{
    this->setData_provider_optim(data_provider(filename));

}

arma::mat stepdirection_controller::get_stepdirection(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration)
{
    std::map<std::string,std::string> subroutines = this->getData_provider_optim().getSubroutines();
    std::string control_update = subroutines.find("direction_update")->second;

    if(optimization_iteration == 1) {
        logger::Info("First iteration, using negative gradient as stepdirection");
        return fixed_gradient_descent(gradient,optimization_iteration);
    } else {
        if(control_update.compare("negative_gradient")==0) {
            logger::Info("Updating control using stepdirection update: Negative Gradient");
            return fixed_gradient_descent(gradient,optimization_iteration);
        } else if (control_update.compare("ncg_FR")==0) {
            logger::Info("Updating control using stepdirection update: NCG using Fletcher-Reeves formula");
            return ncg_scheme_FR(gradient, gradient_old, stepdirectionOld, optimization_iteration);
        }
        else {
            std::invalid_argument("No such direction update subroutine");
            throw std::runtime_error("No such direction update subroutine");
        }
    }
}


arma::mat stepdirection_controller::fixed_gradient_descent(arma::mat gradient, unsigned int optimization_iteration)
{
    return -gradient;
}

arma::mat stepdirection_controller::ncg_scheme_FR(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration)
{
    arma::mat stepdirection_new;
    double beta = arma::dot(gradient,gradient)/arma::dot(gradient_old,gradient_old);

    stepdirection_new = -gradient + beta*stepdirectionOld;

    return stepdirection_new;
}

arma::mat stepdirection_controller::ncg_scheme_PR(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration)
{
    arma::mat stepdirection_new;
    double beta = arma::dot(gradient, gradient - gradient_old)/arma::dot(gradient_old,gradient_old);

    stepdirection_new = -gradient + beta*stepdirectionOld;

    return stepdirection_new;
}

arma::mat stepdirection_controller::ncg_scheme_HZ(arma::mat gradient, arma::mat gradient_old, arma::mat stepdirectionOld, unsigned int optimization_iteration)
{
    arma::mat stepdirection_new;
    arma::mat y = gradient-gradient_old;
    double beta = arma::dot(y - 2.0*stepdirectionOld*arma::dot(y,y)/arma::dot(stepdirectionOld,y),gradient/arma::dot(stepdirectionOld,y));

    stepdirection_new = -gradient + beta*stepdirectionOld;

    return stepdirection_new;
}
