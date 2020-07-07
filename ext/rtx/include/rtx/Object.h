#ifndef H_OBJECT
#define H_OBJECT

#include <rtx/Entity.h>
#include <rtx/Material.h>
#include <rtx/Ray.h>

namespace rtx {

/**
 * Hérite de Entity.
 * Classe servant de base à tous les objets de la scène : sphère,
 * cube, plan....
 *
 * Tous les Point, Vector et Ray sont exprimés dans le référentiel global.
 */
struct Object : public Entity {
  Material *material;

  Object(){};
  Object(Material *mat) : material(mat){};
  virtual ~Object(){};

  bool operator==(const Object &rhs) const {
    return trans == rhs.trans && material == rhs.material;
  };

  /**
   * Retourne le Material correspondant au point de la surface de l'objet passé
   * en paramètre.
   */
  Material getMaterial(const Point &p) const {
    // On crée un materiaux de transition, le reste du code peut ainsi rester
    // inchangé

    const Color ka = material->getAmbiante(p);
    const Color kd = material->kd;
    const Color ks = material->ks;

    const int shininess = material->shininess;
    const float reflectivity = material->reflectivity;
    const float refractivity = material->refractivity;

    return Material(ka, kd, ks, shininess, reflectivity, refractivity);
  };

  /**
   * Retourne la normale correspondant au point de la surface de l'objet passé
   * en paramètre, observé depuis le point passé en second paramètre.
   */
  virtual Ray getNormal(const Point &p, const Point &o) const = 0;

  /**
   * Calcule si le rayon passé en paramètre intersecte l'objet. Si c'est le cas,
   * met le point d'impact passé en paramètre par référence à jour, et retourne
   * true. Sinon, retourne false.
   */
  virtual bool intersect(const Ray &ray, Point &impact) const = 0;
};

} // namespace rtx

#endif