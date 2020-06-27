#ifndef H_RAY
#define H_RAY

#include <iostream>

#include <rtx/Point.h>

namespace rtx {

/**
 * Représente un rayon 3D.
 */
struct Ray {
  /**
   * Point représentant l'origine du rayon.
   */
  Point origin;

  /**
   * Vector représentant le vecteur directeur du rayon
   */
  Vector vector;

  Ray();
  Ray(float ox, float oy, float oz, float vx, float vy, float vz);
  Ray(Point origin, Vector vector) : origin(origin), vector(vector){};

  friend std::ostream &operator<<(std::ostream &os, const Ray &r);
};

} // namespace rtx

#endif