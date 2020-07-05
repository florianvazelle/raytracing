#include <rtx/Triangle.h>

#include <cmath>

using namespace rtx;

/**
 * Möller–Trumbore intersection algorithm
 */
bool Triangle::intersect(const Ray &ray, Point &impact) const {
  Point O = globalToLocal(ray.origin);
  Vector V = globalToLocal(ray.vector);

  const float EPSILON = 0.0000001;

  Point vertex0 = Point(-1, 1, 0);
  Point vertex1 = Point(1, 1, 0);
  Point vertex2 = Point(0, -1, 0);
  Vector edge1, edge2, h, s, q;

  float a, f, u, v;
  edge1 = vertex1 - vertex0;
  edge2 = vertex2 - vertex0;

  h = V.cross(edge2);
  a = edge1.dot(h);

  if (a > -EPSILON && a < EPSILON)
    return false;

  f = 1.0 / a;
  s = O - vertex0;
  u = f * s.dot(h);

  if (u < 0.0 || u > 1.0)
    return false;

  q = s.cross(edge1);
  v = f * V.dot(q);

  if (v < 0.0 || u + v > 1.0)
    return false;

  float t = f * edge2.dot(q);
  if (t > EPSILON) {
    impact[0] = O[0] + t * V[0];
    impact[1] = O[1] + t * V[1];
    impact[2] = O[2] + t * V[2];

    impact = localToGlobal(impact);
    return true;
  }

  return false;
}