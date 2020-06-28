#include <rtx/Plan.h>

#include <cmath>

using namespace rtx;

bool Plan::intersect(const Ray &ray, Point &impact) const {
  Vector O = globalToLocal(ray.origin);
  Vector V = globalToLocal(ray.vector);

  float t = -O[2] / V[2];
  if (t > 0) {
    impact[0] = O[0] + t * V[0];
    impact[1] = O[1] + t * V[1];
    impact[2] = O[2] + t * V[2];

    impact = localToGlobal(impact);
    return true;
  }
  return false;
}

Ray Plan::getNormal(const Point &impact, const Point &observator) const {
  Vector dir(0, 0, 1);
  Vector p = localToGlobal(dir);
  if (observator.dot(p) < 1)
    dir = -dir;

  Ray r;
  r.origin = impact;
  r.vector = localToGlobal(dir);
  return r;
}

Point Plan::getTextureCoordinates(const Point &p) const {
  Point tmp = globalToLocal(p);
  for (int i = 0; i < 3; i++) {
    tmp[i] = fmod(tmp[i], 1);
    if (tmp[i] < 0)
      tmp[i]++;
  }
  return tmp;
}