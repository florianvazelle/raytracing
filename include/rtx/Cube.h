#ifndef H_CUBE
#define H_CUBE

#include <rtx/Object.h>

namespace rtx {

struct Cube : public Object {
  Cube();
  Cube(std::shared_ptr<Material> mat) : Object(mat){};
  virtual ~Cube() {}

  Ray getNormal(const Point& p, const Point& o) const;
  bool intersect(const Ray& ray, Point& impact) const;
  Point getTextureCoordinates(const Point& p) const;
};

}  // namespace rtx

#endif