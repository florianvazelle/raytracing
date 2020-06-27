#include <rtx/Ray.h>

using namespace rtx;

Ray::Ray() {
  origin = Point();
  vector = Vector();
}

Ray::Ray(float ox, float oy, float oz, float vx, float vy, float vz) {
  origin = Point(ox, oy, oz);
  vector = Vector(vx, vy, vz);
}

// Ray &Ray::operator=(const Ray &rhs) {
//   origin = rhs.origin;
//   vector = rhs.vector;
//   return *this;
// }