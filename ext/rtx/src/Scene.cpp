#include <rtx/Cube.h>
#include <rtx/Light.h>
#include <rtx/Scene.h>

using namespace rtx;

/**
 * Retourne la couleur d'arrière plan.
 */
Color Scene::getBackground() const { return Color(); };

/**
 * Retourne la valeur de lumière ambiante.
 */
Color Scene::getAmbiant() const { return Color(1.f, 0.f, 1.f); };

/**
 * Retourne le nombre de lumières dans la scène.
 */
int Scene::nbLights() const { return lights.size(); };

/**
 * Retourne un pointeur vers la nième lumière.
 */
const Light *Scene::getLight(int index) const { return lights.at(index); };

/**
 * Retourne un pointeur vers l'objet intersecté par le rayon passé en paramètre
 * le plus proche, et nullptr si il n'y en a pas. Met à jour le point d'impact
 * passé en paramètre par référence.
 */
Object *Scene::closer_intersected(const Ray &ray, Point &impact) const {
  for (Object *obj : objects) {
    if (obj->intersect(ray, impact)) {
      return obj; // TODO : faire le calcule de distance
    }
  }
  return nullptr;
};