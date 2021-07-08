#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "position.hpp"
#include <vector>

namespace smooth_sim
{
// create a Rectangle class to divide the World in n rectangles, that will be clusters
class Rectangle
{
  private:
    Position blh_corner;
    Position trh_corner;

  public:
    Rectangle(Position& blh_corner, Position& trh_corner); // constructor
    Rectangle();

    // return the Area of the rectangle
    double get_area();

    // return the bottom-left corner of the rectangle
    Position get_blh_corner();

    //return the top-right corner of the rectangle
    Position get_trh_corner();

    // Split the rectangle in two randomly uneven rectangle/
    std::vector<Rectangle> split(Random& engine);

    // Divide the Rectangle in n part using Split() function
    std::vector<Rectangle> divide(int areas_to_divide);
};

} // namespace smooth_sim

#endif