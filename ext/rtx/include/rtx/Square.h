#ifndef H_SQUARE
#define H_SQUARE

#include <rtx/Plan.h>

namespace rtx {

struct Square : public Plan {
  Square();
  Square(Material* mat) : Plan(mat){};
  virtual ~Square() {}

  bool intersect(const Ray& ray, Point& impact) const;
  Point getTextureCoordinates(const Point& p) const;
};

}  // namespace rtx

#endif