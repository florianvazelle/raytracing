#include <rtx/Ray.h>

using namespace rtx;

Ray::Ray() : Ray(0, 0, 0, 0, 0, 0) {}

Ray::Ray(float ox, float oy, float oz, float vx, float vy, float vz) {
  origin = Point(ox, oy, oz);
  vector = Vector(vx, vy, vz);

  /* Move intersection slightly forward to avoid intersecting with itself. */
  origin += (vector / 1000);
}

Ray::Ray(const Point& o, const Vector& v) : Ray(o.x, o.y, o.z, v.x, v.y, v.z) {}

std::ostream& rtx::operator<<(std::ostream& os, const Ray& r) {
  return os << "orig" << r.origin << " dir" << r.vector;
}