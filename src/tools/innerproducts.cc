#include "innerproducts.h"

InnerProducts::InnerProducts() { }

double InnerProducts::L2InnerProduct(arma::mat m1, arma::mat m2)
{

    double small_discr_volume = static_cast<double>(this->get_DataProviderOptim().getOptimizationParameters().find("small_discr_volume")->second);

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
        product += arma::dot(m1.col(i),m2.col(i))*small_discr_volume;
    }

    return product;
}

double InnerProducts::H1InnerProduct(arma::mat m1, arma::mat m2)
{
    EquationSolvingController solver = EquationSolvingController();
    solver.set_DataProviderOptim(this->get_DataProviderOptim());

    arma::mat D1 = solver.D1SecondOrder();

    std::map<std::string,double> optimizationParameters = this->get_DataProviderOptim().getOptimizationParameters();

    double small_discr_volume = static_cast<double>(optimizationParameters.find("small_discr_volume")->second);
    double small_discr_sidelength = static_cast<double>(optimizationParameters.find("small_discr_sidelength")->second);
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
    product += L2InnerProduct(m1,m2);


    /*
     * first derivative
     */
    arma::mat D1mat1 = D1*m1.rows(start_control-1,end_control-1)/(2.0*small_discr_sidelength);
    arma::mat D1mat2 = D1*m2.rows(start_control-1,end_control-1)/(2.0*small_discr_sidelength);

    for(int i = 0; i < D1mat1.n_cols; i++) {
        std::cout << arma::dot(D1mat1.col(i),D1mat2.col(i))*small_discr_volume << std::endl;
        product += arma::dot(D1mat1.col(i),D1mat2.col(i))*small_discr_volume;
    }

    return product;
}

double InnerProducts::H2InnerProduct(arma::mat m1, arma::mat m2)
{
    EquationSolvingController solver = EquationSolvingController();
    solver.set_DataProviderOptim(this->get_DataProviderOptim());

    arma::mat D2 = solver.Laplacian3D();

    std::map<std::string,double> optimizationParameters = this->get_DataProviderOptim().getOptimizationParameters();

    double small_discr_volume = static_cast<double>(optimizationParameters.find("small_discr_volume")->second);
    double small_discr_sidelength = static_cast<double>(optimizationParameters.find("small_discr_sidelength")->second);
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
    product += H1InnerProduct(m1,m2);

    /*
     * second derivative
     */
    arma::mat D2mat1 = D2*m1.rows(start_control-1,end_control-1)/(small_discr_sidelength*small_discr_sidelength);
    arma::mat D2mat2 = D2*m2.rows(start_control-1,end_control-1)/(small_discr_sidelength*small_discr_sidelength);


    for(int i = 0; i < m1.n_cols; i++) {
        product += arma::dot(D2mat1.col(i),D2mat2.col(i))*small_discr_sidelength;
    }

    return product;
}
