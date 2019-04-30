#include "point.h"

#include <math.h>

Point::Point() : x(0), y(0) {}
void Point::move(double dx, double dy) {
  x += dx;
  y += dy;
}
double Point::distanceFrom(const Point & p) {
  double distance = sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
  return distance;
}
