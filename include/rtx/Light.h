#ifndef H_LIGHT
#define H_LIGHT

#include <rtx/Entity.h>
#include <rtx/Point.h>
#include <rtx/Ray.h>
#include <rtx/Vector.h>

namespace rtx {

/**
 * Hérite de Entity.
 * Représente une lumière ponctuelle dans la scène.
 *
 * Contient également les attributs de type Color id et is, accessibles
 * publiquement, et correspondant respectivement aux composantes diffuses et
 * spéculaires de la lumière. Tous les Point, Vector et Ray sont exprimés dans
 * le référentiel global. Dans son référentiel, la lumière est positionnée à
 * l'origine (0,0,0).
 */
struct Light : public Entity {
  Color id, is;

  Light(){};
  Light(Color id, Color is) : id(id), is(is){};

  Ray getRayToLight(const Point& p) const;
  Ray getRayFromLight(const Point& p) const;
  Vector getVectorToLight(const Point& p) const;
  Vector getVectorFromLight(const Point& p) const;
};

}  // namespace rtx

#endif