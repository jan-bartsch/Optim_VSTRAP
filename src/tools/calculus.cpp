#include "calculus.h"

Calculus::Calculus() { }

double Calculus::DivergenceVector(arma::mat &Input)
{
    DataProvider optim_provider = this->get_DataProviderOptim();
    std::map<std::string,double> parameters = optim_provider.getOptimizationParameters();

    double small_discr_sidelength = static_cast<double>(parameters.find("small_discr_sidelength")->second);
    double small_discr_volume = static_cast<double>(parameters.find("small_discr_volume")->second);

    EquationSolvingController solver = EquationSolvingController();
    solver.set_DataProviderOptim(optim_provider);

    arma::mat D1X1 = solver.D1x1SecondOrder();
    arma::mat D1X2 = solver.D1x2SecondOrder();
    arma::mat D1X3 = solver.D1x3SecondOrder();

    arma::vec control_x1 = D1X1*Input.col(0);
    arma::vec control_x2 = D1X2*Input.col(1);
    arma::vec control_x3 = D1X3*Input.col(2);

    double div = 0.0;

    div += arma::accu(control_x1)/small_discr_sidelength;
    div += arma::accu(control_x2)/small_discr_sidelength;
    div += arma::accu(control_x3)/small_discr_sidelength;

    div *= (small_discr_sidelength*small_discr_sidelength*small_discr_sidelength);

    return div;
}
