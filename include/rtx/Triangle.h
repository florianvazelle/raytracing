#ifndef H_TRIANGLE
#define H_TRIANGLE

#include <rtx/Plan.h>

namespace rtx {

struct Triangle : public Plan {
  Triangle();
  Triangle(Material* mat) : Plan(mat){};
  virtual ~Triangle() {}

  bool intersect(const Ray& ray, Point& impact) const;
  Point getTextureCoordinates(const Point& p) const;
};

}  // namespace rtx

#endif