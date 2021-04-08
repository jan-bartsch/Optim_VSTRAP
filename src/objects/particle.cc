#include "particle.h"

#include <cmath>

Particle::Particle() { vx_ = 0.0; vy_ = 0.0; vz_ = 0.0; px_ = 0.0; py_ = 0.0; pz_ = 0.0;
                       //timestep = 0.0;
                       cell_id_ = -1; weight_ = 0.0; }

Particle::Particle(double vx, double vy, double vz) {
    Particle::setVx(vx);
    Particle::setVy(vy);
    Particle::setVz(vz);
    //particle::setTimestep(timestep);
    px_ = 0.0; py_ = 0.0; pz_ = 0.0;
    cell_id_ = -1; weight_ = 0.0;
}

Particle::Particle(double px, double py, double pz, double vx, double vy, double vz) {
    Particle::setPx(px);
    Particle::setPy(py);
    Particle::setPz(pz);
    Particle::setVx(vx);
    Particle::setVy(vy);
    Particle::setVz(vz);
    Particle::setWeight(weight_);
    cell_id_ = -1;

}

Particle::Particle(double px, double py, double pz, double vx, double vy, double vz, int cell_id)
{
    Particle::setPx(px);
    Particle::setPy(py);
    Particle::setPz(pz);
    Particle::setVx(vx);
    Particle::setVy(vy);
    Particle::setVz(vz);
    Particle::setCell_id(cell_id);

}

bool Particle::operator==(const Particle &particle) const
{
    double tol = std::pow(10.0,-10);
    return (std::fabs(particle.getPx() - this->getPx()) < tol &&
            std::fabs(particle.getPy() - this->getPy()) < tol &&
            std::fabs(particle.getPz() - this->getPz()) < tol &&
            std::fabs(particle.getVx() - this->getVx()) < tol &&
            std::fabs(particle.getVy() - this->getVy()) < tol &&
            std::fabs(particle.getVz() - this->getVz()) < tol &&
            particle.getCell_id() == this->getCell_id());
}

double Particle::getVelocityMagnitudeParticle() {
    return sqrt(pow(vx_,2.0)+pow(vy_,2)+pow(vz_,2.0));
}

std::string Particle::toString() {
    return std::to_string(this->getPx()) + " " + std::to_string(this->getPy()) + " " + std::to_string(this->getPz())
            + " " + std::to_string(this->getVx()) + " " + std::to_string(this->getVy()) + " " + std::to_string(this->getVz());
}

double Particle::getPx() const {  return px_; }

void Particle::setPx(double value) {  px_ = value; }

double Particle::getPy() const { return py_;}

void Particle::setPy(double value) {  py_ = value; }

double Particle::getPz() const {  return pz_;}

void Particle::setPz(double value) {  pz_ = value;}

double Particle::getVx() const {   return vx_;}

void Particle::setVx(double value) { vx_ = value;}

double Particle::getVy() const {  return vy_;}

double Particle::getVz() const { return vz_;}

void Particle::setVz(double value) {  vz_ = value;}

void Particle::setVy(double value) {  vy_ = value;}

int Particle::getCell_id() const { return cell_id_; }

void Particle::setCell_id(int value) { cell_id_ = value; }

double Particle::getWeight() const { return weight_; }

void Particle::setWeight(double value) { weight_ = value; }

