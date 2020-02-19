#include "gradient_calculator.h"

gradient_calculator::gradient_calculator(const char *filename) {

    this->setData_provider_optim(data_provider(filename));

}

arma::mat gradient_calculator::calculateGradient_forceControl_space_L2(std::vector<std::vector<particle>> forwardParticles, std::vector<std::vector<particle>> backwardParticles, arma::mat control)
{

    std::map<std::string, double> optimizationParameters = this->getData_provider_optim().getOptimizationParameters();

    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(this->getData_provider_optim());




    unsigned int pcell_gp = static_cast<unsigned int>(optimizationParameters.find("pcell_gp")->second);
    unsigned int vcell_gp = static_cast<unsigned int>(optimizationParameters.find("vcell_gp")->second);
    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);
    unsigned int numberParticles_gp = static_cast<unsigned int>(optimizationParameters.find("numberParticles_gp")->second);
    unsigned int dimensionOfControl_gp = static_cast<unsigned int>(optimizationParameters.find("dimensionOfControl_gp")->second);
    double dv_gp = static_cast<double>(optimizationParameters.find("dv_gp")->second);
    double dt_gp = static_cast<double>(optimizationParameters.find("dt_gp")->second);
    double vmax_gp = static_cast<double>(optimizationParameters.find("vmax_gp")->second);
    double weight_control_gp = static_cast<double>(optimizationParameters.find("weight_control_gp")->second);



    arma::mat gradient(pcell_gp,3,arma::fill::zeros);


    //assemble pdfs
    std::unordered_map<coordinate_phase_space_time,double> forwardPDF =pdf_control.assemblingMultiDim(forwardParticles,0);
    std::unordered_map<coordinate_phase_space_time,double> backwardPDF = pdf_control.assemblingMultiDim(backwardParticles,0);


    //Caculate integral in gradient
    int numberThreadsTBB = tbb::task_scheduler_init::default_num_threads();
    int usedThreads = numberThreadsTBB;

    if(numberThreadsTBB > static_cast<int>(pcell_gp)) {
        usedThreads = static_cast<int>(pcell_gp);
    }
    tbb::task_scheduler_init init(usedThreads);

    std::cout << "Using " <<  usedThreads << " threads for assembling gradient" << std::endl;
    const unsigned int n = pcell_gp;
    tbb::parallel_for(static_cast<unsigned int> (0), n , [&]( unsigned int i ) {

        std::cout << "Cell_id: " << i << " " << std::endl;

        std::vector<std::vector<std::vector<std::vector<double>>>> firstDerivativeForwardPDF_V1(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                                (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
        std::vector<std::vector<std::vector<std::vector<double>>>> firstDerivativeForwardPDF_V2(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                                (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
        std::vector<std::vector<std::vector<std::vector<double>>>> firstDerivativeForwardPDF_V3(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                                (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
        std::vector<std::vector<std::vector<std::vector<double>>>> forwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                    (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));
        std::vector<std::vector<std::vector<std::vector<double>>>> backwardPDFdouble(ntimesteps_gp, std::vector<std::vector<std::vector<double>>>
                                                                                     (vcell_gp, std::vector<std::vector<double>> (vcell_gp, std::vector<double> (vcell_gp,0.0))));


        for(unsigned int o = 0; o<ntimesteps_gp; o++) {
            for(unsigned int l = 0; l<vcell_gp; l++) {
                for(unsigned int m= 0; m<vcell_gp; m++) {
                    for(unsigned int n = 0; n<vcell_gp; n++) {
                        coordinate_phase_space_time coordinate = coordinate_phase_space_time( static_cast<int>(i),static_cast<int>(l),static_cast<int>(m),static_cast<int>(n),static_cast<int>(o));
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


        // forwardPDFdouble = manPDF.relaxating_GaussSeidel_2D(forwardPDFdouble,1000);


        for (unsigned int o = 0; o<ntimesteps_gp; o++) {
            for(unsigned int l = 0; l<vcell_gp; l++) {
                for(unsigned int m= 0; m<vcell_gp; m++) {
                    for(unsigned int n = 0; n<vcell_gp; n++) {
                        // derivative v_x
//                        std::cout << o  << std::endl;
//                        std::cout << l  << std::endl;
//                        std::cout << m  << std::endl;
//                        std::cout << n  << std::endl;
                        if (l == 0) {
                            firstDerivativeForwardPDF_V1[o][l][m][n] = (forwardPDFdouble[o][l+1][m][n]-forwardPDFdouble[o][l][m][n])/dv_gp;
                        } else if (l == vcell_gp-1) {
                            firstDerivativeForwardPDF_V1[o][l][m][n] = (forwardPDFdouble[o][l][m][n]-forwardPDFdouble[o][l-1][m][n])/dv_gp;
                        } else {
                            firstDerivativeForwardPDF_V1[o][l][m][n] = (forwardPDFdouble[o][l+1][m][n]-forwardPDFdouble[o][l-1][m][n])/(2.0*dv_gp);
                        }
                        //derivative v_y
                        if (m== 0) {
                            firstDerivativeForwardPDF_V2[o][l][m][n] = (forwardPDFdouble[o][l][m+1][n]-forwardPDFdouble[o][l][m][n])/dv_gp;
                        } else if (m == vcell_gp-1) {
                            firstDerivativeForwardPDF_V2[o][l][m][n] = (forwardPDFdouble[o][l][m][n]-forwardPDFdouble[o][l][m-1][n])/dv_gp;
                        } else {
                            firstDerivativeForwardPDF_V2[o][l][m][n] = (forwardPDFdouble[o][l][m+1][n]-forwardPDFdouble[o][l][m-1][n])/(2.0*dv_gp);
                        }
                        //derivative v_z
                        if (n == 0) {
                            firstDerivativeForwardPDF_V3[o][l][m][n] = (forwardPDFdouble[o][l][m][n+1]-forwardPDFdouble[o][l][m][n])/dv_gp;
                        } else if (n == vcell_gp-1) {
                            firstDerivativeForwardPDF_V3[o][l][m][n] = (forwardPDFdouble[o][l][m][n]-forwardPDFdouble[o][l][m][n-1])/dv_gp;
                        } else {
                            firstDerivativeForwardPDF_V3[o][l][m][n] = (forwardPDFdouble[o][l][m][n+1]-forwardPDFdouble[o][l][m][n-1])/(2.0*dv_gp);
                        }
                        gradient(i,0) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V1[o][l][m][n]*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                        gradient(i,1) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V2[o][l][m][n]*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                        gradient(i,2) += backwardPDFdouble[o][l][m][n]*firstDerivativeForwardPDF_V3[o][l][m][n]*pow(dv_gp,3.0)*pow(dt_gp,1.0);
                        //RieszB_F(i-1) += backwardPDFdouble[o][l]*(1.0*(firstDerivativeForwardPDF[o][l])/(1.0))*pow(dv_gp,dimension_v_gp)*pow(dt_gp,1.0);
                    }
                }
            }
        }
    });



    /*Assemble gradient
     *
     */

    std::cout << gradient << std::endl;

    //gradient_C = manPDF.relaxating_control_GaussSeidel(gradient_C,100);
    // gradient_F = manPDF.relaxating_control_GaussSeidel(gradient_F,100);

    for(unsigned int j = 0; j < dimensionOfControl_gp; j++) {
        gradient(j,0) = weight_control_gp*control(j,0) + gradient(j,0);
        gradient(j,1) = weight_control_gp*control(j,1) + gradient(j,1);
        gradient(j,2) = weight_control_gp*control(j,2) + gradient(j,2);
    }



    std::cout << "Gradient" << std::endl;
    std::cout << gradient.col(1) << std::endl;


    return gradient;

}

