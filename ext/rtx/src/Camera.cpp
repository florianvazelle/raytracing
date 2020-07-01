#include <rtx/Camera.h>
#include <rtx/Point.h>

using namespace rtx;

/**
 * Retourne le rayon correspondant au point de l'écran (x,y). Ces coordonnées
 * sont exprimées en ratio de largeur et hauteur totale, et sont donc toutes
 * deux comprises dans l'intervalle [0;1].
 */
Ray Camera::getRay(float x, float y) const {
  float w = 1, h = 1;
  float W = 2, H = 2;

  float X = x * (W / w) - W / 2;
  float Y = y * (H / h) - H / 2;

  Vector O(X, -Y, 0);
  Vector F(0, 0, focal);
  Vector V = (O - F).normalized();

  Point origin(O[0], O[1], O[2]);
  origin = localToGlobal(origin);

  Vector direction(V[0], V[1], V[2]);
  direction = localToGlobal(direction);
  direction = direction.normalized();

  Ray ray(origin, direction);
  return ray;
}