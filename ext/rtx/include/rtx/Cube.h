#ifndef H_CUBE
#define H_CUBE

#include <rtx/Object.h>

namespace rtx {

struct Cube : public Object {
  Cube();
  Cube(const Material &mat) : Object(mat){};
  virtual ~Cube() {}

  Material getMaterial(const Point &p) const;
  Ray getNormal(const Point &p, const Point &o) const;
  bool intersect(const Ray &ray, Point &impact) const;
};

} // namespace rtx

#endif