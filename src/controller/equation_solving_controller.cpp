#include "equation_solving_controller.h"

equation_solving_controller::equation_solving_controller()
{

}

int equation_solving_controller::start_solving_forward(std::string start_forward)
{
    return system(&start_forward[0]);
}

int equation_solving_controller::start_solving_backward(std::string start_backward)
{
    return  system(&start_backward[0]);
}

arma::mat equation_solving_controller::Laplacian_3D()
{
    unsigned int dimesion = 32;

    arma::mat Laplace(dimesion,dimesion,arma::fill::zeros);

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    std::map<int,std::vector<double>> barycenters = this->getData_provider_optim().getMesh_barycenters();



    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int vcell_gp = static_cast<unsigned int>(optimizationParameters.find("vcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    double dv_gp = static_cast<double>(optimizationParameters.find("dv_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double weight_control_gp = static_cast<double>(optimizationParameters.find("weight_control_gp")->second);

    std::vector<double> current_barycenter;
    std::vector<double> next_cell_xm; std::vector<double> next_cell_xp;
    std::vector<double> next_cell_ym;std::vector<double> next_cell_yp;
    std::vector<double> next_cell_zm;std::vector<double> next_cell_zp;
    int cell_id;

    arma::mat gradient(pcell_gp,3,arma::fill::zeros);

    for(int i = 17; i<=48; i++) {
        current_barycenter = barycenters.find(static_cast<int>(i))->second;
        Laplace(i-17,i-17) = -6.0;

        next_cell_xm = current_barycenter;
        next_cell_xp = current_barycenter;
        next_cell_ym = current_barycenter;
        next_cell_yp = current_barycenter;
        next_cell_zm = current_barycenter;
        next_cell_zp = current_barycenter;

        if (std::abs(current_barycenter[0]-0.25)<0.5) {
            next_cell_xm[0] = current_barycenter[0]-0.25;
        }
        if (std::abs(current_barycenter[0]+0.25)<0.5) {
            next_cell_xp[0] = current_barycenter[0]+0.25;
        }
        if (std::abs(current_barycenter[1]-0.25)<0.5) {
            next_cell_ym[1] = current_barycenter[1]-0.25;
        }
        if (std::abs(current_barycenter[1]+0.25)<0.5) {
            next_cell_yp[1] = current_barycenter[1]+0.25;
        }
        if (std::abs(current_barycenter[2]-0.25)<0.5) {
            next_cell_zm[2] = current_barycenter[2]-0.25;
        }
        if (std::abs(current_barycenter[2]+0.25)<0.5) {
            next_cell_zp[2] = current_barycenter[2]+0.25;
        }

        for(int l = 17; l<=48;l++) {
            std::vector<double> temp = barycenters.find(l)->second;
            if (temp == next_cell_xm || temp == next_cell_xp || temp == next_cell_ym || temp == next_cell_yp || temp == next_cell_zm || temp == next_cell_zp) {
                if (i != l) {
                    Laplace(i-17,l-17) = 1.0;
                }
            }
        }
    }

    return Laplace;
}

arma::mat equation_solving_controller::Laplacian_Squared_3D()
{
    unsigned int dimesion = 32;

    arma::mat Laplace(dimesion,dimesion,arma::fill::zeros);

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    std::map<int,std::vector<double>> barycenters = this->getData_provider_optim().getMesh_barycenters();



    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int vcell_gp = static_cast<unsigned int>(optimizationParameters.find("vcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    double dv_gp = static_cast<double>(optimizationParameters.find("dv_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double weight_control_gp = static_cast<double>(optimizationParameters.find("weight_control_gp")->second);

    std::vector<double> current_barycenter;
    std::vector<double> next_cell_xm; std::vector<double> next_cell_xp;
    std::vector<double> next_cell_ym;std::vector<double> next_cell_yp;
    std::vector<double> next_cell_zm;std::vector<double> next_cell_zp;

    std::vector<double> next_cell_xmm; std::vector<double> next_cell_xpp;
    std::vector<double> next_cell_ymm;std::vector<double> next_cell_ypp;
    std::vector<double> next_cell_zmm;std::vector<double> next_cell_zpp;


    arma::mat gradient(pcell_gp,3,arma::fill::zeros);

    for(int i = 17; i<=48; i++) {
        current_barycenter = barycenters.find(static_cast<int>(i))->second;
        Laplace(i-17,i-17) = 18.0;

        next_cell_xm = current_barycenter;
        next_cell_xp = current_barycenter;
        next_cell_ym = current_barycenter;
        next_cell_yp = current_barycenter;
        next_cell_zm = current_barycenter;
        next_cell_zp = current_barycenter;

        next_cell_xmm = current_barycenter;
        next_cell_xpp = current_barycenter;
        next_cell_ymm = current_barycenter;
        next_cell_ypp = current_barycenter;
        next_cell_zmm = current_barycenter;
        next_cell_zpp = current_barycenter;

        if (std::abs(current_barycenter[0]-0.25)<0.5) {
            next_cell_xm[0] = current_barycenter[0]-0.25;
        }
        if (std::abs(current_barycenter[0]+0.25)<0.5) {
            next_cell_xp[0] = current_barycenter[0]+0.25;
        }
        if (std::abs(current_barycenter[1]-0.25)<0.5) {
            next_cell_ym[1] = current_barycenter[1]-0.25;
        }
        if (std::abs(current_barycenter[1]+0.25)<0.5) {
            next_cell_yp[1] = current_barycenter[1]+0.25;
        }
        if (std::abs(current_barycenter[2]-0.25)<0.5) {
            next_cell_zm[2] = current_barycenter[2]-0.25;
        }
        if (std::abs(current_barycenter[2]+0.25)<0.5) {
            next_cell_zp[2] = current_barycenter[2]+0.25;
        }

        if (std::abs(current_barycenter[0]-0.5)<0.5) {
            next_cell_xmm[0] = current_barycenter[0]-0.5;
        }
        if (std::abs(current_barycenter[0]+0.5)<0.5) {
            next_cell_xpp[0] = current_barycenter[0]+0.5;
        }
        if (std::abs(current_barycenter[1]-0.5)<0.5) {
            next_cell_ymm[1] = current_barycenter[1]-0.5;
        }
        if (std::abs(current_barycenter[1]+0.5)<0.5) {
            next_cell_ypp[1] = current_barycenter[1]+0.5;
        }
        if (std::abs(current_barycenter[2]-0.5)<0.5) {
            next_cell_zmm[2] = current_barycenter[2]-0.5;
        }
        if (std::abs(current_barycenter[2]+0.5)<0.5) {
            next_cell_zpp[2] = current_barycenter[2]+0.5;
        }

        for(int l = 17; l<=48;l++) {
            std::vector<double> temp = barycenters.find(l)->second;
            if (temp == next_cell_xm || temp == next_cell_xp || temp == next_cell_ym || temp == next_cell_yp || temp == next_cell_zm || temp == next_cell_zp) {
                if (i != l) {
                    Laplace(i-17,l-17) = -4.0;
                }
            }
            if (temp == next_cell_xmm || temp == next_cell_xpp || temp == next_cell_ymm || temp == next_cell_ypp || temp == next_cell_zmm || temp == next_cell_zpp) {
                if (i != l) {
                    Laplace(i-17,l-17) = -1.0;
                }
            }
        }
    }

    return Laplace;


}


