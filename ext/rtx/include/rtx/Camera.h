#ifndef H_CAMERA
#define H_CAMERA

#include <rtx/Entity.h>
#include <rtx/Ray.h>

namespace rtx {

/**
 * Hérite de Entity.
 * Représente une caméra dans la scène. La caméra est dans le plan X,Y et fait
 * face points de valeur négative en Z. L'écran de la caméra à des coordonnées
 * qui vont de (-1,-1,0) à (1,1,0) dans son référentiel. Elle contient un
 * attribut réel focal qui correspond a la distance de son centre focal par
 * rapport au centre de l'écran. Par exemple, une valeur de 5 indique que les
 * droites de tous les rayons lancés par cette caméra passent par le point
 * (0,0,5)
 */
struct Camera : Entity {
  int focal;

  Ray getRay(float x, float y) const;
};

} // namespace rtx

#endif