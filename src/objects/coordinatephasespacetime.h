#ifndef COORDINATE_PHASE_SPACE_TIME_H
#define COORDINATE_PHASE_SPACE_TIME_H

#include <cmath>
#include <string>

/**
 * @brief The coordinate_phase_space_time class defines coordinates
 * in the seven dimensional time-phase-space cylinder
 */
class CoordinatePhaseSpaceTime {
public:
  CoordinatePhaseSpaceTime();

  CoordinatePhaseSpaceTime(int cell_id_, int vx_, int vy_, int vz_, int time_);

  std::string toString() const;

  /**
   * @brief operator == overloads the compare operator. Two objects equal
   * if all attributes equal
   *
   * @param coordinate
   * @return
   */
  bool operator==(const CoordinatePhaseSpaceTime &coordinate) const;
  CoordinatePhaseSpaceTime
  operator-(const CoordinatePhaseSpaceTime &coordinate) const;

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

  int getCell_id() const;
  void setCell_id(int value);

private:
  // positions
  int px_;
  int py_;
  int pz_;

  int cell_id_;
  // velocities
  int vx_;
  int vy_;
  int vz_;

  // time
  int time_;
};

namespace std {
template <> struct hash<CoordinatePhaseSpaceTime> {
  typedef CoordinatePhaseSpaceTime argument_type;
  typedef size_t result_type;

  size_t operator()(const argument_type &x) const {
    return (hash<int>()(x.getPx()) ^ hash<int>()(x.getPy()) ^
            hash<int>()(x.getPz()) ^ hash<int>()(x.getCell_id()) ^
            hash<int>()(x.getVx()) ^ hash<int>()(x.getVy()) ^
            hash<int>()(x.getVz()) ^ hash<int>()(x.getTime()));
  }
};
} // namespace std

#endif // COORDINATE_PHASE_SPACE_TIME_H
