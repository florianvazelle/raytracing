#ifndef H_OBJECT
#define H_OBJECT

#include "Material.h"
#include "Ray.h"
#include "Entity.h"

/**
 * Hérite de Entity.
 * Classe servant de base à tous les objets de la scène : sphère,
 * cube, plan....
 *
 * Tous les Point, Vector et Ray sont exprimés dans le référentiel global.
 */
struct Object : public Entity {
  virtual Material getMaterial(const Point &p) const = 0;
  virtual bool intersect(const Ray &ray, Point &impact) const = 0;
  virtual Ray getNormal(const Point &p, const Point &o) const = 0;
  virtual ~Object() {}
};

#endif