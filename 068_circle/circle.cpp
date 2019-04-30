#include "circle.h"

#include <math.h>

Circle::Circle(Point p, const double d) : center(p), radius(d) {}

void Circle::move(double dx, double dy) {
  center.move(dx, dy);
}

double Circle::intersectionArea(const Circle & otherCircle) {
  double d = center.distanceFrom(otherCircle.center);
  double r1 = radius;
  double r2 = otherCircle.radius;
  if (d >= r1 + r2) {
    return 0;  //there is no intersection (strictly speaking)
  }
  if (d <= abs(r1 - r2)) {
    return M_PI * pow(fmin(r1, r2), 2);  //one circle resides in the other one
  }
  double d1 = (pow(r1, 2) - pow(r2, 2) + pow(d, 2)) / (2 * d);
  double d2 = (pow(r2, 2) - pow(r1, 2) + pow(d, 2)) / (2 * d);
  double area1 =
      pow(r1, 2) * acos(d1 / r1) - d1 * sqrt(pow(r1, 2) - pow(d1, 2));  //the fisrt intersector
  double area2 =
      pow(r2, 2) * acos(d2 / r2) - d2 * sqrt(pow(r2, 2) - pow(d2, 2));  //the second intersector
  return area1 + area2;
}
