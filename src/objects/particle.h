#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>


class particle
{

private:
    double px;
    double py;
    double pz;

    double vx;
    double vy;
    double vz;

    int cell_id;
    double weight;

    //double timestep;

public:

    /*
     * Constructors
     */
    particle();

    particle(double vx, double vy, double vz);
    particle(double px, double py, double pz, double vx, double vy, double vz);
    particle(double px, double py, double pz, double vx, double vy, double vz, int cell_id);
    bool operator==(const particle &particle) const;


    /**
     * @brief getVelocityMagnitudeParticle calculates speed of particles using Euclidean Norm
     * @return
     */
    double getVelocityMagnitudeParticle();

    /**
     * @brief toString
     * @return
     */
    std::string toString();


    double getPx() const;
    void setPx(double value);
    double getPy() const;
    void setPy(double value);
    double getPz() const;
    void setPz(double value);
    double getVx() const;
    void setVx(double value);
    double getVy() const;
    void setVy(double value);
    double getVz() const;
    void setVz(double value);

    //    double getTimestep() const;
    //    void setTimestep(double value);


    int getCell_id() const;
    void setCell_id(int value);
    double getWeight() const;
    void setWeight(double value);
};

#endif // PARTICLE_H
