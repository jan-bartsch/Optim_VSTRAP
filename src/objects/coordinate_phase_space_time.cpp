#include "coordinate_phase_space_time.h"

coordinate_phase_space_time::coordinate_phase_space_time()
{

}

coordinate_phase_space_time::coordinate_phase_space_time(int cell_id, int vx, int vy, int vz, int time)
{
    this->setPx(-1.0); this->setPy(-1.0); this->setPz(-1.0);
    this->setCell_id(cell_id);
    this->setVx(vx); this->setVy(vy); this->setVz(vz);
    this->setTime(time); this->setNorm(calculateNorm());
}

std::string coordinate_phase_space_time::toString() const
{
    return "{" + std::to_string(time) + ", " + std::to_string(px) + ", "
            + std::to_string(py) + ", "
            + std::to_string(pz) + ", "
            + std::to_string(vx) + ", "
            + std::to_string(vy) + ", "
            + std::to_string(vz) + "}";
}

double coordinate_phase_space_time::calculateNorm()
{
    return this->getPx()*this->getPy() + this->getPy()*this->getPy() + this->getPz()*this->getPz()+
            this->getVx()*this->getVy() + this->getVy()*this->getVy() + this->getVz()*this->getVz() +
            this->getTime()*this->getTime();
}

bool coordinate_phase_space_time::operator==(const coordinate_phase_space_time &coordinate) const
{
    return (coordinate.getPx() == this->getPx() && coordinate.getPy() == this->getPy()
            && coordinate.getPz() == this->getPz() && coordinate.getVx() == this->getVx()
            && coordinate.getVy() == this->getVy() && coordinate.getVz() == this->getVz()
            && coordinate.getTime() == this->getTime());
}

coordinate_phase_space_time coordinate_phase_space_time::operator-(const coordinate_phase_space_time &coordinate) const
{
    return coordinate_phase_space_time(
                this->getTime() - coordinate.getTime(),
               this->getCell_id() - coordinate.getCell_id(),
                this->getVx() - coordinate.getVx(),
                this->getVy() - coordinate.getVy(),
                this->getVz() - coordinate.getVz()
                );
}



int coordinate_phase_space_time::getPx() const
{
    return px;
}

int coordinate_phase_space_time::getPy() const
{
    return py;
}

void coordinate_phase_space_time::setPy(int value)
{
    py = value;
}

int coordinate_phase_space_time::getPz() const
{
    return pz;
}

int coordinate_phase_space_time::getVx() const
{
    return vx;
}

void coordinate_phase_space_time::setVx(int value)
{
    vx = value;
}

int coordinate_phase_space_time::getVy() const
{
    return vy;
}

void coordinate_phase_space_time::setVy(int value)
{
    vy = value;
}

int coordinate_phase_space_time::getVz() const
{
    return vz;
}

void coordinate_phase_space_time::setVz(int value)
{
    vz = value;
}

int coordinate_phase_space_time::getTime() const
{
    return time;
}

void coordinate_phase_space_time::setTime(int value)
{
    time = value;
}

double coordinate_phase_space_time::getNorm() const
{
    return norm;
}

void coordinate_phase_space_time::setNorm(double value)
{
    norm = value;
}

int coordinate_phase_space_time::getCell_id() const
{
    return cell_id;
}

void coordinate_phase_space_time::setCell_id(int value)
{
    cell_id = value;
}

void coordinate_phase_space_time::setPz(int value)
{
    pz = value;
}

void coordinate_phase_space_time::setPx(int value)
{
    px = value;
}
