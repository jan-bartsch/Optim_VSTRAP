#include "coordinatephasespacetime.h"

CoordinatePhaseSpaceTime::CoordinatePhaseSpaceTime() { }

CoordinatePhaseSpaceTime::CoordinatePhaseSpaceTime(int cell_id, int vx, int vy, int vz, int time)
{
    this->setPx(-1.0); this->setPy(-1.0); this->setPz(-1.0);
    this->setCell_id(cell_id);
    this->setVx(vx); this->setVy(vy); this->setVz(vz);
    this->setTime(time);
}

std::string CoordinatePhaseSpaceTime::toString() const
{
    return "{" + std::to_string(time) + ", " + std::to_string(px) + ", "
            + std::to_string(py) + ", "
            + std::to_string(pz) + ", "
            + std::to_string(vx) + ", "
            + std::to_string(vy) + ", "
            + std::to_string(vz) + "}";
}

bool CoordinatePhaseSpaceTime::operator==(const CoordinatePhaseSpaceTime &coordinate) const
{
    return (coordinate.getPx() == this->getPx() && coordinate.getPy() == this->getPy()
            && coordinate.getPz() == this->getPz() && coordinate.getVx() == this->getVx()
            && coordinate.getVy() == this->getVy() && coordinate.getVz() == this->getVz()
            && coordinate.getTime() == this->getTime());
}

CoordinatePhaseSpaceTime CoordinatePhaseSpaceTime::operator-(const CoordinatePhaseSpaceTime &coordinate) const
{
    return CoordinatePhaseSpaceTime(
               this->getCell_id() - coordinate.getCell_id(),
                this->getVx() - coordinate.getVx(),
                this->getVy() - coordinate.getVy(),
                this->getVz() - coordinate.getVz(),
                this->getTime() - coordinate.getTime()
                );
}



int CoordinatePhaseSpaceTime::getPx() const
{
    return px;
}

int CoordinatePhaseSpaceTime::getPy() const
{
    return py;
}

void CoordinatePhaseSpaceTime::setPy(int value)
{
    py = value;
}

int CoordinatePhaseSpaceTime::getPz() const
{
    return pz;
}

int CoordinatePhaseSpaceTime::getVx() const
{
    return vx;
}

void CoordinatePhaseSpaceTime::setVx(int value)
{
    vx = value;
}

int CoordinatePhaseSpaceTime::getVy() const
{
    return vy;
}

void CoordinatePhaseSpaceTime::setVy(int value)
{
    vy = value;
}

int CoordinatePhaseSpaceTime::getVz() const
{
    return vz;
}

void CoordinatePhaseSpaceTime::setVz(int value)
{
    vz = value;
}

int CoordinatePhaseSpaceTime::getTime() const
{
    return time;
}

void CoordinatePhaseSpaceTime::setTime(int value)
{
    time = value;
}

int CoordinatePhaseSpaceTime::getCell_id() const
{
    return cell_id;
}

void CoordinatePhaseSpaceTime::setCell_id(int value)
{
    cell_id = value;
}

void CoordinatePhaseSpaceTime::setPz(int value)
{
    pz = value;
}

void CoordinatePhaseSpaceTime::setPx(int value)
{
    px = value;
}
