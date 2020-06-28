#include <rtx/InfiniteCylinder.h>

#include <cmath>

using namespace rtx;

bool InfiniteCylinder::intersect(const Ray &ray, Point &impact) const {
  Vector O = globalToLocal(ray.origin);
  Vector V = globalToLocal(ray.vector);

  float a = V[0] * V[0] + V[2] * V[2];
  float b = V[0] * O[0] + V[2] * O[2];
  float c = O[0] * O[0] + O[2] * O[2] - 1;

  float discr = b * b - a * c;

  if (discr < 0)
    return false;

  // nearest intersection
  float t = (-b - sqrt(discr)) / a;

  // t<0 means the intersection is behind the ray origin
  // which we don't want
  if (t <= 0)
    return false;

  impact[0] = O[0] + t * V[0];
  impact[1] = O[1] + t * V[1];
  impact[2] = O[2] + t * V[2];

  impact = localToGlobal(impact);

  return true;
}

Ray InfiniteCylinder::getNormal(const Point &impact,
                                const Point &observator) const {
  Vector dir = globalToLocal(impact);
  if (observator.dot(dir) < 0)
    dir = -dir;
  dir[1] = 0;
  Ray r;
  r.origin = impact;
  r.vector = localToGlobal(dir).normalized();
  return r;
}