#include "stepdirectioncontroller.h"

StepdirectionController::StepdirectionController(const char *filename)
{
    this->set_DataProviderOptim(DataProvider(filename));

}

arma::mat StepdirectionController::GetStepdirection(arma::mat &gradient, arma::mat gradient_old, arma::mat &stepdirection_old, unsigned int &optimization_iteration)
{

    std::map<std::string,std::string> subroutines = this->get_DataProviderOptim().getSubroutines();
    std::map<std::string,double> optim_paramters = this->get_DataProviderOptim().getOptimizationParameters();
    std::string direction_update = "";

    try{
        direction_update =  subroutines.find("direction_update")->second;
    } catch(std::exception e) {
        logger::Info("No Direction_update found");
        logger::Info(e.what());
    }

    if(optimization_iteration == 1) {
        logger::Info("First iteration, using negative gradient as stepdirection");
        return FixedGradientDescent(gradient,optimization_iteration);
    } else {
        if(direction_update.compare("negative_gradient")==0) {
            logger::Info("Updating control using stepdirection update: Negative Gradient");
            return FixedGradientDescent(gradient,optimization_iteration);
        } else if (direction_update.compare("ncg_FR")==0) {
            logger::Info("Updating control using stepdirection update: NCG using Fletcher-Reeves formula");
            return NcgSchemeFR(gradient, gradient_old, stepdirection_old, optimization_iteration);
        } else if (direction_update.compare("ncg_PR")==0) {
            logger::Info("Updating control using stepdirection update: NCG using Fletcher-Reeves formula");
            return NcgSchemePR(gradient, gradient_old, stepdirection_old, optimization_iteration);
        } else if (direction_update.compare("ncg_HZ")==0) {
            logger::Info("Updating control using stepdirection update: NCG using Fletcher-Reeves formula");
            return NcgSchemeHZ(gradient, gradient_old, stepdirection_old, optimization_iteration);
        }
        else {
            std::invalid_argument("No such direction update subroutine");
            throw std::runtime_error("No such direction update subroutine");
        }
    }
}


arma::mat StepdirectionController::FixedGradientDescent(arma::mat gradient, unsigned int optimization_iteration)
{
    return -gradient;
}

arma::mat StepdirectionController::NcgSchemeFR(arma::mat &gradient, arma::mat &gradient_old, arma::mat &stepdirection_old, unsigned int &optimization_iteration)
{
    arma::mat stepdirection_new;
    InnerProducts pro = InnerProducts();
    pro.set_DataProviderOptim(this->get_DataProviderOptim());

    //double beta = arma::dot(gradient,gradient)/arma::dot(gradient_old,gradient_old);
    double beta = pro.H1InnerProduct(gradient,gradient)/pro.H2InnerProduct(gradient_old,gradient_old);

    stepdirection_new = -gradient + beta*stepdirection_old;

    return stepdirection_new;
}

arma::mat StepdirectionController::NcgSchemePR(arma::mat &gradient, arma::mat &gradient_old, arma::mat &stepdirection_old, unsigned int &optimization_iteration)
{
    arma::mat stepdirection_new;
    double beta = arma::dot(gradient, gradient - gradient_old)/arma::dot(gradient_old,gradient_old);

    stepdirection_new = -gradient + beta*stepdirection_old;

    return stepdirection_new;
}

arma::mat StepdirectionController::NcgSchemeHZ(arma::mat &gradient, arma::mat &gradient_old, arma::mat &stepdirection_old, unsigned int &optimization_iteration)
{
    arma::mat stepdirection_new;
    arma::mat y = gradient-gradient_old;
    double beta = arma::dot(y - 2.0*stepdirection_old*arma::dot(y,y)/arma::dot(stepdirection_old,y),gradient/arma::dot(stepdirection_old,y));

    stepdirection_new = -gradient + beta*stepdirection_old;

    return stepdirection_new;
}
