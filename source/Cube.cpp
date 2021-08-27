#include <rtx/Cube.h>

#include <cmath>

using namespace rtx;

bool Cube::intersect(const Ray& ray, Point& impact) const {
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

  if ((tmin > tymax) || (tymin > tmax)) return false;
  if (tymin > tmin) tmin = tymin;
  if (tymax < tmax) tmax = tymax;

  tzmin = (bounds[sign[2]][2] - O[2]) * invdir[2];
  tzmax = (bounds[!sign[2]][2] - O[2]) * invdir[2];

  if ((tmin > tzmax) || (tzmin > tmax)) return false;
  if (tzmin > tmin) tmin = tzmin;
  if (tzmax < tmax) tmax = tzmax;

  float t = tmin;

  if (t < 0) {
    t = tmax;
    if (t < 0) return false;
  }

  impact[0] = O[0] + t * V[0];
  impact[1] = O[1] + t * V[1];
  impact[2] = O[2] + t * V[2];

  impact = localToGlobal(impact);

  return true;
}

Ray Cube::getNormal(const Point& impact, const Point& observator) const {
  const float EPSILON = 0.00001f;

  Point I = globalToLocal(impact);
  Vector dir = I;

  Vector p = localToGlobal(dir);
  if (observator.dot(p) < -2) dir = -dir;

  for (int i = 0; i < 3; i++) {
    if (-1.f + EPSILON < dir[i] && dir[i] < 1.f - EPSILON) {
      dir[i] = 0.0f;
    } else if (dir[i] < -1.f + EPSILON) {
      dir[i] = -1;
    } else if (1.f - EPSILON < dir[i]) {
      dir[i] = 1;
    }
  }

  Ray r;
  r.origin = impact;
  r.vector = localToGlobal(dir).normalized();
  return r;
}

Point Cube::getTextureCoordinates(const Point& p) const {
  Point locP = globalToLocal(p);
  float x = locP[0], y = locP[1], z = locP[2];

  float absX = fabs(x);
  float absY = fabs(y);
  float absZ = fabs(z);

  int isXPositive = x > 0 ? 1 : 0;
  int isYPositive = y > 0 ? 1 : 0;
  int isZPositive = z > 0 ? 1 : 0;

  float maxAxis, uc, vc, u, v;

  // POSITIVE X
  if (isXPositive && absX >= absY && absX >= absZ) {
    // u (0 to 1) goes from +z to -z
    // v (0 to 1) goes from -y to +y
    maxAxis = absX;
    uc = z;
    vc = y;
  }
  // NEGATIVE X
  if (!isXPositive && absX >= absY && absX >= absZ) {
    // u (0 to 1) goes from -z to +z
    // v (0 to 1) goes from -y to +y
    maxAxis = absX;
    uc = -z;
    vc = y;
  }
  // POSITIVE Y
  if (isYPositive && absY >= absX && absY >= absZ) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from +z to -z
    maxAxis = absY;
    uc = x;
    vc = z;
  }
  // NEGATIVE Y
  if (!isYPositive && absY >= absX && absY >= absZ) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from -z to +z
    maxAxis = absY;
    uc = x;
    vc = -z;
  }
  // POSITIVE Z
  if (isZPositive && absZ >= absX && absZ >= absY) {
    // u (0 to 1) goes from -x to +x
    // v (0 to 1) goes from -y to +y
    maxAxis = absZ;
    uc = x;
    vc = y;
  }
  // NEGATIVE Z
  if (!isZPositive && absZ >= absX && absZ >= absY) {
    // u (0 to 1) goes from +x to -x
    // v (0 to 1) goes from -y to +y
    maxAxis = absZ;
    uc = -x;
    vc = y;
  }

  // Convert range from -1 to 1 to 0 to 1
  u = 0.5f * (uc / maxAxis + 1.0f);
  v = 0.5f * (vc / maxAxis + 1.0f);

  return Point(u, v, 0);
}