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
            }
        }
    }



    return pdf;

}


