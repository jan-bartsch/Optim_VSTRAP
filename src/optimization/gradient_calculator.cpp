#include "gradient_calculator.h"

gradient_calculator::gradient_calculator(const char *filename) {

    this->setData_provider_optim(data_provider(filename));

}

arma::mat gradient_calculator::calculateGradient_forceControl_space_L2(std::unordered_map<coordinate_phase_space_time,double> forwardPDF,
                                                                       std::unordered_map<coordinate_phase_space_time,double> backwardPDF, arma::mat control)
{

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();
    std::map<int,std::vector<double>> barycenters = this->getData_provider_optim().getMesh_barycenters();


    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(this->getData_provider_optim());

    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int vcell_gp = static_cast<unsigned int>(optimizationParameters.find("vcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    double dv_gp = static_cast<double>(optimizationParameters.find("dv_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double weight_control_gp = static_cast<double>(optimizationParameters.find("weight_control_gp")->second);



    arma::mat gradient(pcell_gp,3,arma::fill::zeros);

    //Caculate integral in gradient
    int numberThreadsTBB = tbb::task_scheduler_init::default_num_threads();
    int usedThreads = numberThreadsTBB;

    if(numberThreadsTBB > static_cast<int>(pcell_gp)) {
        usedThreads = static_cast<int>(pcell_gp);
    }
    tbb::task_scheduler_init init(usedThreads);

    std::cout << "Using " <<  usedThreads << " threads for assembling gradient" << std::endl;
    const unsigned int n = pcell_gp;
    tbb::parallel_for(static_cast<unsigned int> (1), n+1 , [&]( unsigned int i ) {

        std::vector<double> current_barycenter = barycenters.find(static_cast<int>(i))->second;

        std::cout << "Cell_id: " << i << " with barycenter (" << current_barycenter[0] << ","
                  << current_barycenter[1] << "," << current_barycenter[2] << ")" << std::endl;

        if (current_barycenter[0]< - 0.2 || current_barycenter[0] > 0.2) {
            //if(false) {
            std::cout << "Gradient will stay zero here" << std::endl;
        } else {
            std::vector<std::vector<std::vector<std::vector<double>>>> forwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                        (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
            std::vector<std::vector<std::vector<std::vector<double>>>> backwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                         (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));

            double firstDerivativeForwardPDF_V1_current, firstDerivativeForwardPDF_V2_current, firstDerivativeForwardPDF_V3_current;

            for(unsigned int o = 0; o<ntimesteps_gp; o++) {
                for(unsigned int l = 0; l<vcell_gp; l++) {
                    for(unsigned int m= 0; m<vcell_gp; m++) {
                        for(unsigned int n = 0; n<vcell_gp; n++) {
                            coordinate_phase_space_time coordinate = coordinate_phase_space_time( static_cast<int>(i+1),static_cast<int>(l),static_cast<int>(m),static_cast<int>(n),static_cast<int>(o));
                            auto iteratorForward = forwardPDF.find(coordinate);
                            auto iteratorBackward = backwardPDF.find(coordinate);
                            if(iteratorForward != forwardPDF.end()) {
                                forwardPDFdouble[o][l][m][n] = iteratorForward->second;
                            } else {
                                forwardPDFdouble[o][l][m][n] = 0.0;
                            }
                            if( iteratorBackward != backwardPDF.end() ) {
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
                            gradient(i-1,0) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V1_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            gradient(i-1,1) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V2_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                            gradient(i-1,2) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V3_current*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                        }
                    }
                }
            }
        }
    });



    /*
     * Assemble gradient
     */

    std::cout << "Gradient:" << std::endl;
    std::cout << gradient << std::endl;

    for(unsigned int j = 0; j < dimensionOfControl_gp; j++) {
        gradient(j,0) = weight_control_gp*control(j,0) + gradient(j,0);
        gradient(j,1) = weight_control_gp*control(j,1) + gradient(j,1);
        gradient(j,2) = weight_control_gp*control(j,2) + gradient(j,2);
    }

    return gradient;

}

