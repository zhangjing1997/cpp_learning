#define _CIRCLE_H_
#include "point.h"

class Circle
{
 private:
  Point center;
  const double radius;

 public:
  Circle(Point p, const double d);
  void move(double dx, double dy);
  double intersectionArea(const Circle & otherCircle);
};
