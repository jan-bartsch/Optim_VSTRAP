#include "gradient_calculator.h"

gradient_calculator::gradient_calculator(const char *filename) {

    this->setData_provider_optim(data_provider(filename));

}

arma::mat gradient_calculator::calculateGradient_forceControl_space_Hm_not_parallel(std::vector<std::unordered_map<coordinate_phase_space_time, double> > forwardPDF_time,
                                                                                    std::vector<std::unordered_map<coordinate_phase_space_time, double> > backwardPDF_time, arma::mat control)
{

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    std::map<int,std::vector<double>> barycenters = this->getData_provider_optim().getMesh_barycenters();


    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(this->getData_provider_optim());
    equation_solving_controller model_solver = equation_solving_controller();
    model_solver.setData_provider_optim(this->getData_provider_optim());
    output_control_update outController = output_control_update();
    outController.setData_provider_optim(this->getData_provider_optim());

    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int vcell_gp = static_cast<unsigned int>(optimizationParameters.find("vcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    double dv_gp = static_cast<double>(optimizationParameters.find("dv_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double db_gp = static_cast<double>(optimizationParameters.find("db_gp")->second);
    double weight_control_gp = static_cast<double>(optimizationParameters.find("weight_control_gp")->second);
    double local_control_x_min_gp = static_cast<double>(optimizationParameters.find("local_control_x_min_gp")->second);
    double local_control_x_max_gp = static_cast<double>(optimizationParameters.find("local_control_x_max_gp")->second);

    int start_control = static_cast<int>(optimizationParameters.find("start_control_gp")->second);
    int end_control = static_cast<int>(optimizationParameters.find("end_control_gp")->second);

    int magnetic_force = static_cast<int>(optimizationParameters.find("magnetic_force")->second);
    int electric_force = static_cast<int>(optimizationParameters.find("electric_force")->second);

    if (magnetic_force == 1 && electric_force == 1) {
        std::cerr << "Magnetic and electric force combined is not implemented yet" << std::endl;
    }
    if (magnetic_force == 0 && electric_force == 0) {
        std::cerr << "Force/Control should be either magnetic or electric. Nothing was specified" << std::endl;
    }

    arma::mat Laplace = model_solver.Laplacian_3D();
    arma::mat Laplace_Squared = model_solver.Laplacian_Squared_3D();

    //std::cout << Laplace << std::endl;
    //std::cout << Laplace_Squared << std::endl;

    arma::mat gradient(pcell_gp,3,arma::fill::zeros);
    arma::mat gradient_Riesz(dimensionOfControl_gp,3,arma::fill::zeros);
    arma::mat rhs_Riesz(dimensionOfControl_gp,3,arma::fill::zeros);

    //Caculate integral in gradient
    const unsigned int n = pcell_gp;

    //#pragma omp parallel for
    for(unsigned int i = 1; i< n+1; i++) {

        std::vector<double> current_barycenter = barycenters.find(static_cast<int>(i))->second;

        if (current_barycenter[0]> local_control_x_min_gp && current_barycenter[0] < local_control_x_max_gp) {
            std::vector<std::vector<std::vector<std::vector<double>>>> forwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                        (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
            std::vector<std::vector<std::vector<std::vector<double>>>> backwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                         (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));

            double firstDerivativeForwardPDF_V1_current, firstDerivativeForwardPDF_V2_current, firstDerivativeForwardPDF_V3_current;

            for(unsigned int o = 0; o<ntimesteps_gp; o++) {
                for(unsigned int l = 0; l<vcell_gp; l++) {
                    for(unsigned int m= 0; m<vcell_gp; m++) {
                        for(unsigned int n = 0; n<vcell_gp; n++) {
                            coordinate_phase_space_time coordinate = coordinate_phase_space_time( static_cast<int>(i),static_cast<int>(l),static_cast<int>(m),static_cast<int>(n),static_cast<int>(o));
                            auto iteratorForward = forwardPDF_time[o].find(coordinate);
                            auto iteratorBackward = backwardPDF_time[o].find(coordinate);
                            if(iteratorForward != forwardPDF_time[o].end()) {
                                forwardPDFdouble[o][l][m][n] = iteratorForward->second;
                            } else {
                                forwardPDFdouble[o][l][m][n] = 0.0;
                            }
                            if( iteratorBackward != backwardPDF_time[o].end() ) {
                                backwardPDFdouble[o][l][m][n]= iteratorBackward->second;
                            } else {
                                backwardPDFdouble[o][l][m][n] = 0.0;
                            }
                        }
                    }
                }
            }


            forwardPDFdouble = pdf_control.relaxating_GaussSeidel_4D(forwardPDFdouble,1000);


            for (unsigned int o = 0; o<ntimesteps_gp; o++) {
                for(unsigned int l = 0; l<vcell_gp; l++) {
                    for(unsigned int m= 0; m<vcell_gp; m++) {
                        for(unsigned int n = 0; n<vcell_gp; n++) {
                            // derivative v_x
                            if (l == 0) {
                                firstDerivativeForwardPDF_V1_current = (forwardPDFdouble[o][l+1][m][n]-forwardPDFdouble[o][l][m][n])/dv_gp;
                            } else if (l == vcell_gp-1) {
                                firstDerivativeForwardPDF_V1_current = (forwardPDFdouble[o][l][m][n]-forwardPDFdouble[o][l-1][m][n])/dv_gp;
                            } else {
                                firstDerivativeForwardPDF_V1_current = (forwardPDFdouble[o][l+1][m][n]-forwardPDFdouble[o][l-1][m][n])/(2.0*dv_gp);
                            }
                            //derivative v_y
                            if (m== 0) {
                                firstDerivativeForwardPDF_V2_current = (forwardPDFdouble[o][l][m+1][n]-forwardPDFdouble[o][l][m][n])/dv_gp;
                            } else if (m == vcell_gp-1) {
                                firstDerivativeForwardPDF_V2_current = (forwardPDFdouble[o][l][m][n]-forwardPDFdouble[o][l][m-1][n])/dv_gp;
                            } else {
                                firstDerivativeForwardPDF_V2_current = (forwardPDFdouble[o][l][m+1][n]-forwardPDFdouble[o][l][m-1][n])/(2.0*dv_gp);
                            }
                            //derivative v_z
                            if (n == 0) {
                                firstDerivativeForwardPDF_V3_current = (forwardPDFdouble[o][l][m][n+1]-forwardPDFdouble[o][l][m][n])/dv_gp;
                            } else if (n == vcell_gp-1) {
                                firstDerivativeForwardPDF_V3_current = (forwardPDFdouble[o][l][m][n]-forwardPDFdouble[o][l][m][n-1])/dv_gp;
                            } else {
                                firstDerivativeForwardPDF_V3_current = (forwardPDFdouble[o][l][m][n+1]-forwardPDFdouble[o][l][m][n-1])/(2.0*dv_gp);
                            }
                            /*
                             * electric field
                            */
                            if (electric_force == 1) {
                                gradient(i-1,0) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V1_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                                gradient(i-1,1) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V2_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                                gradient(i-1,2) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V3_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            }
                            /*
                             * Magnetic field
                             */
                            if (magnetic_force == 1) {
                                gradient(i-1,0) += backwardPDFdouble[o][l][m][n]*(-velocity_Discr[n]*firstDerivativeForwardPDF_V2_current+velocity_Discr[m]*firstDerivativeForwardPDF_V3_current)*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                                gradient(i-1,1) += backwardPDFdouble[o][l][m][n]*(velocity_Discr[n]*firstDerivativeForwardPDF_V1_current-velocity_Discr[l]*firstDerivativeForwardPDF_V3_current)*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                                gradient(i-1,2) += backwardPDFdouble[o][l][m][n]*(-velocity_Discr[m]*firstDerivativeForwardPDF_V1_current+velocity_Discr[l]*firstDerivativeForwardPDF_V2_current)*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            }
                        }
                    }
                }
            }
        } else {
            std::cout << "Cell_id " << i << ": Gradient stays zero here" << std::endl;
        }
    }

    /*
     * Assemble gradient
     */

    //    std::cout << "Gradient:" << std::endl;
    //    std::cout << gradient << std::endl;


    for(int j = 0; j < pcell_gp; j++) {
        if (j>start_control-2 && j<end_control) {
            rhs_Riesz(j-start_control+1,0) = gradient(j,0);
            rhs_Riesz(j-start_control+1,1) = gradient(j,1);
            rhs_Riesz(j-start_control+1,2) = gradient(j,2);
        }
    }

    //std::cout << "rhs_Riesz:" << std::endl;
    std::cout << rhs_Riesz << std::endl;

    // std::cout << "Riesz Matrix" << std::endl;
    arma::mat Riesz = weight_control_gp*(arma::eye(dimensionOfControl_gp,dimensionOfControl_gp) - 1.0/(pow(db_gp,2))*Laplace + 1.0/(pow(db_gp,4))*Laplace_Squared);
    //std::cout << Riesz << std::endl;
    outController.writeArmaMatrixToFile(Riesz,"RiesMatrix");
    //std::cout << "Condition number Matrix Riesz: " << arma::cond(Riesz) << std::endl;

    arma::mat   Riesz_control(dimensionOfControl_gp,3,arma::fill::zeros);
    for(int j = 0; j < pcell_gp; j++) {
        if (j>start_control-2 && j<end_control) {
            Riesz_control(j-start_control+1,0) = control(j,0);
            Riesz_control(j-start_control+1,1) = control(j,1);
            Riesz_control(j-start_control+1,2) = control(j,2);
        }
    }

    gradient_Riesz = arma::solve(Riesz,Riesz*Riesz_control+rhs_Riesz);
    arma::mat return_gradient(pcell_gp,3,arma::fill::zeros);

    //    std::cout << "Solution elliptic equation:" << std::endl;
    std::cout << gradient_Riesz << std::endl;

    for(int j = 0; j < pcell_gp; j++) {
        if (j>start_control-2 && j<end_control) {
            return_gradient(j,0) = gradient_Riesz(j-start_control+1,0);
            return_gradient(j,1) = gradient_Riesz(j-start_control+1,1);
            return_gradient(j,2) = gradient_Riesz(j-start_control+1,2);
        }
    }
    std::cout << "Return_Gradient:" << std::endl;
    std::cout << return_gradient << std::endl;

    return return_gradient;
}

arma::mat gradient_calculator::calculateGradient_forceControl_space_Hm(std::vector<std::unordered_map<coordinate_phase_space_time, double> > forwardPDF_time,
                                                                       std::vector<std::unordered_map<coordinate_phase_space_time, double> > backwardPDF_time, arma::mat control)
{

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    std::map<int,std::vector<double>> barycenters = this->getData_provider_optim().getMesh_barycenters();

    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(this->getData_provider_optim());
    equation_solving_controller model_solver = equation_solving_controller();
    model_solver.setData_provider_optim(this->getData_provider_optim());
    output_control_update outController = output_control_update();
    outController.setData_provider_optim(this->getData_provider_optim());

    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int vcell_gp = static_cast<unsigned int>(optimizationParameters.find("vcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    double dv_gp = static_cast<double>(optimizationParameters.find("dv_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double db_gp = static_cast<double>(optimizationParameters.find("db_gp")->second);
    double vmax_gp = static_cast<double>(optimizationParameters.find("vmax_gp")->second);
    double weight_control_gp = static_cast<double>(optimizationParameters.find("weight_control_gp")->second);
    double local_control_x_min_gp = static_cast<double>(optimizationParameters.find("local_control_x_min_gp")->second);
    double local_control_x_max_gp = static_cast<double>(optimizationParameters.find("local_control_x_max_gp")->second);

    int start_control = static_cast<int>(optimizationParameters.find("start_control_gp")->second);
    int end_control = static_cast<int>(optimizationParameters.find("end_control_gp")->second);

    int magnetic_force = static_cast<int>(optimizationParameters.find("magnetic_force")->second);
    int electric_force = static_cast<int>(optimizationParameters.find("electric_force")->second);

    if (magnetic_force == 1 && electric_force == 1) {
        std::cerr << "Magnetic and electric force combined is not implemented yet" << std::endl;
    }
    if (magnetic_force == 0 && electric_force == 0) {
        std::cerr << "Force/Control should be either magnetic or electric. Nothing was specified" << std::endl;
    }

    arma::mat Laplace = model_solver.Laplacian_3D();
    arma::mat Laplace_Squared = model_solver.Laplacian_Squared_3D();

    //std::cout << Laplace << std::endl;
    //std::cout << Laplace_Squared << std::endl;

    arma::mat gradient(pcell_gp,3,arma::fill::zeros);
    arma::mat gradient_2(pcell_gp,3,arma::fill::zeros);
    arma::mat gradient_Riesz(dimensionOfControl_gp,3,arma::fill::zeros);
    arma::mat rhs_Riesz(dimensionOfControl_gp,3,arma::fill::zeros);

    arma::vec velocity_Discr = arma::linspace<arma::vec>(-vmax_gp,vmax_gp,vcell_gp);

    //Caculate integral in gradient
    const unsigned int n = pcell_gp;

#pragma omp parallel for
    for(unsigned int i = 1; i< n+1; i++) {

        std::cout << "Calculating gradient in Cell " << i << std::endl;

        std::vector<double> current_barycenter = barycenters.find(static_cast<int>(i))->second;

        if (current_barycenter[0]> local_control_x_min_gp && current_barycenter[0] < local_control_x_max_gp) {
            std::vector<std::vector<std::vector<std::vector<double>>>> forwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                        (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
            std::vector<std::vector<std::vector<std::vector<double>>>> backwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                         (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));

            double firstDerivativeForwardPDF_V1_current = 0.0;
            double firstDerivativeForwardPDF_V2_current = 0.0;
            double firstDerivativeForwardPDF_V3_current = 0.0;

            double firstDerivativeBackwardPDF_V1_current = 0.0;
            double firstDerivativeBackwardPDF_V2_current = 0.0;
            double firstDerivativeBackwardPDF_V3_current = 0.0;


            for(unsigned int o = 0; o<ntimesteps_gp; o++) {
                for(unsigned int l = 0; l<vcell_gp; l++) {
                    for(unsigned int m= 0; m<vcell_gp; m++) {
                        for(unsigned int n = 0; n<vcell_gp; n++) {
                            coordinate_phase_space_time coordinate = coordinate_phase_space_time( static_cast<int>(i),static_cast<int>(l),static_cast<int>(m),static_cast<int>(n),static_cast<int>(o));
                            auto iteratorForward = forwardPDF_time[o].find(coordinate);
                            //                            auto iteratorBackward = backwardPDF_time[o].find(coordinate);
                            if(iteratorForward != forwardPDF_time[o].end()) {
                                forwardPDFdouble[o][l][m][n] = iteratorForward->second;
                                auto iteratorBackward = backwardPDF_time[o].find(coordinate);
                                if( iteratorBackward != backwardPDF_time[o].end() ) {
                                    backwardPDFdouble[o][l][m][n]= iteratorBackward->second;
                                }
                            } else {
                                forwardPDFdouble[o][l][m][n] = 0.0;
                                backwardPDFdouble[o][l][m][n] = 0.0;
                            }
                            //                            if( iteratorBackward != backwardPDF_time[o].end() ) {
                            //                                backwardPDFdouble[o][l][m][n]= iteratorBackward->second;
                            //                            } else {
                            //                                backwardPDFdouble[o][l][m][n] = 0.0;
                            //                            }
                        }
                    }
                }
            }


            forwardPDFdouble = pdf_control.relaxating_GaussSeidel_4D(forwardPDFdouble,1000);


            for (unsigned int o = 0; o<ntimesteps_gp; o++) {
                for(unsigned int l = 0; l<vcell_gp; l++) {
                    for(unsigned int m= 0; m<vcell_gp; m++) {
                        for(unsigned int n = 0; n<vcell_gp; n++) {
                            // derivative v_x
                            double current_forward = forwardPDFdouble[o][l][m][n];
                            if (l == 0) {
                                firstDerivativeForwardPDF_V1_current = (forwardPDFdouble[o][l+1][m][n]-current_forward)/dv_gp;
                                //firstDerivativeBackwardPDF_V1_current = (backwardPDFdouble[o][l+1][m][n]-backwardPDFdouble[o][l][m][n])/dv_gp;
                            } else if (l == vcell_gp-1) {
                                firstDerivativeForwardPDF_V1_current = (current_forward-forwardPDFdouble[o][l-1][m][n])/dv_gp;
                                //firstDerivativeBackwardPDF_V1_current = (backwardPDFdouble[o][l][m][n]-backwardPDFdouble[o][l-1][m][n])/dv_gp;
                            } else {
                                firstDerivativeForwardPDF_V1_current = (forwardPDFdouble[o][l+1][m][n]-forwardPDFdouble[o][l-1][m][n])/(2.0*dv_gp);
                                //firstDerivativeBackwardPDF_V1_current = (backwardPDFdouble[o][l+1][m][n]-backwardPDFdouble[o][l-1][m][n])/(2.0*dv_gp);
                            }
                            //derivative v_y
                            if (m== 0) {
                                firstDerivativeForwardPDF_V2_current = (forwardPDFdouble[o][l][m+1][n]-current_forward)/dv_gp;
                                //firstDerivativeBackwardPDF_V2_current = (backwardPDFdouble[o][l][m+1][n]-backwardPDFdouble[o][l][m][n])/dv_gp;
                            } else if (m == vcell_gp-1) {
                                firstDerivativeForwardPDF_V2_current = (current_forward-forwardPDFdouble[o][l][m-1][n])/dv_gp;
                                //firstDerivativeBackwardPDF_V2_current = (backwardPDFdouble[o][l][m][n]-backwardPDFdouble[o][l][m-1][n])/dv_gp;
                            } else {
                                firstDerivativeForwardPDF_V2_current = (forwardPDFdouble[o][l][m+1][n]-forwardPDFdouble[o][l][m-1][n])/(2.0*dv_gp);
                                //firstDerivativeBackwardPDF_V2_current = (backwardPDFdouble[o][l][m+1][n]-backwardPDFdouble[o][l][m-1][n])/(2.0*dv_gp);
                            }
                            //derivative v_z
                            if (n == 0) {
                                firstDerivativeForwardPDF_V3_current = (forwardPDFdouble[o][l][m][n+1]-current_forward)/dv_gp;
                                //firstDerivativeBackwardPDF_V3_current = (backwardPDFdouble[o][l][m][n+1]-backwardPDFdouble[o][l][m][n])/dv_gp;
                            } else if (n == vcell_gp-1) {
                                firstDerivativeForwardPDF_V3_current = (current_forward-forwardPDFdouble[o][l][m][n-1])/dv_gp;
                                //firstDerivativeBackwardPDF_V3_current = (backwardPDFdouble[o][l][m][n]-backwardPDFdouble[o][l][m][n-1])/dv_gp;
                            } else {
                                firstDerivativeForwardPDF_V3_current = (forwardPDFdouble[o][l][m][n+1]-forwardPDFdouble[o][l][m][n-1])/(2.0*dv_gp);
                                //firstDerivativeBackwardPDF_V3_current = (backwardPDFdouble[o][l][m][n+1]-backwardPDFdouble[o][l][m][n-1])/(2.0*dv_gp);
                            }
                            /*
                             * Electric field
                             *  */

                            if (electric_force == 1) {
                                gradient(i-1,0) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V1_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                                gradient(i-1,1) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V2_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                                gradient(i-1,2) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V3_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            }
                            //integration by parts leads to
                            //                            gradient_2(i-1,0) -= forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V1_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            //                            gradient_2(i-1,1) -= forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V2_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            //                            gradient_2(i-1,2) -= forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V3_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);


                            /*
                             * Magnetic field
                             */
                            if (magnetic_force == 1) {
                                gradient(i-1,0) += backwardPDFdouble[o][l][m][n]*(-velocity_Discr[n]*firstDerivativeForwardPDF_V2_current+velocity_Discr[m]*firstDerivativeForwardPDF_V3_current)*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                                gradient(i-1,1) += backwardPDFdouble[o][l][m][n]*(velocity_Discr[n]*firstDerivativeForwardPDF_V1_current-velocity_Discr[l]*firstDerivativeForwardPDF_V3_current)*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                                gradient(i-1,2) += backwardPDFdouble[o][l][m][n]*(-velocity_Discr[m]*firstDerivativeForwardPDF_V1_current+velocity_Discr[l]*firstDerivativeForwardPDF_V2_current)*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            }
                        }
                    }
                }
            }
        } else {
            std::cout << "Cell_id " << i << ": Gradient stays zero here" << std::endl;
        }
    }



    /*
     * Assemble gradient
     */

    //    std::cout << "Gradient:" << std::endl;
    //    std::cout << gradient << std::endl;


    for(int j = 0; j < pcell_gp; j++) {
        if (j>start_control-2 && j<end_control) {
            rhs_Riesz(j-start_control+1,0) = gradient(j,0);
            rhs_Riesz(j-start_control+1,1) = gradient(j,1);
            rhs_Riesz(j-start_control+1,2) = gradient(j,2);
        }
    }

    //std::cout << "rhs_Riesz:" << std::endl;
    std::cout << rhs_Riesz << std::endl;

    // std::cout << "Riesz Matrix" << std::endl;
    arma::mat Riesz = weight_control_gp*(arma::eye(dimensionOfControl_gp,dimensionOfControl_gp) - 1.0/(pow(db_gp,2))*Laplace + 1.0/(pow(db_gp,4))*Laplace_Squared);
    //std::cout << Riesz << std::endl;
    outController.writeArmaMatrixToFile(Riesz,"RiesMatrix");
    //std::cout << "Condition number Matrix Riesz: " << arma::cond(Riesz) << std::endl;

    arma::mat   Riesz_control(dimensionOfControl_gp,3,arma::fill::zeros);
    for(int j = 0; j < pcell_gp; j++) {
        if (j>start_control-2 && j<end_control) {
            Riesz_control(j-start_control+1,0) = control(j,0);
            Riesz_control(j-start_control+1,1) = control(j,1);
            Riesz_control(j-start_control+1,2) = control(j,2);
        }
    }

    gradient_Riesz = arma::solve(Riesz,Riesz*Riesz_control+rhs_Riesz);
    //gradient_Riesz = arma::solve(Riesz,-rhs_Riesz);
    arma::mat return_gradient(pcell_gp,3,arma::fill::zeros);

    //    std::cout << "Solution elliptic equation:" << std::endl;
    std::cout << gradient_Riesz << std::endl;

    for(int j = 0; j < pcell_gp; j++) {
        if (j>start_control-2 && j<end_control) {
            return_gradient(j,0) = gradient_Riesz(j-start_control+1,0);
            return_gradient(j,1) = gradient_Riesz(j-start_control+1,1);
            return_gradient(j,2) = gradient_Riesz(j-start_control+1,2);
        }
    }
    std::cout << "Return_Gradient:" << std::endl;
    std::cout << return_gradient << std::endl;

    return return_gradient;
}

arma::mat gradient_calculator::calculateGradient_forceControl_space_Hm_plasma(std::vector<std::unordered_map<coordinate_phase_space_time, double>> forwardPDF_time,
                                                                              std::vector<std::unordered_map<coordinate_phase_space_time, double>> backwardPDF_time,
                                                                              std::vector<std::unordered_map<coordinate_phase_space_time, double>> forwardPDF_time_electrons,
                                                                              std::vector<std::unordered_map<coordinate_phase_space_time, double>> backwardPDF_time_electrons,
                                                                              arma::mat control)
{

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    std::map<int,std::vector<double>> barycenters = this->getData_provider_optim().getMesh_barycenters();


    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(this->getData_provider_optim());
    equation_solving_controller model_solver = equation_solving_controller();
    model_solver.setData_provider_optim(this->getData_provider_optim());
    output_control_update outController = output_control_update();
    outController.setData_provider_optim(this->getData_provider_optim());

    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int vcell_gp = static_cast<unsigned int>(optimizationParameters.find("vcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    double dv_gp = static_cast<double>(optimizationParameters.find("dv_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double db_gp = static_cast<double>(optimizationParameters.find("db_gp")->second);
    double weight_control_gp = static_cast<double>(optimizationParameters.find("weight_control_gp")->second);
    double local_control_x_min_gp = static_cast<double>(optimizationParameters.find("local_control_x_min_gp")->second);
    double local_control_x_max_gp = static_cast<double>(optimizationParameters.find("local_control_x_max_gp")->second);

    int start_control = static_cast<int>(optimizationParameters.find("start_control_gp")->second);
    int end_control = static_cast<int>(optimizationParameters.find("end_control_gp")->second);

    int simulating_plasma = static_cast<int>(optimizationParameters.find("simulating_plasma")->second);

    arma::mat Laplace = model_solver.Laplacian_3D();
    arma::mat Laplace_Squared = model_solver.Laplacian_Squared_3D();

    //std::cout << Laplace << std::endl;
    //std::cout << Laplace_Squared << std::endl;

    arma::mat gradient(pcell_gp,3,arma::fill::zeros);
    arma::mat gradient_2(pcell_gp,3,arma::fill::zeros);
    arma::mat gradient_Riesz(dimensionOfControl_gp,3,arma::fill::zeros);
    arma::mat rhs_Riesz(dimensionOfControl_gp,3,arma::fill::zeros);

    //Caculate integral in gradient
    const unsigned int n = pcell_gp;

#pragma omp parallel for
    for(unsigned int i = 1; i< n+1; i++) {

        std::cout << "Calculating gradient in Cell " << i << std::endl;

        std::vector<double> current_barycenter = barycenters.find(static_cast<int>(i))->second;

        if (current_barycenter[0]> local_control_x_min_gp && current_barycenter[0] < local_control_x_max_gp) {
            std::vector<std::vector<std::vector<std::vector<double>>>> forwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                        (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
            std::vector<std::vector<std::vector<std::vector<double>>>> backwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                         (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
            std::vector<std::vector<std::vector<std::vector<double>>>> forwardPDFdouble_electrons(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                                  (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
            std::vector<std::vector<std::vector<std::vector<double>>>> backwardPDFdouble_electrons(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                                   (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));

            double firstDerivativeForwardPDF_V1_current = 0.0;
            double firstDerivativeForwardPDF_V2_current = 0.0;
            double firstDerivativeForwardPDF_V3_current = 0.0;

            double firstDerivativeForwardPDF_V1_current_electrons = 0.0;
            double firstDerivativeForwardPDF_V2_current_electrons = 0.0;
            double firstDerivativeForwardPDF_V3_current_electrons = 0.0;

            double firstDerivativeBackwardPDF_V1_current = 0.0;
            double firstDerivativeBackwardPDF_V2_current = 0.0;
            double firstDerivativeBackwardPDF_V3_current = 0.0;


            for(unsigned int o = 0; o<ntimesteps_gp; o++) {
                for(unsigned int l = 0; l<vcell_gp; l++) {
                    for(unsigned int m= 0; m<vcell_gp; m++) {
                        for(unsigned int n = 0; n<vcell_gp; n++) {
                            coordinate_phase_space_time coordinate = coordinate_phase_space_time( static_cast<int>(i),static_cast<int>(l),static_cast<int>(m),static_cast<int>(n),static_cast<int>(o));
                            auto iteratorForward = forwardPDF_time[o].find(coordinate);
                            //                            auto iteratorBackward = backwardPDF_time[o].find(coordinate);
                            if(iteratorForward != forwardPDF_time[o].end()) {
                                forwardPDFdouble[o][l][m][n] = iteratorForward->second;
                                auto iteratorBackward = backwardPDF_time[o].find(coordinate);
                                if( iteratorBackward != backwardPDF_time[o].end() ) {
                                    backwardPDFdouble[o][l][m][n]= iteratorBackward->second;
                                }
                            } else {
                                forwardPDFdouble[o][l][m][n] = 0.0;
                                backwardPDFdouble[o][l][m][n] = 0.0;
                            }
                            //                            if( iteratorBackward != backwardPDF_time[o].end() ) {
                            //                                backwardPDFdouble[o][l][m][n]= iteratorBackward->second;
                            //                            } else {
                            //                                backwardPDFdouble[o][l][m][n] = 0.0;
                            //                            }
                            auto iteratorForward_electrons = forwardPDF_time_electrons[o].find(coordinate);
                            if(iteratorForward_electrons != forwardPDF_time_electrons[o].end()) {
                                forwardPDFdouble_electrons[o][l][m][n] = iteratorForward_electrons->second;
                                auto iteratorBackward_electrons = backwardPDF_time_electrons[o].find(coordinate);
                                if( iteratorBackward_electrons != backwardPDF_time_electrons[o].end() ) {
                                    backwardPDFdouble_electrons[o][l][m][n]= iteratorBackward_electrons->second;
                                }
                            } else {
                                forwardPDFdouble_electrons[o][l][m][n] = 0.0;
                                backwardPDFdouble_electrons[o][l][m][n] = 0.0;
                            }
                        }
                    }
                }
            }


            forwardPDFdouble = pdf_control.relaxating_GaussSeidel_4D(forwardPDFdouble,1000);


            for (unsigned int o = 0; o<ntimesteps_gp; o++) {
                for(unsigned int l = 0; l<vcell_gp; l++) {
                    for(unsigned int m= 0; m<vcell_gp; m++) {
                        for(unsigned int n = 0; n<vcell_gp; n++) {
                            // derivative v_x
                            double current_forward = forwardPDFdouble[o][l][m][n];
                            double current_forward_electrons = forwardPDFdouble_electrons[o][l][m][n];
                            if (l == 0) {
                                firstDerivativeForwardPDF_V1_current = (forwardPDFdouble[o][l+1][m][n]-current_forward)/dv_gp;
                                firstDerivativeForwardPDF_V1_current_electrons = (forwardPDFdouble_electrons[o][l+1][m][n]-current_forward_electrons)/dv_gp;
                                //firstDerivativeBackwardPDF_V1_current = (backwardPDFdouble[o][l+1][m][n]-backwardPDFdouble[o][l][m][n])/dv_gp;
                            } else if (l == vcell_gp-1) {
                                firstDerivativeForwardPDF_V1_current = (current_forward-forwardPDFdouble[o][l-1][m][n])/dv_gp;
                                firstDerivativeForwardPDF_V1_current_electrons = (current_forward_electrons-forwardPDFdouble_electrons[o][l-1][m][n])/dv_gp;
                                //firstDerivativeBackwardPDF_V1_current = (backwardPDFdouble[o][l][m][n]-backwardPDFdouble[o][l-1][m][n])/dv_gp;
                            } else {
                                firstDerivativeForwardPDF_V1_current = (forwardPDFdouble[o][l+1][m][n]-forwardPDFdouble[o][l-1][m][n])/(2.0*dv_gp);
                                firstDerivativeForwardPDF_V1_current_electrons = (forwardPDFdouble_electrons[o][l+1][m][n]-forwardPDFdouble_electrons[o][l-1][m][n])/(2.0*dv_gp);
                                //firstDerivativeBackwardPDF_V1_current = (backwardPDFdouble[o][l+1][m][n]-backwardPDFdouble[o][l-1][m][n])/(2.0*dv_gp);
                            }
                            //derivative v_y
                            if (m== 0) {
                                firstDerivativeForwardPDF_V2_current = (forwardPDFdouble[o][l][m+1][n]-current_forward)/dv_gp;
                                firstDerivativeForwardPDF_V2_current_electrons = (forwardPDFdouble_electrons[o][l][m+1][n]-current_forward_electrons)/dv_gp;
                                //firstDerivativeBackwardPDF_V2_current = (backwardPDFdouble[o][l][m+1][n]-backwardPDFdouble[o][l][m][n])/dv_gp;
                            } else if (m == vcell_gp-1) {
                                firstDerivativeForwardPDF_V2_current = (current_forward-forwardPDFdouble[o][l][m-1][n])/dv_gp;
                                firstDerivativeForwardPDF_V2_current_electrons = (current_forward_electrons-forwardPDFdouble_electrons[o][l][m-1][n])/dv_gp;
                                //firstDerivativeBackwardPDF_V2_current = (backwardPDFdouble[o][l][m][n]-backwardPDFdouble[o][l][m-1][n])/dv_gp;
                            } else {
                                firstDerivativeForwardPDF_V2_current = (forwardPDFdouble[o][l][m+1][n]-forwardPDFdouble[o][l][m-1][n])/(2.0*dv_gp);
                                firstDerivativeForwardPDF_V2_current_electrons = (forwardPDFdouble_electrons[o][l][m+1][n]-forwardPDFdouble_electrons[o][l][m-1][n])/(2.0*dv_gp);
                                //firstDerivativeBackwardPDF_V2_current = (backwardPDFdouble[o][l][m+1][n]-backwardPDFdouble[o][l][m-1][n])/(2.0*dv_gp);
                            }
                            //derivative v_z
                            if (n == 0) {
                                firstDerivativeForwardPDF_V3_current = (forwardPDFdouble[o][l][m][n+1]-current_forward)/dv_gp;
                                firstDerivativeForwardPDF_V3_current_electrons = (forwardPDFdouble_electrons[o][l][m][n+1]-current_forward_electrons)/dv_gp;
                                //firstDerivativeBackwardPDF_V3_current = (backwardPDFdouble[o][l][m][n+1]-backwardPDFdouble[o][l][m][n])/dv_gp;
                            } else if (n == vcell_gp-1) {
                                firstDerivativeForwardPDF_V3_current = (current_forward-forwardPDFdouble[o][l][m][n-1])/dv_gp;
                                firstDerivativeForwardPDF_V3_current_electrons = (current_forward_electrons-forwardPDFdouble_electrons[o][l][m][n-1])/dv_gp;
                                //firstDerivativeBackwardPDF_V3_current = (backwardPDFdouble[o][l][m][n]-backwardPDFdouble[o][l][m][n-1])/dv_gp;
                            } else {
                                firstDerivativeForwardPDF_V3_current = (forwardPDFdouble[o][l][m][n+1]-forwardPDFdouble[o][l][m][n-1])/(2.0*dv_gp);
                                firstDerivativeForwardPDF_V3_current_electrons = (forwardPDFdouble_electrons[o][l][m][n+1]-forwardPDFdouble_electrons[o][l][m][n-1])/(2.0*dv_gp);
                                //firstDerivativeBackwardPDF_V3_current = (backwardPDFdouble[o][l][m][n+1]-backwardPDFdouble[o][l][m][n-1])/(2.0*dv_gp);
                            }

                            //DIFFERENT MASSES!!!
                            gradient(i-1,0) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V1_current*pow(dv_gp,3.0)*pow(dt_gp,1.0)
                                    - 1.0/6.0*backwardPDFdouble_electrons[o][l][m][n]*firstDerivativeForwardPDF_V1_current_electrons*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            gradient(i-1,1) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V2_current*pow(dv_gp,3.0)*pow(dt_gp,1.0)
                                    - 1.0/6.0*backwardPDFdouble_electrons[o][l][m][n]*firstDerivativeForwardPDF_V2_current_electrons*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            gradient(i-1,2) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V3_current*pow(dv_gp,3.0)*pow(dt_gp,1.0)
                                    - 1.0/6.0*backwardPDFdouble_electrons[o][l][m][n]*firstDerivativeForwardPDF_V3_current_electrons*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            //integration by parts leads to
                            //                            gradient_2(i-1,0) -= forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V1_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            //                            gradient_2(i-1,1) -= forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V2_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            //                            gradient_2(i-1,2) -= forwardPDFdouble[o][l][m][n]*firstDerivativeBackwardPDF_V3_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                        }
                    }
                }
            }
        } else {
            std::cout << "Cell_id " << i << ": Gradient stays zero here" << std::endl;
        }
    }



    /*
     * Assemble gradient
     */

    //    std::cout << "Gradient:" << std::endl;
    //    std::cout << gradient << std::endl;


    for(int j = 0; j < pcell_gp; j++) {
        if (j>start_control-2 && j<end_control) {
            rhs_Riesz(j-start_control+1,0) = gradient(j,0);
            rhs_Riesz(j-start_control+1,1) = gradient(j,1);
            rhs_Riesz(j-start_control+1,2) = gradient(j,2);
        }
    }

    //std::cout << "rhs_Riesz:" << std::endl;
    std::cout << rhs_Riesz << std::endl;

    // std::cout << "Riesz Matrix" << std::endl;
    arma::mat Riesz = weight_control_gp*(arma::eye(dimensionOfControl_gp,dimensionOfControl_gp) - 1.0/(pow(db_gp,2))*Laplace + 1.0/(pow(db_gp,4))*Laplace_Squared);
    //std::cout << Riesz << std::endl;
    outController.writeArmaMatrixToFile(Riesz,"RiesMatrix");
    //std::cout << "Condition number Matrix Riesz: " << arma::cond(Riesz) << std::endl;

    arma::mat   Riesz_control(dimensionOfControl_gp,3,arma::fill::zeros);
    for(int j = 0; j < pcell_gp; j++) {
        if (j>start_control-2 && j<end_control) {
            Riesz_control(j-start_control+1,0) = control(j,0);
            Riesz_control(j-start_control+1,1) = control(j,1);
            Riesz_control(j-start_control+1,2) = control(j,2);
        }
    }

    gradient_Riesz = arma::solve(Riesz,Riesz*Riesz_control+rhs_Riesz);
    //gradient_Riesz = arma::solve(Riesz,-rhs_Riesz);
    arma::mat return_gradient(pcell_gp,3,arma::fill::zeros);

    //    std::cout << "Solution elliptic equation:" << std::endl;
    std::cout << gradient_Riesz << std::endl;

    for(int j = 0; j < pcell_gp; j++) {
        if (j>start_control-2 && j<end_control) {
            return_gradient(j,0) = gradient_Riesz(j-start_control+1,0);
            return_gradient(j,1) = gradient_Riesz(j-start_control+1,1);
            return_gradient(j,2) = gradient_Riesz(j-start_control+1,2);
        }
    }
    std::cout << "Return_Gradient:" << std::endl;
    std::cout << return_gradient << std::endl;

    return return_gradient;
}

