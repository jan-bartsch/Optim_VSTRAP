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
    return "{" + std::to_string(time_) + ", " + std::to_string(px_) + ", "
            + std::to_string(py_) + ", "
            + std::to_string(pz_) + ", "
            + std::to_string(vx_) + ", "
            + std::to_string(vy_) + ", "
            + std::to_string(vz_) + "}";
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
    return px_;
}

int CoordinatePhaseSpaceTime::getPy() const
{
    return py_;
}

void CoordinatePhaseSpaceTime::setPy(int value)
{
    py_ = value;
}

int CoordinatePhaseSpaceTime::getPz() const
{
    return pz_;
}

int CoordinatePhaseSpaceTime::getVx() const
{
    return vx_;
}

void CoordinatePhaseSpaceTime::setVx(int value)
{
    vx_ = value;
}

int CoordinatePhaseSpaceTime::getVy() const
{
    return vy_;
}

void CoordinatePhaseSpaceTime::setVy(int value)
{
    vy_ = value;
}

int CoordinatePhaseSpaceTime::getVz() const
{
    return vz_;
}

void CoordinatePhaseSpaceTime::setVz(int value)
{
    vz_ = value;
}

int CoordinatePhaseSpaceTime::getTime() const
{
    return time_;
}

void CoordinatePhaseSpaceTime::setTime(int value)
{
    time_ = value;
}

int CoordinatePhaseSpaceTime::getCell_id() const
{
    return cell_id_;
}

void CoordinatePhaseSpaceTime::setCell_id(int value)
{
    cell_id_ = value;
}

void CoordinatePhaseSpaceTime::setPz(int value)
{
    pz_ = value;
}

void CoordinatePhaseSpaceTime::setPx(int value)
{
    px_ = value;
}
