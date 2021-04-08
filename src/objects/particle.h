#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>

/**
 * @brief The particle class defines a particle using its position, velocity and weight
 */
class Particle
{

public:

    /*
     * Constructors
     */
    Particle();

    Particle(double vx_, double vy_, double vz_);
    Particle(double px_, double py_, double pz_, double vx_, double vy_, double vz_);
    Particle(double px_, double py_, double pz_, double vx_, double vy_, double vz_, int cell_id_);

    /**
     * @brief operator == overloads the compare operator. Two particles
     * equal if the difference of the values in every component is
     * less than a given tolerance (comparing two doubles)
     *
     * @param particle
     * @return
     */
    bool operator==(const Particle &Particle) const;


    /**
     * @brief getVelocityMagnitudeParticle calculates speed of particles using Euclidean Norm
     *
     * @return
     */
    double getVelocityMagnitudeParticle();

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


    int getCell_id() const;
    void setCell_id(int value);
    double getWeight() const;
    void setWeight(double value);

private:
    double px_;
    double py_;
    double pz_;

    double vx_;
    double vy_;
    double vz_;

    int cell_id_;
    double weight_;
};

#endif // PARTICLE_H
