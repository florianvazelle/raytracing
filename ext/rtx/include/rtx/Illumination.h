#ifndef H_ILLUMINATION
#define H_ILLUMINATION

#include <rtx/Object.h>
#include <rtx/Scene.h>

namespace rtx {

struct Illumination {
  static Color Lambert(const Ray &ray, const Object &obj, const Point &impact,
                       const Scene &scene);
  static Color Phong(const Ray &ray, const Object &obj, const Point &impact,
                     const Scene &scene);
};

} // namespace rtx

#endif