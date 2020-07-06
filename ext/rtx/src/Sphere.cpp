#include <rtx/Sphere.h>

#define _USE_MATH_DEFINES
#include <cmath>

#ifdef  _USE_MATH_DEFINES

/* Define _USE_MATH_DEFINES before including math.h to expose these macro
 * definitions for common math constants.  These are placed under an #ifdef
 * since these commonly-defined names are not part of the C/C++ standards.
 */

 /* Definitions of useful mathematical constants
  * M_E        - e
  * M_LOG2E    - log2(e)
  * M_LOG10E   - log10(e)
  * M_LN2      - ln(2)
  * M_LN10     - ln(10)
  * M_PI       - pi
  * M_PI_2     - pi/2
  * M_PI_4     - pi/4
  * M_1_PI     - 1/pi
  * M_2_PI     - 2/pi
  * M_2_SQRTPI - 2/sqrt(pi)
  * M_SQRT2    - sqrt(2)
  * M_SQRT1_2  - 1/sqrt(2)
  */

#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.434294481903251827651
#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define M_1_PI     0.318309886183790671538
#define M_2_PI     0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.707106781186547524401

#endif  /* _USE_MATH_DEFINES */

using namespace rtx;

bool Sphere::intersect(const Ray &ray, Point &impact) const {
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

  if (t0 > t1)
    std::swap(t0, t1);

  if (t0 < 0) {
    t0 = t1; // if t0 is negative, let's use t1 instead
    if (t0 < 0)
      return false; // both t0 and t1 are negative
  }

  float t = t0;

  impact[0] = O[0] + t * V[0];
  impact[1] = O[1] + t * V[1];
  impact[2] = O[2] + t * V[2];

  impact = localToGlobal(impact);

  return true;
}

Ray Sphere::getNormal(const Point &impact, const Point &observator) const {
  Vector dir = globalToLocal(impact);
  if (observator.dot(dir) < -1)
    dir = -dir;
  Ray r;
  r.origin = impact;
  r.vector = localToGlobal(dir).normalized();
  return r;
}

Point Sphere::getTextureCoordinates(const Point &p) const {
  static const float TWO_M_PI = 2. * M_PI;

  const Vector dir = globalToLocal(p);

  float u = std::atan2(dir[1], -dir[0]);
  if (u < 0)
    u += TWO_M_PI;
  u = u / TWO_M_PI;

  float v = std::acos(dir[2]) / M_PI;

  return Point(u, v, 0);
}