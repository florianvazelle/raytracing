#include <rtx/Sphere.h>

#include <math.h>

using namespace rtx;

bool Sphere::intersect(const Ray& ray, Point& impact) const {
  Vector O = globalToLocal(ray.origin);
  Vector V = globalToLocal(ray.vector);

  float a = V.dot(V);
  float b = 2 * V.dot(O);
  float c = O.dot(O) - 1;

  float t0, t1;

  float discr = b * b - 4 * a * c;
  if (discr < 0)
    return false;
  else if (discr == 0)
    t0 = t1 = -0.5 * b / a;
  else {
    float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
    t0 = q / a;
    t1 = c / q;
  }

  if (t0 > t1) std::swap(t0, t1);

  if (t0 < 0) {
    t0 = t1;                   // if t0 is negative, let's use t1 instead
    if (t0 < 0) return false;  // both t0 and t1 are negative
  }

  float t = t0;

  impact[0] = O[0] + t * V[0];
  impact[1] = O[1] + t * V[1];
  impact[2] = O[2] + t * V[2];

  impact = localToGlobal(impact);

  return true;
}

Ray Sphere::getNormal(const Point& impact, const Point& observator) const {
  Vector dir = globalToLocal(impact);
  if (observator.dot(dir) < -1) dir = -dir;
  Ray r;
  r.origin = impact;
  r.vector = localToGlobal(dir).normalized();
  return r;
}

Point Sphere::getTextureCoordinates(const Point& p) const {
  static const float TWO_M_PI = 2. * M_PI;

  const Vector dir = globalToLocal(p);

  float u = std::atan2(dir[1], -dir[0]);
  if (u < 0) u += TWO_M_PI;
  u = u / TWO_M_PI;

  float v = std::acos(dir[2]) / M_PI;

  return Point(u, v, 0);
}