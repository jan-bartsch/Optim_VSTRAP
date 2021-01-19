#include "inner_products.h"

inner_products::inner_products() { }

double inner_products::L2_inner_product(arma::mat m1, arma::mat m2)
{

    double dp_gp = static_cast<double>(this->getData_provider_optim().getOptimizationParameters().find("dp_gp")->second);

    double product = 0.0;

    const arma::SizeMat size1 = arma::size(m1);
    const arma::SizeMat size2 = arma::size(m2);

    if (size1.n_cols != size2.n_cols || size1.n_rows != size2.n_rows) {
        std::cout << m1 << std::endl;
        std::cout << m2 << std::endl;
        throw std::invalid_argument("Objects in inner product calculation do not have proper size");
    }

    int columns = static_cast<int>(size1.n_cols);

    for(int i = 0; i < columns; i++) {
        product += arma::dot(m1.col(i),m2.col(i))*dp_gp;
    }

    return product;
}

double inner_products::H1_inner_product(arma::mat m1, arma::mat m2)
{
    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(this->getData_provider_optim());

    arma::mat D1 = solver.D1_second_order();

    std::map<std::string,double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();

    double dp_gp = static_cast<double>(optimizationParameters.find("dp_gp")->second);
    double db_gp = static_cast<double>(optimizationParameters.find("db_gp")->second);
    int start_control = static_cast<int>(optimizationParameters.find("start_control_gp")->second);
    int end_control = static_cast<int>(optimizationParameters.find("end_control_gp")->second);

    double product = 0.0;

    const arma::SizeMat size1 = arma::size(m1);
    const arma::SizeMat size2 = arma::size(m2);

    if (size1.n_cols != size2.n_cols || size1.n_rows != size2.n_rows) {
        std::cout << m1 << std::endl;
        std::cout << m2 << std::endl;
        throw std::invalid_argument("Objects in inner product calculation do not have proper size");
    }

    int columns = static_cast<int>(size1.n_cols);

    /**
     * L2 part
     */
    product += L2_inner_product(m1,m2);


    /*
     * first derivative
     */
    arma::mat D1mat1 = D1*m1.rows(start_control-1,end_control-1)/(2.0*db_gp);
    arma::mat D1mat2 = D1*m2.rows(start_control-1,end_control-1)/(2.0*db_gp);

    for(int i = 0; i < D1mat1.n_cols; i++) {
        std::cout << arma::dot(D1mat1.col(i),D1mat2.col(i))*dp_gp << std::endl;
        product += arma::dot(D1mat1.col(i),D1mat2.col(i))*dp_gp;
    }

    return product;
}

double inner_products::H2_inner_product(arma::mat m1, arma::mat m2)
{
    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(this->getData_provider_optim());

    arma::mat D2 = solver.Laplacian_3D();

    std::map<std::string,double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();

    double dp_gp = static_cast<double>(optimizationParameters.find("dp_gp")->second);
    double db_gp = static_cast<double>(optimizationParameters.find("db_gp")->second);
    int start_control = static_cast<int>(optimizationParameters.find("start_control_gp")->second);
    int end_control = static_cast<int>(optimizationParameters.find("end_control_gp")->second);

    double product = 0.0;

    const arma::SizeMat size1 = arma::size(m1);
    const arma::SizeMat size2 = arma::size(m2);

    if (size1.n_cols != size2.n_cols || size1.n_rows != size2.n_rows) {
        std::cout << m1 << std::endl;
        std::cout << m2 << std::endl;
        throw std::invalid_argument("Objects in inner product calculation do not have proper size");
    }

    /**
     * L2 and H1 part
     */
    product += H1_inner_product(m1,m2);

    /*
     * second derivative
     */
    arma::mat D2mat1 = D2*m1.rows(start_control-1,end_control-1)/(db_gp*db_gp);
    arma::mat D2mat2 = D2*m2.rows(start_control-1,end_control-1)/(db_gp*db_gp);


    for(int i = 0; i < m1.n_cols; i++) {
        product += arma::dot(D2mat1.col(i),D2mat2.col(i))*db_gp;
    }

    return product;
}
