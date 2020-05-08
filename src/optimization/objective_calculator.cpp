#include "objective_calculator.h"

objective_calculator::objective_calculator()
{

}

objective_calculator::objective_calculator(const char *filename)
{
    this->setData_provider_optim(data_provider(filename));
}

double objective_calculator::calculate_objective_L2(std::unordered_map<coordinate_phase_space_time,double> forwardPDF, arma::mat control)
{

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();

    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    double pmax_gp = static_cast<double>(optimizationParameters.find("pmax_gp")->second);
    double vmax_gp = static_cast<double>(optimizationParameters.find("vmax_gp")->second);
    unsigned int vcell_gp = static_cast<unsigned int>(optimizationParameters.find("vcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    double dv_gp = static_cast<double>(optimizationParameters.find("dv_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double dp_gp = static_cast<double>(optimizationParameters.find("dp_gp")->second);
    double weight_control_gp = static_cast<double>(optimizationParameters.find("weight_control_gp")->second);
    double C_phi_gp = static_cast<double>(optimizationParameters.find("C_phi_gp")->second);
    double C_theta_gp = static_cast<double>(optimizationParameters.find("C_theta_gp")->second);
    double sigma_x_gp = static_cast<double>(optimizationParameters.find("sigma_x_gp")->second);
    double sigma_v_gp = static_cast<double>(optimizationParameters.find("sigma_v_gp")->second);

    static arma::vec positionDiscr_gp = arma::linspace<arma::vec>(0.0,pmax_gp,pcell_gp+1);
    static arma::vec velocityDiscr_gp = arma::linspace<arma::vec>(-vmax_gp,vmax_gp,vcell_gp);


    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(this->getData_provider_optim());

    desired_trajectory_controller trajectory_controller = desired_trajectory_controller();
    trajectory_controller.setData_provider_optim(this->getData_provider_optim());

    std::map<int,std::vector<double>> baryc = trajectory_controller.getData_provider_optim().getMesh_barycenters();

    //std::cout << baryc.at(0)[0] << std::endl;

    double objective = 0.0;
    double costOfControl = 0.0;

    std::vector<double> objective_time(ntimesteps_gp,0.0);


    /*
    * Add terminal integral
    */

    /*for(unsigned int i = 0; i<pcell_gp; i++)  {
     double current_termPot;
     std::vector<double> p_d;
        for(unsigned int l = 0; l<vcell_gp; l++) {
            for(unsigned int m = 0; m<vcell_gp; m++) {
                for(unsigned int n = 0; n<vcell_gp; n++) {
                    p_d = trajectory_controller.trajectory_desired(i,l,m,n,ntimesteps_gp-1);
                    coordinate_phase_space_time coordinate = coordinate_phase_space_time(i,l,m,n,ntimesteps_gp-1);
                    //termPot[i][l][m][n]
                    current_termPot = - C_phi_gp/(2.0*M_PI*sigma_x_gp*sigma_v_gp)*exp(
                                -(
                                    pow(positionDiscr_gp(i)-p_d[0],2.0)/(2.0*sigma_x_gp*sigma_x_gp)+
                                pow(velocityDiscr_gp(l)-p_d[3],2.0)/(2.0*sigma_v_gp*sigma_v_gp)+
                            pow(velocityDiscr_gp(m)-p_d[4],2.0)/(2.0*sigma_v_gp*sigma_v_gp)+
                            pow(velocityDiscr_gp(n)-p_d[5],2.0)/(2.0*sigma_v_gp*sigma_v_gp)
                            ));
                    if (forwardPDF.find(coordinate) != forwardPDF.end()) {
                        objective += forwardPDF.at(coordinate)*current_termPot*pow(dp_gp,3)*pow(dv_gp,3);

                    }
                }
            }
        }
    }*/

    /*
     * Add tracking integral using first-order(?) RULE
     */

    int numberThreadsTBB = tbb::task_scheduler_init::default_num_threads();
    int usedThreads = numberThreadsTBB;

    if(numberThreadsTBB > static_cast<int>(ntimesteps_gp)) {
        usedThreads = static_cast<int>(ntimesteps_gp);
    }
    tbb::task_scheduler_init init(usedThreads);

    std::cout << "Using " <<  usedThreads << " threads for assembling functional" << std::endl;

    tbb::parallel_for(static_cast<unsigned int> (0), ntimesteps_gp , [&]( unsigned int o ) {
        //for(unsigned int  o = 0; o<ntimesteps_gp; o++) {

        double current_trackPot;
        std::vector<double> p_d;
        float temp;

        std::cout << "Calculating functional in " << o << " timestep" << std::endl;
        for(unsigned int  i = 1; i<=pcell_gp; i++)  {
            for( unsigned int l = 0; l<vcell_gp; l++) {
                for(unsigned int  m = 0; m<vcell_gp; m++) {
                    for(unsigned int n = 0; n<vcell_gp; n++) {
                        std::vector<double> current_barycenter = baryc.find(static_cast<int> (i))->second;
                        p_d = trajectory_controller.trajectory_desired(current_barycenter,l,m,n,o);
                        coordinate_phase_space_time coordinate = coordinate_phase_space_time(i,l,m,n,o);
                        //trackPot[o][i][l][m][n]
                        current_trackPot = - C_theta_gp/(2.0*M_PI*sigma_x_gp*sigma_v_gp)*exp(
                                    -(p_d[0]/(2.0*sigma_x_gp*sigma_x_gp)+
                                    0.0*pow(velocityDiscr_gp(l)-p_d[3],2.0)/(2.0*sigma_v_gp*sigma_v_gp)+
                                0.0*pow(velocityDiscr_gp(m)-p_d[4],2.0)/(2.0*sigma_v_gp*sigma_v_gp)+
                                0.0*pow(velocityDiscr_gp(n)-p_d[5],2.0)/(2.0*sigma_v_gp*sigma_v_gp)
                                ));
//                        temp = static_cast<float>( exp(-(
//                                                           pow(positionDiscr_gp(i)-p_d[0],2.0)/(2.0*sigma_x_gp*sigma_x_gp)+
//                                                       0.0*pow(velocityDiscr_gp(l)-p_d[3],2.0)/(2.0*sigma_v_gp*sigma_v_gp)+
//                                                   0.0*pow(velocityDiscr_gp(m)-p_d[4],2.0)/(2.0*sigma_v_gp*sigma_v_gp)+
//                                0.0*pow(velocityDiscr_gp(n)-p_d[5],2.0)/(2.0*sigma_v_gp*sigma_v_gp)
//                                )));
                        if (forwardPDF.find(coordinate) != forwardPDF.end()) {
                            objective_time[o] += forwardPDF.at(coordinate)*current_trackPot*pow(dp_gp,3.0)*pow(dv_gp,2.0)*dt_gp;
                        }
                        if (objective_time[o]>0) {
                            logger::Info("Functional tracking part positiv");
                        }
                    }
                }
            }

        }
    });

    for(unsigned int o = 0; o<ntimesteps_gp; o++) {
        objective += objective_time[o];
    }

    //add control, no trapezodial rule needed since control is zero at the boundary (?)
    costOfControl += 1.0/2.0*arma::norm(control,"fro")*arma::norm(control,"fro")*pow(dp_gp,1.0);
    // dp_gp^1 since we have elements with volume dp_gp

    return objective + weight_control_gp*costOfControl;

}
