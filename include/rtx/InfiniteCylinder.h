#ifndef H_INFINITECYLINDER
#define H_INFINITECYLINDER

#include <rtx/Object.h>

namespace rtx {

struct InfiniteCylinder : public Object {
  InfiniteCylinder();
  InfiniteCylinder(std::shared_ptr<Material> mat) : Object(mat){};
  virtual ~InfiniteCylinder() {}

  Ray getNormal(const Point& p, const Point& o) const;
  bool intersect(const Ray& ray, Point& impact) const;
  Point getTextureCoordinates(const Point& p) const;
};

}  // namespace rtx

#endif