#ifndef H_POINT
#define H_POINT

#include <ostream>
#include <rtx/Vector.h>

namespace rtx {

struct Point : Vector {
  Point() : Vector(){};
  Point(float x, float y, float z) : Vector(x, y, z){};

  friend std::ostream &operator<<(std::ostream &os, const Point &v) {
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  }
};

} // namespace rtx

#endif