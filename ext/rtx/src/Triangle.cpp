#include <rtx/Triangle.h>

#include <cmath>

using namespace rtx;

/**
 * Möller–Trumbore intersection algorithm
 */
bool Triangle::intersect(const Ray &ray, Point &impact) const {
  Ray R = globalToLocal(ray);

  const float EPSILON = 0.0000001;
  Vector vertex0 = Vector(1, 1, 0);
  Vector vertex1 = Vector(-1, 1, 0);
  Vector vertex2 = Vector(0, -1, 0);
  Vector edge1, edge2, h, s, q;
  float a, f, u, v;
  edge1 = vertex1 - vertex0;
  edge2 = vertex2 - vertex0;
  h = R.vector.cross(edge2);
  a = edge1.dot(h);
  if (a > -EPSILON && a < EPSILON)
    return false; // This ray is parallel to this triangle.
  f = 1.0 / a;
  s = R.origin - vertex0;
  u = f * s.dot(h);
  if (u < 0.0 || u > 1.0)
    return false;
  q = s.cross(edge1);
  v = f * R.vector.dot(q);
  if (v < 0.0 || u + v > 1.0)
    return false;
  // At this stage we can compute t to find out where the intersection point is
  // on the line.
  float t = f * edge2.dot(q);
  if (t > EPSILON) // ray intersection
  {
    Vector P = localToGlobal(R.origin + R.vector * t);
    impact = Point(P.x, P.y, P.z);
    return true;
  } else // This means that there is a line intersection but not a ray
         // intersection.
    return false;
}