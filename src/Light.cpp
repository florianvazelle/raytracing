#include "Light.h"

using namespace rtx;

/**
 * Retourne un Ray partant du Point passé en paramètre et orienté vers la
 * lumière.
 */
Ray Light::getRayToLight(const Point &p) const { return Ray(); }

/**
 * Retourne un Ray partant de la lumière et orienté vers le Point passé en
 * paramètre.
 */
Ray Light::getRayFromLight(const Point &p) const { return Ray(); }

/**
 * Retourne un Vector orienté du Point passé en paramètre vers la lumière.
 */
Vector Light::getVectorToLight(const Point &p) const { return Vector(); }

/**
 * Retourne un Vector orienté de la lumière vers le Point passé en paramètre.
 */
Vector Light::getVectorFromLight(const Point &p) const { return Vector(); }