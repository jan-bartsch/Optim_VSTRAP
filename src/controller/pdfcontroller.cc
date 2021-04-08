#include "pdfcontroller.h"

PdfController::PdfController() { }

int PdfController::AssemblingMultiDim(std::vector<std::vector<Particle> > &particles_time, unsigned int equation_type,
                                      std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double> > &pdf_time)
{
    int too_fast_particles = 0;
    int too_fast_adjoint_particles = 0;

    pdf_time.clear();
    pdf_time.resize(MOTIONS::params::ntimesteps_gp);

    std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> pdf_time_test(MOTIONS::params::ntimesteps_gp);

    std::vector<double> sizeParticles(MOTIONS::params::ntimesteps_gp);

    int return_flag=0;
    unsigned int o=0;

    double dv_gp = MOTIONS::params::dv_gp;
    double vcell_gp = static_cast<double>(MOTIONS::params::vcell_gp);


    while( o<MOTIONS::params::ntimesteps_gp && return_flag == 0) {
        //std::cout << "Assembling pdf in timestep " << o << std::endl;
        double px,py,pz,vx,vy,vz;
        std::vector<Particle> particles = particles_time[o];;
        CoordinatePhaseSpaceTime coordinate;

        int binNumberVx, binNumberVy, binNumberVz;
        int binNumberTime;
        int cell_id;
        //sizeParticles[o] = particles.size();
        for(unsigned int i = 0; i < particles.size(); i++) {
            px = particles[i].getPx(); py = particles[i].getPy(); pz = particles[i].getPz();
            vx = particles[i].getVx(); vy = particles[i].getVy(); vz = particles[i].getVz();

            if (sqrt(vx*vx+vy*vy+vz*vz) <= MOTIONS::params::ntimesteps_gp ) { //else not needed


                binNumberTime = static_cast<int>(o);

                cell_id = particles[i].getCell_id();

                binNumberVx = static_cast<int>(std::floor(vx/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0))); //adding what is left of zero
                binNumberVy = static_cast<int>(std::floor(vy/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0)));
                binNumberVz = static_cast<int>(std::floor(vz/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0)));

                coordinate = CoordinatePhaseSpaceTime(cell_id,binNumberVx,binNumberVy,binNumberVz,binNumberTime);

                if (pdf_time[o].find(coordinate) == pdf_time[o].end()) {
                    pdf_time[o].insert(std::pair<CoordinatePhaseSpaceTime,double>(coordinate,1.0));
                } else {
                    pdf_time[o][coordinate]++;
                }
                if (pdf_time_test[o].find(coordinate) == pdf_time_test[o].end()) {
                    pdf_time_test[o].insert(std::pair<CoordinatePhaseSpaceTime,double>(coordinate,1.0));
                } else {
                    pdf_time_test[o][coordinate]++;
                }
            } else {
                if (equation_type == 0) {
                    too_fast_particles++;
                    std::cout << "particle at " << coordinate.toString() << " has speed " << sqrt(vx*vx+vy*vy+vz*vz) << std::endl;
                    if (too_fast_particles >= MOTIONS::params::fraction_fast_particles_gp*particles.size()) {
                        //logger::Trace("Too many too fast particles, try to increase velocity bound");
                        return_flag = 1;
                    }
                } else if (equation_type == 1)  {
                    too_fast_adjoint_particles++;
                    //logger::Warning("Particle " + std::to_string(i) + " exceeding velocity bound (adjoint equation)");
                }
            }
        }
        o++;
    }

    if (equation_type == 0 && too_fast_particles>0) {
        logger::Info("Particles faster than " + std::to_string(MOTIONS::params::vmax_gp) + ": " + std::to_string(too_fast_particles));
    }
    if (equation_type == 1 && too_fast_adjoint_particles>0) {
        logger::Info("Too fast adjoint particles: " + std::to_string(too_fast_adjoint_particles));
    }

    if (return_flag == 1) {
        logger::Warning("Too many too fast particles in forward equation.");
    }

    return return_flag;
}

int PdfController::AssemblingMultidimParallel(std::vector<std::vector<Particle>> &particles_time, unsigned int equation_type,
                                              std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> &pdf_time)
{
    unsigned int ntimesteps_gp = MOTIONS::params::ntimesteps_gp;
    double vmax_gp = MOTIONS::params::vmax_gp;

    std::vector<int> too_fast_particles(ntimesteps_gp,0);
    int too_fast_adjoint_particles = 0;

    double dv_gp = MOTIONS::params::dv_gp;
    double vcell_gp = static_cast<double>(MOTIONS::params::vcell_gp);

    pdf_time.clear();
    pdf_time.resize(ntimesteps_gp);

    std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double>> pdf_time_test(ntimesteps_gp);

    std::vector<double> sizeParticles(ntimesteps_gp);

    int return_flag=0;

    //std::cout << "Using " <<  usedThreads << " threads for assembling pdfs" << std::endl;

#pragma omp parallel for
    for (unsigned int o=0; o<ntimesteps_gp; o++) {
        //std::cout << "Assembling pdf in timestep " << o << std::endl;
        double px,py,pz,vx,vy,vz;
        std::vector<Particle> particles = particles_time[o];;
        CoordinatePhaseSpaceTime coordinate;

        int binNumberVx, binNumberVy, binNumberVz;
        int binNumberTime;
        int cell_id;
        //sizeParticles[o] = particles.size();
        unsigned int i = 0;
        while(i < particles.size() && return_flag == 0) {
            px = particles[i].getPx(); py = particles[i].getPy(); pz = particles[i].getPz();
            vx = particles[i].getVx(); vy = particles[i].getVy(); vz = particles[i].getVz();

            //if (sqrt(vx*vx+vy*vy+vz*vz) <= vmax_gp ) { //else not needed
            if(vx < vmax_gp && vy < vmax_gp && vz < vmax_gp) {

                binNumberTime = static_cast<int>(o);

                cell_id = particles[i].getCell_id();

                binNumberVx = static_cast<int>(std::floor(vx/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0))); //adding what is left of zero
                binNumberVy = static_cast<int>(std::floor(vy/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0)));
                binNumberVz = static_cast<int>(std::floor(vz/dv_gp)) +  static_cast<int>(std::floor((vcell_gp/2.0)));

                coordinate = CoordinatePhaseSpaceTime(cell_id,binNumberVx,binNumberVy,binNumberVz,binNumberTime);

                if (pdf_time[o].find(coordinate) == pdf_time[o].end()) {
                    pdf_time[o].insert(std::pair<CoordinatePhaseSpaceTime,double>(coordinate,1.0));
                } else {
                    pdf_time[o][coordinate]++;
                }
                if (pdf_time_test[o].find(coordinate) == pdf_time_test[o].end()) {
                    pdf_time_test[o].insert(std::pair<CoordinatePhaseSpaceTime,double>(coordinate,1.0));
                } else {
                    pdf_time_test[o][coordinate]++;
                }
            } else {
                if (equation_type == 0) {
                    too_fast_particles[o]++;
                    //std::cout << "particle at " << coordinate.toString() << " has speed " << sqrt(vx*vx+vy*vy+vz*vz) << std::endl;
                    if (too_fast_particles[o] >= MOTIONS::params::fraction_fast_particles_gp*particles.size()) {
                        //logger::Trace("Too many too fast particles, try to increase velocity bound");
                        too_fast_particles[o]++;
                        //return_flag = 1;
                        //break;
                    }
                } else if (equation_type == 1)  {
                    too_fast_adjoint_particles++;
                    //logger::Warning("Particle " + std::to_string(i) + " exceeding velocity bound (adjoint equation)");
                }
            }
            i++;
        }
    }

    std::vector<int>::iterator max_too_fast_particles = std::max_element(too_fast_particles.begin(),too_fast_particles.end());
    int number_too_fast_particles = max_too_fast_particles[std::distance(too_fast_particles.begin(),max_too_fast_particles)];

    if (equation_type == 0 && number_too_fast_particles>0) {
        logger::Info("(Maximum) particles faster than " + std::to_string(vmax_gp) + ": " + std::to_string(number_too_fast_particles) + " in iteration "
                     + std::to_string(std::distance(too_fast_particles.begin(),max_too_fast_particles)));
    }
    if (equation_type == 1 && too_fast_adjoint_particles>0) {
        logger::Info("Too fast adjoint particles: " + std::to_string(too_fast_adjoint_particles));
    }

    if (return_flag == 1) {
        logger::Warning("Too many too fast particles in forward equation.");
    }

    return return_flag;
}

DoublePDF PdfController::RelaxatingGaussseidel4D(DoublePDF pdf, unsigned int number_relaxation_steps)
{
    double tolerance_relaxation_gp = MOTIONS::params::tolerance_relaxation_gp;
    unsigned int ntimesteps_gp = MOTIONS::params::ntimesteps_gp;
    double dt_gp = MOTIONS::params::dt_gp;
    double dv_gp = MOTIONS::params::dv_gp;
    double vcell_gp = MOTIONS::params::vcell_gp;


    DoublePDF rhs = pdf;

    DoublePDF relaxated_pdf_New = rhs;
    DoublePDF relaxated_pdf_Old = rhs;

    unsigned int n = 0;
    double norm = 1.0;


    double c_s = 1000.0;

    while(n <= number_relaxation_steps && norm > tolerance_relaxation_gp  ) {
        for(unsigned int o = 1; o <= ntimesteps_gp -2; o++) {
            for (unsigned int l = 1; l <= vcell_gp - 2; l++) {
                for (unsigned int m = 1; m <= vcell_gp - 2; m++) {
                    for (unsigned int n = 1; n <= vcell_gp - 2; n++) {
                        relaxated_pdf_New.at(o,l,m,n) = 1.0/(1.0+c_s*2.0/(dt_gp*dt_gp)+c_s*6.0/(dv_gp*dv_gp))*(rhs.at(o,l,m,n)
                                                                                                               + c_s/(dv_gp*dv_gp)*(relaxated_pdf_New.at(o,l+1,m,n)
                                                                                                               + relaxated_pdf_New.at(o,l-1,m,n)
                                +relaxated_pdf_New.at(o,l,m+1,n) + relaxated_pdf_New.at(o,l,m-1,n)
                                +relaxated_pdf_New.at(o,l,m,n+1) + relaxated_pdf_New.at(o,l,m,n-1))
                                +c_s/(dt_gp*dt_gp)*(relaxated_pdf_New.at(o+1,l,m,n)+relaxated_pdf_New.at(o-1,l,m,n)));
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
                        norm += pow(relaxated_pdf_New.at(o,l,m,n)-relaxated_pdf_Old.at(o,l,m,n),2.0);
                    }
                }
            }
        }
        norm = std::sqrt(norm);
        relaxated_pdf_Old = relaxated_pdf_New; //.assign(relaxated_pdf_New.begin(),relaxated_pdf_New.end());
        ++n;
    }

    return relaxated_pdf_New;
}

double PdfController::CalculateWassersteinMetric(std::vector<std::vector<Particle> > dist1, std::vector<std::vector<Particle> > dist2)
{
    double wasserstein_value = 0.0;

    unsigned int numberParticles = static_cast<unsigned int>(dist1[0].size());
    for(unsigned int o = 0; o<MOTIONS::params::ntimesteps_gp; o++) {
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

double PdfController::CalculateWassersteinMetricHistogramm(std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double> > dist1, std::vector<std::unordered_map<CoordinatePhaseSpaceTime, double> > dist2)
{
    double wasserstein_value = 0.0;

    uint vcell_gp = MOTIONS::params::vcell_gp;
    double dv_gp = MOTIONS::params::dv_gp;
    //double vmax_gp = MOTIONS::params::vmax_gp;
    uint number_cells_position = MOTIONS::params::number_cells_position;

    std::map<int,std::vector<double>> barycenters = this->get_DataProviderOptim().getMeshBarycenters();

#pragma omp parallel for
    for (uint o = 0; o < MOTIONS::params::ntimesteps_gp; o++) {
        std::cout << "Wasserstein in " << o << std::endl;
        for (uint i1 = 1; i1 <= number_cells_position; i1++) {
            for (uint l1 = 0; l1 < vcell_gp; l1++) {
                for (uint m1 = 0; m1 < vcell_gp; m1++ ) {
                    for (uint n1 = 0; n1 < vcell_gp; n1++) {

                        CoordinatePhaseSpaceTime c1(o,i1,l1,m1,n1);

                        for(unsigned int i2 = 1; i2 <= number_cells_position; i2++) {
                            for (unsigned int l2 = 0; l2<vcell_gp; l2++) {
                                for (unsigned int m2 = 0; m2<vcell_gp; m2++ ) {
                                    for (unsigned int n2 = 0; n2<vcell_gp; n2++) {

                                        CoordinatePhaseSpaceTime c2(o,i2,l2,m2,n2);


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


