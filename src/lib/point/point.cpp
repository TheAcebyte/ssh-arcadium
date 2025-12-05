#include "point.hpp"

Point::Point() : Point(0, 0) {}
Point::Point(pt x, pt y) : x(x), y(y) {}

bool Point::operator==(Point other) const { return x == other.x && y == other.y; }
bool Point::operator<(Point other) const {
  if (x != other.x) {
    return x < other.x;
  }

  return y < other.y;
}
