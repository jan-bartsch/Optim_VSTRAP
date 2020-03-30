#include "pdf_controller.h"

pdf_controller::pdf_controller()
{

}

std::unordered_map<coordinate_phase_space_time, double> pdf_controller::assemblingMultiDim(std::vector<std::vector<particle>> &particlesTime, unsigned int equationType)
{
    std::unordered_map<coordinate_phase_space_time,double> pdf;


    double px,py,pz,vx,vy,vz;
    std::vector<particle> particles;
    coordinate_phase_space_time coordinate;

    int binNumberPx, binNumberPy, binNumberPz, binNumberVx, binNumberVy, binNumberVz;
    int binNumberTime;
    int cell_id;

    double scaling = 1.0;

    unsigned int ntimesteps_gp = static_cast<unsigned int>(this->getData_provider_optim().getOptimizationParameters().find("ntimesteps_gp")->second);
    double vmax_gp = this->getData_provider_optim().getOptimizationParameters().find("vmax_gp")->second;

    double dp_gp = this->getData_provider_optim().getOptimizationParameters().find("dp_gp")->second;
    double dv_gp = this->getData_provider_optim().getOptimizationParameters().find("dv_gp")->second;
    double vcell_gp = this->getData_provider_optim().getOptimizationParameters().find("vcell_gp")->second;

    std::vector<double> sizeParticles(ntimesteps_gp);

    for(unsigned int k = 0; k < ntimesteps_gp; k++) {
        //std::cout << "Assembling for equation_type " << equationType << std::endl;
        particles = particlesTime[k];
        sizeParticles[k] = particles.size();
        for(unsigned int i = 0; i < particles.size(); i++) {
            if (equationType == 1) {
                //scaling =  1.0/(1+timeDiscr_gp(ntimesteps_gp-1-k)); //vmax_gp/pow(10,29);
            }
            px = particles[i].getPx(); py = particles[i].getPy(); pz = particles[i].getPz();
            vx = particles[i].getVx()*scaling; vy = particles[i].getVy()*scaling; vz = particles[i].getVz()*scaling;
            // error handling

            //Back transformation zeta


            if (sqrt(vx*vx+vy*vy+vz*vz) <= vmax_gp ) { //else not needed

                binNumberTime = static_cast<int>(k);

                //                binNumberPx = static_cast<int>(std::floor(px/dp_gp)); //adding what is left of zero
                //                binNumberPy = static_cast<int>(std::floor(py/dp_gp));
                //                binNumberPz = static_cast<int>(std::floor(pz/dp_gp));

                cell_id = particles[i].getCell_id();

                binNumberVx = static_cast<int>(std::floor(vx/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0))); //adding what is left of zero
                binNumberVy = static_cast<int>(std::floor(vy/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0)));
                binNumberVz = static_cast<int>(std::floor(vz/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0)));

                coordinate = coordinate_phase_space_time(cell_id,binNumberVx,binNumberVy,binNumberVz,binNumberTime);

                if (pdf.find(coordinate) == pdf.end()) {
                    pdf.insert(std::pair<coordinate_phase_space_time,double>(coordinate,1.0));
                } else {
                    pdf[coordinate]++;
                }
            } else {
                // std::cout << "Particle " + std::to_string(i) + " exceeding velocity bound" << std::endl;
            }
        }
    }



    return pdf;

}

std::vector<std::vector<std::vector<std::vector<double>>>> pdf_controller::relaxating_GaussSeidel_4D(std::vector<std::vector<std::vector<std::vector<double>>>> pdf,
                                                                                                     unsigned int numberOfRelaxationSteps)
{

    double tolerance_relaxation_gp = this->getData_provider_optim().getOptimizationParameters().find("tolerance_relaxation_gp")->second;
    unsigned int ntimesteps_gp = static_cast<unsigned int>(this->getData_provider_optim().getOptimizationParameters().find("ntimesteps_gp")->second);
    double vmax_gp = this->getData_provider_optim().getOptimizationParameters().find("vmax_gp")->second;
    double dt_gp = this->getData_provider_optim().getOptimizationParameters().find("dt_gp")->second;
    double dp_gp = this->getData_provider_optim().getOptimizationParameters().find("dp_gp")->second;
    double dv_gp = this->getData_provider_optim().getOptimizationParameters().find("dv_gp")->second;
    double vcell_gp = this->getData_provider_optim().getOptimizationParameters().find("vcell_gp")->second;


    std::vector<std::vector<std::vector<std::vector<double>>>> rhs = pdf;

    std::vector<std::vector<std::vector<std::vector<double>>>> relaxated_pdf_New = rhs;
    //relaxated_pdf_New.assign(pdf.begin(),pdf.end());
    std::vector<std::vector<std::vector<std::vector<double>>>> relaxated_pdf_Old = rhs;
    //relaxated_pdf_Old.assign(pdf.begin(),pdf.end());

    unsigned int n = 0;
    double norm = 1.0;


    double c_s = 1000.0;

    while(n <= numberOfRelaxationSteps && norm > tolerance_relaxation_gp  ) {
        //relaxated_pdf_New.assign(relaxated_pdf_Old.begin(),relaxated_pdf_Old.end());
        for(unsigned int o = 1; o <= ntimesteps_gp -2; o++) {
            for (unsigned int l = 1; l <= vcell_gp - 2; l++) {
                for (unsigned int m = 1; m <= vcell_gp - 2; m++) {
                    for (unsigned int n = 1; n <= vcell_gp - 2; n++) {
                        relaxated_pdf_New[o][l][m][n] = 1.0/(1.0+c_s*2.0/(dt_gp*dt_gp)+c_s*6.0/(dv_gp*dv_gp))*(rhs[o][l][m][n]
                                                                                                               +c_s/(dv_gp*dv_gp)*(relaxated_pdf_New[o][l+1][m][n] + relaxated_pdf_New[o][l-1][m][n]
                                +relaxated_pdf_New[o][l][m+1][n] + relaxated_pdf_New[o][l][m-1][n]
                                +relaxated_pdf_New[o][l][m][n+1] + relaxated_pdf_New[o][l][m][n-1])
                                +c_s/(dt_gp*dt_gp)*(relaxated_pdf_New[o+1][l][m][n]+relaxated_pdf_New[o-1][l][m][n]));
                        //Savitzky Golay filter ?
                    }
                }
            }
        }
        norm = 0.0;
        for(unsigned int o = 1; o <= ntimesteps_gp -2; o++) {
            for (unsigned int l = 1; l <= vcell_gp - 2; l++) {
                for (unsigned int m = 1; m <= vcell_gp - 2; m++) {
                    for (unsigned int n = 1; n <= vcell_gp - 2; n++) {
                        norm += pow(relaxated_pdf_New[o][l][m][n]-relaxated_pdf_Old[o][l][m][n],2.0);
                    }
                }
            }
        }
        norm = std::sqrt(norm);
        relaxated_pdf_Old.assign(relaxated_pdf_New.begin(),relaxated_pdf_New.end());
        n++;
    }

    return relaxated_pdf_New;
}

double pdf_controller::calculate_wasserstein_metric(std::vector<std::vector<particle> > dist1, std::vector<std::vector<particle> > dist2)
{
    unsigned int ntimesteps_gp = static_cast<unsigned int>(this->getData_provider_optim().getOptimizationParameters().find("ntimesteps_gp")->second);
    unsigned int numberParticles = static_cast<unsigned int>(dist1[0].size());

    double wasserstein_value = 0;

    for(unsigned int o = 0; o<ntimesteps_gp; o++) {
        for(unsigned int p = 0; p<numberParticles; p++) {
            wasserstein_value += pow(dist1[o][p].getPx()-dist2[o][p].getPx(),2.0) +
                    pow(dist1[o][p].getPy()-dist2[o][p].getPy(),2.0)+
                     pow(dist1[o][p].getPz()-dist2[o][p].getPz(),2.0)+
                     pow(dist1[o][p].getVx()-dist2[o][p].getVx(),2.0)+
                     pow(dist1[o][p].getVx()-dist2[o][p].getVx(),2.0)+
                     pow(dist1[o][p].getVx()-dist2[o][p].getVx(),2.0);
        }
    }

    return wasserstein_value;
}


