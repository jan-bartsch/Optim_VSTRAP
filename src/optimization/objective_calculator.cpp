#include "objective_calculator.h"

objective_calculator::objective_calculator()
{

}

objective_calculator::objective_calculator(const char *filename)
{
    this->setData_provider_optim(data_provider(filename));
}

double objective_calculator::calculate_objective_L2(std::vector<std::unordered_map<coordinate_phase_space_time, double> > forwardPDF_time, arma::mat control)
{

    equation_solving_controller solver = equation_solving_controller();
    solver.setData_provider_optim(this->getData_provider_optim());

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
     double db_gp = static_cast<double>(optimizationParameters.find("db_gp")->second);
    double weight_control_gp = static_cast<double>(optimizationParameters.find("weight_control_gp")->second);
    double C_phi_gp = static_cast<double>(optimizationParameters.find("C_phi_gp")->second);
    double C_theta_gp = static_cast<double>(optimizationParameters.find("C_theta_gp")->second);
    double sigma_x_gp = static_cast<double>(optimizationParameters.find("sigma_x_gp")->second);
    double sigma_v_gp = static_cast<double>(optimizationParameters.find("sigma_v_gp")->second);
    double velocity_part_objective = static_cast<double>(optimizationParameters.find("velocity_part_objective")->second);

    int start_control = static_cast<int>(optimizationParameters.find("start_control_gp")->second);
    int end_control = static_cast<int>(optimizationParameters.find("end_control_gp")->second);

    static arma::vec positionDiscr_gp = arma::linspace<arma::vec>(0.0,pmax_gp,pcell_gp+1);
    static arma::vec velocityDiscr_gp = arma::linspace<arma::vec>(-vmax_gp,vmax_gp,vcell_gp);


    //pdf_controller pdf_control = pdf_controller();
    // pdf_control.setData_provider_optim(this->getData_provider_optim());

    desired_trajectory_controller trajectory_controller = desired_trajectory_controller();
    trajectory_controller.setData_provider_optim(this->getData_provider_optim());

    std::map<int,std::vector<double>> baryc = trajectory_controller.getData_provider_optim().getMesh_barycenters();

    //std::cout << baryc.at(0)[0] << std::endl;

    double objective = 0.0;
    double costOfControl = 0.0;

    std::vector<double> objective_time(pcell_gp,0.0);


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

    std::map<std::string,std::string> subroutines = this->getData_provider_optim().getSubroutines();
    std::string desired_traj = subroutines.find("desired_trajectory")->second;
    std::string objective_calculation = subroutines.find("objective_calculation")->second;

    std::cout << "Using <" << desired_traj << "> for desired trajectory" << std::endl;

#pragma omp parallel for
    for(unsigned int  i = 1; i<=pcell_gp; i++)  {
        std::cout << "Functionalcalc. in part " << i << std::endl;
        for(unsigned int  o = 0; o<ntimesteps_gp; o++) {
            //std::cout << "Calculating functional in " << o << " timestep" << std::endl;

            for( unsigned int l = 0; l<vcell_gp; l++) {
                for(unsigned int  m = 0; m<vcell_gp; m++) {
                    for(unsigned int n = 0; n<vcell_gp; n++) {
                        std::vector<double> current_barycenter = baryc.find(static_cast<int> (i))->second;
                        std::vector<double> p_d = trajectory_controller.trajectory_desired(current_barycenter,l,m,n,o);
                        coordinate_phase_space_time coordinate = coordinate_phase_space_time(static_cast<int>(i),static_cast<int>(l),static_cast<int>(m),static_cast<int>(n),static_cast<int>(o));
                        // std::cout << velocityDiscr_gp(l) << std::endl;
                        double current_trackPot = 0.0;
                        if(objective_calculation.compare("magnitude")==0) {
                            current_trackPot = - 1.0/(2.0*M_PI*sigma_x_gp*sigma_v_gp)*exp(
                                        -((std::pow(current_barycenter[0]-p_d[0],2)+std::pow(current_barycenter[1]-p_d[1],2)+std::pow(current_barycenter[2]-p_d[2],2))/(2.0*sigma_x_gp*sigma_x_gp)+
                                    velocity_part_objective*(std::abs(velocityDiscr_gp(l)*velocityDiscr_gp(l)+
                                                                      velocityDiscr_gp(m)*velocityDiscr_gp(m)+
                                                                      velocityDiscr_gp(n)*velocityDiscr_gp(n)-p_d[4]*p_d[4]))/(2.0*sigma_v_gp*sigma_v_gp)
                                    ));
                            //(std::pow(velocityDiscr_gp(l)-p_d[4],2)+
                            //std::pow(velocityDiscr_gp(m)-p_d[4],2)+
                            //std::pow(velocityDiscr_gp(n)-p_d[4],2))/(2.0*sigma_v_gp*sigma_v_gp)
                            //std::cout << current_barycenter[0] << ", " << current_trackPot << std::endl;
                        } else if(objective_calculation.compare("components")==0) {
                            current_trackPot = - C_theta_gp/(2.0*M_PI*sigma_x_gp*sigma_v_gp)*exp(
                                        -(p_d[0]*p_d[0]/(2.0*sigma_x_gp*sigma_x_gp)+
                                    pow(velocityDiscr_gp(l)-p_d[1],2.0)/(2.0*sigma_v_gp*sigma_v_gp)+
                                    pow(velocityDiscr_gp(m)-p_d[2],2.0)/(2.0*sigma_v_gp*sigma_v_gp)+
                                    pow(velocityDiscr_gp(n)-p_d[3],2.0)/(2.0*sigma_v_gp*sigma_v_gp)
                                    ));
                        }
                        if (forwardPDF_time[o].find(coordinate) != forwardPDF_time[o].end()) {
                            objective_time[i] += forwardPDF_time[o].at(coordinate)*current_trackPot*dp_gp*pow(dv_gp,3.0)*dt_gp;
                        } else {
                            std::runtime_error("No such objective calculation rule");
                        }
                        if (objective_time[o]>0) {
                            logger::Info("Functional tracking part positiv");
                            std::runtime_error("Functional tracking part positiv");
                        }
                    }
                }
            }

        }
    }

    for(unsigned int o = 0; o<pcell_gp; o++) {
        objective += objective_time[o];
    }

    //add control, no trapezodial rule needed since control is zero at the boundary (?)
    std::cout << control << std::endl;
    costOfControl += 1.0/2.0*arma::norm(control.rows(start_control-1,end_control-1),"fro")*arma::norm(control.rows(start_control-1,end_control-1),"fro")*pow(dp_gp,1.0);

    arma::mat second_derivative = solver.Laplacian_3D();
    costOfControl += arma::accu(second_derivative*control.rows(start_control-1,end_control-1))/(db_gp*db_gp)*dp_gp;


    objective += 1.0/C_theta_gp*weight_control_gp*costOfControl;

    if(objective > 0) {
        logger::Info("Value of functional positiv - influence of control may be to big");
    }

    std::cout << "Value of objective: " << objective << std::endl;

    return objective;

}
