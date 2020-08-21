#include "pdf_controller.h"

pdf_controller::pdf_controller()
{

}

int pdf_controller::assemblingMultiDim(std::vector<std::vector<particle>> &particlesTime, unsigned int equationType,
                                       std::unordered_map<coordinate_phase_space_time, double> &pdf)
{
    double scaling = 1.0;

    unsigned int ntimesteps_gp = static_cast<unsigned int>(this->getData_provider_optim().getOptimizationParameters().find("ntimesteps_gp")->second);
    double vmax_gp = this->getData_provider_optim().getOptimizationParameters().find("vmax_gp")->second;

    double fraction_fast_particles_gp = this->getData_provider_optim().getOptimizationParameters().find("fraction_fast_particles_gp")->second;
    double dv_gp = this->getData_provider_optim().getOptimizationParameters().find("dv_gp")->second;
    double vcell_gp = this->getData_provider_optim().getOptimizationParameters().find("vcell_gp")->second;

    std::vector<double> sizeParticles(ntimesteps_gp);
    pdf.clear();

    int too_fast_particles = 0;

    for(unsigned int k = 0; k < ntimesteps_gp; k++) {
        double px,py,pz,vx,vy,vz;
        std::vector<particle> particles;
        coordinate_phase_space_time coordinate;

        int binNumberPx, binNumberPy, binNumberPz, binNumberVx, binNumberVy, binNumberVz;
        int binNumberTime;
        int cell_id;
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
                if (equationType == 0) {
                    too_fast_particles++;
                    if (too_fast_particles >= fraction_fast_particles_gp*particles.size()) {
                        logger::Trace("Too many too fast particles, try to increase velocity bound");
                        return 1;
                    }
                }
            }
        }
    }

    return 0;

}

int pdf_controller::assemblingMultiDim_parallel(std::vector<std::vector<particle>> &particlesTime, unsigned int equationType,
                                                std::vector<std::unordered_map<coordinate_phase_space_time, double>> &pdf_time)
{
    unsigned int ntimesteps_gp = static_cast<unsigned int>(this->getData_provider_optim().getOptimizationParameters().find("ntimesteps_gp")->second);
    double vmax_gp = this->getData_provider_optim().getOptimizationParameters().find("vmax_gp")->second;

    double fraction_fast_particles_gp = this->getData_provider_optim().getOptimizationParameters().find("fraction_fast_particles_gp")->second;
    int too_fast_particles = 0;
    int too_fast_adjoint_particles = 0;

    double dv_gp = this->getData_provider_optim().getOptimizationParameters().find("dv_gp")->second;
    double vcell_gp = this->getData_provider_optim().getOptimizationParameters().find("vcell_gp")->second;

    pdf_time.clear();
    pdf_time.resize(ntimesteps_gp);

    std::vector<std::unordered_map<coordinate_phase_space_time, double>> pdf_time_test(ntimesteps_gp);

    std::vector<double> sizeParticles(ntimesteps_gp);

    int return_flag=0;

    //std::cout << "Using " <<  usedThreads << " threads for assembling pdfs" << std::endl;

//#pragma omp parallel for
    for (unsigned int o=0; o<ntimesteps_gp; o++) {
        //std::cout << "Assembling pdf in timestep " << o << std::endl;
        double px,py,pz,vx,vy,vz;
        std::vector<particle> particles = particlesTime[o];;
        coordinate_phase_space_time coordinate;

        int binNumberVx, binNumberVy, binNumberVz;
        int binNumberTime;
        int cell_id;
        //sizeParticles[o] = particles.size();
        for(unsigned int i = 0; i < particles.size(); i++) {
            px = particles[i].getPx(); py = particles[i].getPy(); pz = particles[i].getPz();
            vx = particles[i].getVx(); vy = particles[i].getVy(); vz = particles[i].getVz();

            if (sqrt(vx*vx+vy*vy+vz*vz) <= vmax_gp ) { //else not needed


                binNumberTime = static_cast<int>(o);

                cell_id = particles[i].getCell_id();

                binNumberVx = static_cast<int>(std::floor(vx/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0))); //adding what is left of zero
                binNumberVy = static_cast<int>(std::floor(vy/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0)));
                binNumberVz = static_cast<int>(std::floor(vz/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0)));

                coordinate = coordinate_phase_space_time(cell_id,binNumberVx,binNumberVy,binNumberVz,binNumberTime);

                if (pdf_time[o].find(coordinate) == pdf_time[o].end()) {
                    pdf_time[o].insert(std::pair<coordinate_phase_space_time,double>(coordinate,1.0));
                } else {
                    pdf_time[o][coordinate]++;
                }
                if (pdf_time_test[o].find(coordinate) == pdf_time_test[o].end()) {
                    pdf_time_test[o].insert(std::pair<coordinate_phase_space_time,double>(coordinate,1.0));
                } else {
                    pdf_time_test[o][coordinate]++;
                }
            } else {
                if (equationType == 0) {
                    too_fast_particles++;
std::cout << "particle at " << coordinate.toString() << " has speed " << sqrt(vx*vx+vy*vy+vz*vz) << std::endl;


                    if (too_fast_particles >= fraction_fast_particles_gp*particles.size()) {
                        //logger::Trace("Too many too fast particles, try to increase velocity bound");
                        return_flag = 1;
                    }
                } else if (equationType == 1)  {
                    too_fast_adjoint_particles++;
                    //logger::Warning("Particle " + std::to_string(i) + " exceeding velocity bound (adjoint equation)");
                }
            }
        }
    }

    if (equationType == 0 && too_fast_particles>0) {
        logger::Info("Particles faster than " + std::to_string(vmax_gp) + ": " + std::to_string(too_fast_particles));
    }
    if (equationType == 1 && too_fast_adjoint_particles>0) {
        logger::Info("Too fast adjoint particles: " + std::to_string(too_fast_adjoint_particles));
    }

if (return_flag == 1) {
	logger::Warning("Too many too fast particles in forward equation.");
}

    return return_flag;
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

    double wasserstein_value = 0.0;

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

double pdf_controller::calculate_wasserstein_metric_histogramm(std::vector<std::unordered_map<coordinate_phase_space_time, double> > dist1, std::vector<std::unordered_map<coordinate_phase_space_time, double> > dist2)
{
    double wasserstein_value = 0.0;
    std::map<std::string,double> optimization_parameters =this->getData_provider_optim().getOptimizationParameters();

    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimization_parameters.find("ntimesteps_gp")->second);
    double vcell_gp = static_cast<unsigned int>(optimization_parameters.find("vcell_gp")->second);
    double dv_gp = static_cast<unsigned int>(optimization_parameters.find("dv_gp")->second);
    double vmax_gp = static_cast<unsigned int>(optimization_parameters.find("vmax_gp")->second);
    double pcell_gp = static_cast<unsigned int>(optimization_parameters.find("pcell_gp")->second);

    std::map<int,std::vector<double>> barycenters = this->getData_provider_optim().getMesh_barycenters();

#pragma omp parallel for
    for (unsigned int o = 0; o < ntimesteps_gp; o++) {
        std::cout << "Wasserstein in " << o << std::endl;
        for (unsigned int i1 = 1; i1<=pcell_gp; i1++) {
            for (unsigned int l1 = 0; l1<vcell_gp; l1++) {
                for (unsigned int m1 = 0; m1<vcell_gp; m1++ ) {
                    for (unsigned int n1 = 0; n1<vcell_gp; n1++) {

                        coordinate_phase_space_time c1(o,i1,l1,m1,n1);

                        for(unsigned int i2 = 1; i2 <= pcell_gp; i2++) {
                            for (unsigned int l2 = 0; l2<vcell_gp; l2++) {
                                for (unsigned int m2 = 0; m2<vcell_gp; m2++ ) {
                                    for (unsigned int n2 = 0; n2<vcell_gp; n2++) {

                                        coordinate_phase_space_time c2(o,i2,l2,m2,n2);


                                        if(dist1[o].find(c1) != dist1[o].end() || dist2[o].find(c2) != dist2[o].end()) {

                                            std::vector<double> c1_bc = barycenters.find(static_cast<int>(i1))->second;

                                            std::vector<double> c2_bc = barycenters.find(static_cast<int>(i2))->second;

                                            //-vmax_gp - (-vmax_gp) = 0
                                            double distance = (c1_bc[0]-c2_bc[0])*(c1_bc[0]-c2_bc[0]) +
                                                    (c1_bc[1]-c2_bc[1])*(c1_bc[1]-c2_bc[1]) +
                                                    (c1_bc[1]-c2_bc[1])*(c1_bc[1]-c2_bc[1]) +
                                                    (l1*dv_gp  - l2*dv_gp)*(l1*dv_gp  - l2*dv_gp)+
                                                    (m1*dv_gp  - m2*dv_gp)*(m1*dv_gp  - m2*dv_gp)+
                                                    (n1*dv_gp  - n2*dv_gp)*(n1*dv_gp  - n2*dv_gp);

                                            double occupation1 = 0.0;
                                            double occupation2 = 0.0;

                                            if(dist1[o].find(c1) != dist1[o].end()) {
                                                occupation1 = dist1[o].at(c1); //else it stays 0
                                            }
                                            if(dist2[o].find(c2) != dist2[o].end()) {
                                                occupation2 = dist2[o].at(c2); //else it stays 0
                                            }

                                            wasserstein_value += distance*(occupation1+occupation2);
                                        }
                                    }
                                }

                            }
                        }

                    }
                }

            }
        }
    }

    std::cout << "Wasserstein distance: " << wasserstein_value << std::endl;
    return wasserstein_value;
}


