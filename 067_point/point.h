#define _CODE_H_

class Point
{
 private:
  double x;
  double y;

 public:
  Point();
  void move(double dx, double dy);
  double distanceFrom(const Point & p);
};
