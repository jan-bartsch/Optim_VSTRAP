#include "linear_algebra.h"

linear_algebra::linear_algebra() { }

double linear_algebra::divergence_vector(arma::mat input)
{
    data_provider optim_provider = this->getData_provider_optim();
    std::map<std::string,double> parameters = optim_provider.getOptimizationParameters();

    double db_gp = static_cast<double>(parameters.find("db_gp")->second);
    double dp_gp = static_cast<double>(parameters.find("dp_gp")->second);

    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(optim_provider);

    arma::mat D1X1 = solver.D1X1_second_order();
    arma::mat D1X2 = solver.D1X2_second_order();
    arma::mat D1X3 = solver.D1X3_second_order();

    arma::vec control_x1 = D1X1*input.col(0);
    arma::vec control_x2 = D1X2*input.col(1);
    arma::vec control_x3 = D1X3*input.col(2);

    double div = 0.0;

    div += arma::accu(control_x1)/db_gp;
    div += arma::accu(control_x2)/db_gp;
    div += arma::accu(control_x3)/db_gp;

    div *= (db_gp*db_gp*db_gp);

    return div;
}
