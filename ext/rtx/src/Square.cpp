#include <rtx/Square.h>

using namespace rtx;

bool Square::intersect(const Ray &ray, Point &impact) const {
  Vector O = globalToLocal(ray.origin);
  Vector V = globalToLocal(ray.vector);

  float t = -O[2] / V[2];
  if (t > 0) {
    impact[0] = O[0] + t * V[0];
    impact[1] = O[1] + t * V[1];
    impact[2] = O[2] + t * V[2];

    if (-1 <= impact[0] && impact[0] <= 1 && -1 <= impact[1] &&
        impact[1] <= 1) {
      impact = localToGlobal(impact);
      return true;
    }
  }
  return false;
}

Point Square::getTextureCoordinates(const Point &p) const {
  Point tmp = globalToLocal(p);
  for (int i = 0; i < 3; i++) {
    tmp[i] = (tmp[i] + 1.0f) / 2.0f;
  }
  return tmp;
}