#ifndef H_CUBE
#define H_CUBE

#include "Object.h"

struct Cube : public Object {
  // Cube();
  // virtual ~Cube() {}

  Material getMaterial(const Point &p) const;
  Ray getNormal(const Point &p, const Point &o) const;
  bool intersect(const Ray &ray, Point &impact) const;
};

#endif