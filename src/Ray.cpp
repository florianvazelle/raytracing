#include "Ray.h"

Ray::Ray() {
  origin = Point();
  vector = Vector();
}

Ray::Ray(float ox, float oy, float oz, float vx, float vy, float vz) {
  origin = Point{ox, oy, oz};
  vector = Vector{vx, vy, vz};
}