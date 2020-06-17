#include "Object.h"

/**
 * Retourne le Material correspondant au point de la surface de l'objet passé
 * en paramètre.
 */
Material Object::getMaterial(const Point &p) const {}

/**
 * Retourne la normale correspondant au point de la surface de l'objet passé en
 * paramètre, observé depuis le point passé en second paramètre.
 */
Ray Object::getNormal(const Point &p, const Point &o) const {}

/**
 * Calcule si le rayon passé en paramètre intersecte l'objet. Si c'est le cas,
 * met le point d'impact passé en paramètre par référence à jour, et retourne
 * true. Sinon, retourne false.
 */
bool Object::intersect(const Ray &ray, Point &impact) const {}