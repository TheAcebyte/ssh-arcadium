#pragma once

#include "lib/types.hpp"

struct Point {
  pt x;
  pt y;

  Point();
  Point(pt x, pt y);

  bool operator==(Point other) const;
  bool operator<(Point other) const;
};
