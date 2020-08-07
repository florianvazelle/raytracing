#ifndef H_SPHERE
#define H_SPHERE

#include <rtx/Object.h>

namespace rtx {

struct Sphere : public Object {
  Sphere();
  Sphere(std::shared_ptr<Material> mat) : Object(mat){};
  virtual ~Sphere() {}

  Ray getNormal(const Point& p, const Point& o) const;
  bool intersect(const Ray& ray, Point& impact) const;
  Point getTextureCoordinates(const Point& p) const;
};

}  // namespace rtx

#endif