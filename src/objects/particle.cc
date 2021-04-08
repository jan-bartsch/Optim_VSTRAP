#include "particle.h"

#include <cmath>

Particle::Particle() { vx = 0.0; vy = 0.0; vz = 0.0; px = 0.0; py = 0.0; pz = 0.0;
                       //timestep = 0.0;
                       cell_id = -1; weight = 0.0; }

Particle::Particle(double vx, double vy, double vz) {
    Particle::setVx(vx);
    Particle::setVy(vy);
    Particle::setVz(vz);
    //particle::setTimestep(timestep);
    px = 0.0; py = 0.0; pz = 0.0;
    cell_id = -1; weight = 0.0;
}

Particle::Particle(double px, double py, double pz, double vx, double vy, double vz) {
    Particle::setPx(px);
    Particle::setPy(py);
    Particle::setPz(pz);
    Particle::setVx(vx);
    Particle::setVy(vy);
    Particle::setVz(vz);
    Particle::setWeight(weight);
    cell_id = -1;

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
    return sqrt(pow(vx,2.0)+pow(vy,2)+pow(vz,2.0));
}

std::string Particle::toString() {
    return std::to_string(this->getPx()) + " " + std::to_string(this->getPy()) + " " + std::to_string(this->getPz())
            + " " + std::to_string(this->getVx()) + " " + std::to_string(this->getVy()) + " " + std::to_string(this->getVz());
}

double Particle::getPx() const {  return px; }

void Particle::setPx(double value) {  px = value; }

double Particle::getPy() const { return py;}

void Particle::setPy(double value) {  py = value; }

double Particle::getPz() const {  return pz;}

void Particle::setPz(double value) {  pz = value;}

double Particle::getVx() const {   return vx;}

void Particle::setVx(double value) { vx = value;}

double Particle::getVy() const {  return vy;}

double Particle::getVz() const { return vz;}

void Particle::setVz(double value) {  vz = value;}

void Particle::setVy(double value) {  vy = value;}

int Particle::getCell_id() const { return cell_id; }

void Particle::setCell_id(int value) { cell_id = value; }

double Particle::getWeight() const { return weight; }

void Particle::setWeight(double value) { weight = value; }

