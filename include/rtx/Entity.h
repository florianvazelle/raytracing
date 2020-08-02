#ifndef H_ENTITY
#define H_ENTITY

#include <rtx/Matrix.h>
#include <rtx/Ray.h>

namespace rtx {

/**
 * Base de tout objet déplacable dans la scène : objets, lumières, et caméras.
 * On peut construire un déplacement complexe, en combinant des transformations
 * élémentaires successives.
 *
 * On peut ensuite transformer des Ray, Vector et Point du référentiel local
 * vers global, et global vers local via les méthodes localToGlobal et
 * globalToLocal.
 */
struct Entity {
  Matrix trans, transInv;

  Entity(){};

  void translate(float x, float y, float z);
  void rotateX(float deg);
  void rotateY(float deg);
  void rotateZ(float deg);
  void scale(float factor);

  Vector localToGlobal(const Vector& v) const;
  Point localToGlobal(const Point& p) const;
  Ray localToGlobal(const Ray& r) const;

  Vector globalToLocal(const Vector& v) const;
  Point globalToLocal(const Point& p) const;
  Ray globalToLocal(const Ray& r) const;
};

}  // namespace rtx

#endif