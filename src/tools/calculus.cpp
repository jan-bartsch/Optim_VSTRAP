#include "calculus.h"

calculus::calculus() { }

double calculus::divergence_vector(arma::mat input)
{
    data_provider optim_provider = this->getData_provider_optim();
    std::map<std::string,double> parameters = optim_provider.getOptimizationParameters();

    double small_discr_sidelength = static_cast<double>(parameters.find("small_discr_sidelength")->second);
    double small_discr_volume = static_cast<double>(parameters.find("small_discr_volume")->second);

    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(optim_provider);

    arma::mat D1X1 = solver.D1X1_second_order();
    arma::mat D1X2 = solver.D1X2_second_order();
    arma::mat D1X3 = solver.D1X3_second_order();

    arma::vec control_x1 = D1X1*input.col(0);
    arma::vec control_x2 = D1X2*input.col(1);
    arma::vec control_x3 = D1X3*input.col(2);

    double div = 0.0;

    div += arma::accu(control_x1)/small_discr_sidelength;
    div += arma::accu(control_x2)/small_discr_sidelength;
    div += arma::accu(control_x3)/small_discr_sidelength;

    div *= (small_discr_sidelength*small_discr_sidelength*small_discr_sidelength);

    return div;
}
