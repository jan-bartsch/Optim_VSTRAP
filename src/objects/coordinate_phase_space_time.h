#ifndef COORDINATE_PHASE_SPACE_TIME_H
#define COORDINATE_PHASE_SPACE_TIME_H


#include <string>
#include <cmath>

class coordinate_phase_space_time
{
public:
    coordinate_phase_space_time();

    coordinate_phase_space_time(int cell_id, int vx, int vy, int vz, int time);

    std::string toString() const;

    bool operator==(const coordinate_phase_space_time &coordinate) const;
    coordinate_phase_space_time operator-(const coordinate_phase_space_time &coordinate) const;



    int getPx() const;
    void setPx(int value);

    int getPy() const;
    void setPy(int value);

    int getPz() const;
    void setPz(int value);

    int getVx() const;
    void setVx(int value);

    int getVy() const;
    void setVy(int value);

    int getVz() const;
    void setVz(int value);

    int getTime() const;
    void setTime(int value);

    double getNorm() const;
    void setNorm(double value);

    int getCell_id() const;
    void setCell_id(int value);

private:
    //positions
    int px;
    int py;
    int pz;

    int cell_id;
    //velocities
    int vx;
    int vy;
    int vz;
    //time
    int time;

    //
    double norm;

};

namespace std {
template<>
struct hash<coordinate_phase_space_time>
{
   typedef coordinate_phase_space_time argument_type;
   typedef size_t result_type;

   size_t operator () (const argument_type& x) const
   {
     return (hash<int>()(x.getNorm())^hash<int>()(x.getPx())^hash<int>()(x.getPy())^hash<int>()(x.getPz())^hash<int>()(x.getCell_id())
             ^hash<int>()(x.getVx())^hash<int>()(x.getVy())^hash<int>()(x.getVz())^
             hash<int>()(x.getTime())
             );
     /*return (hash<int>()(x.getNorm())*hash<int>()(x.getPx())*hash<int>()(x.getPy())
             *hash<int>()(x.getVx())*hash<int>()(x.getVy())*hash<int>()(x.getVz())*
             hash<int>()(x.getTime())
             );*/
   }
};
}

#endif // COORDINATE_PHASE_SPACE_TIME_H
