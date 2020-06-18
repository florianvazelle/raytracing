#include <rtx/Cube.h>

using namespace rtx;

bool Cube::intersect(const Ray &ray, Point &impact) const {
  Vector O = globalToLocal(ray.origin);
  Vector V = globalToLocal(ray.vector);
  Vector invdir = Vector(1.0f / V[0], 1.0f / V[1], 1.0f / V[2]);

  Vector bounds[2] = {Vector(-1, -1, -1), Vector(1, 1, 1)};

  bool sign[3] = {(invdir[0] < 0), (invdir[1] < 0), (invdir[2] < 0)};

  float tmin, tmax, tymin, tymax, tzmin, tzmax;

  tmin = (bounds[sign[0]][0] - O[0]) * invdir[0];
  tmax = (bounds[!sign[0]][0] - O[0]) * invdir[0];
  tymin = (bounds[sign[1]][1] - O[1]) * invdir[1];
  tymax = (bounds[!sign[1]][1] - O[1]) * invdir[1];

  if ((tmin > tymax) || (tymin > tmax))
    return false;
  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;

  tzmin = (bounds[sign[2]][2] - O[2]) * invdir[2];
  tzmax = (bounds[!sign[2]][2] - O[2]) * invdir[2];

  if ((tmin > tzmax) || (tzmin > tmax))
    return false;
  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;

  float t = tmin;

  if (t < 0) {
    t = tmax;
    if (t < 0)
      return false;
  }

  impact[0] = O[0] + t * V[0];
  impact[1] = O[1] + t * V[1];
  impact[2] = O[2] + t * V[2];

  impact = localToGlobal(impact);

  return true;
}

Ray Cube::getNormal(const Point &impact, const Point &observator) const {
  Vector dir = globalToLocal(impact);

  Point p = localToGlobal(dir);
  if (observator.dot(p) < -1)
    dir = -dir;

  for (int i = 0; i < 3; i++) {
    if (-1 < dir[i] && dir[i] < 1) {
      dir[i] = 0.0f;
    }
  }

  Ray r;
  r.origin = impact;
  r.vector = localToGlobal(dir).normalized();
  return r;
}

Material Cube::getMaterial(const Point &p) const { return material; }