#ifndef H_PLAN
#define H_PLAN

#include <rtx/Object.h>

namespace rtx {

struct Plan : public Object {
  Plan();
  Plan(std::shared_ptr<Material> mat) : Object(mat){};
  virtual ~Plan() {}

  Ray getNormal(const Point& p, const Point& o) const;
  bool intersect(const Ray& ray, Point& impact) const;
  Point getTextureCoordinates(const Point& p) const;
};

}  // namespace rtx

#endif