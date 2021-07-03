#ifndef POSITION_HPP
#define POSITION_HPP
#include "random.hpp"

namespace smooth_sim
{
class Position
{
  private:
    double x;
    double y;

  public:
    Position(double X, double Y); // Constructor
    Position();                   // Default constructor
    bool InRadius(Position other,double r) const; //determine if two position are close enough
    void move_toward(Position target, double speed, Random& rng); // move position closer to target
    double distance_to(Position& a) const;                        // return the distance from the 2 Position
    double get_x() const;                                         // return the x coordinate
    double get_y() const;                                         // return the y coordinate
};

Position rand_pos(Position blh_corner, Position trh_corner,Random& engine); // get a random position
} // namespace smooth_sim

#endif // POSITION_HPP