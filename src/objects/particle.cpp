#include "particle.h"

#include <cmath>

particle::particle() { vx = 0.0; vy = 0.0; vz = 0.0; px = 0.0; py = 0.0; pz = 0.0; timestep = 0.0; cell_id = -1; weight = 0.0; }

particle::particle(double vx, double vy, double vz,  double timestep) {
    particle::setVx(vx);
    particle::setVy(vy);
    particle::setVz(vz);
    particle::setTimestep(timestep);
    px = 0.0; py = 0.0; pz = 0.0;
    cell_id = -1; weight = 0.0;
}

particle::particle(double px, double py, double pz, double vx, double vy, double vz,double weight) {
    particle::setPx(px);
    particle::setPy(py);
    particle::setPz(pz);
    particle::setVx(vx);
    particle::setVy(vy);
    particle::setVz(vz);
    particle::setWeight(weight);
    cell_id = -1;

}

particle::particle(double px, double py, double pz, double vx, double vy, double vz, int cell_id)
{
    particle::setPx(px);
    particle::setPy(py);
    particle::setPz(pz);
    particle::setVx(vx);
    particle::setVy(vy);
    particle::setVz(vz);
    particle::setCell_id(cell_id);

}

bool particle::operator==(const particle &particle) const
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

double particle::getVelocityMagnitudeParticle() {
    return sqrt(pow(vx,2.0)+pow(vy,2)+pow(vz,2.0));
}

std::string particle::toString() {
    return std::to_string(this->getPx()) + " " + std::to_string(this->getPy()) + " " + std::to_string(this->getPz())
            + " " + std::to_string(this->getVx()) + " " + std::to_string(this->getVy()) + " " + std::to_string(this->getVz());
}

/*bool particle::operator==(const particle &particle) const
{
    return (particle.getPx() == this->getPx() && particle.getPy() == this->getPy()
            && particle.getPz() == this->getPz() && particle.getVx() == this->getVx()
            && particle.getVy() == this->getVy() && particle.getVz() == this->getVz()
            && particle.getTimestep() == this->getTimestep()
            && particle.getWeight() == this->getWeight() && particle.getCell_id() == this->getCell_id()
            );

}*/

double particle::getPx() const {  return px; }

void particle::setPx(double value) {  px = value; }

double particle::getPy() const { return py;}

void particle::setPy(double value) {  py = value; }

double particle::getPz() const {  return pz;}

void particle::setPz(double value) {  pz = value;}

double particle::getVx() const {   return vx;}

void particle::setVx(double value) { vx = value;}

double particle::getVy() const {  return vy;}

double particle::getVz() const { return vz;}

void particle::setVz(double value) {  vz = value;}

void particle::setVy(double value) {  vy = value;}

double particle::getTimestep() const { return timestep; }

void particle::setTimestep(double value) { timestep = value;}

int particle::getCell_id() const { return cell_id; }

void particle::setCell_id(int value) { cell_id = value; }

double particle::getWeight() const { return weight; }

void particle::setWeight(double value) { weight = value; }

