#ifndef H_SCENE
#define H_SCENE

#include <json/json.h>
#include <vector>

#include <rtx/Light.h>
#include <rtx/Object.h>
#include <rtx/Ray.h>
#include <rtx/Vector.h>

namespace rtx {

/**
 * Représente la scène a rendre.
 * Contient la liste des objets, ainsi que la liste des lumières.
 * Défini également la valeur de la lumière ambiante, et la valeur de la couleur
 * d'arrière plan (quand un rayon ne touche pas d'objet).
 */
struct Scene {
  std::vector<Object *> objects;
  std::vector<Light *> lights;

  Color getBackground() const;
  Color getAmbiant() const;
  int nbLights() const;
  const Light *getLight(int index) const;
  Object *getClosestIntersection(const Ray &ray, Point &impact) const;

  Color performLighting(const Ray &ray, const Object &obj,
                        const Point &impact) const;
  bool isInShadow(const Light &light, const Point &impact) const;

  Color getAmbianteLighting(const Object &obj, const Point &impact) const;
  Color getDiffuseLighting(const Ray &ray, const Object &obj,
                           const Point &impact) const;
  Color getSpecularLighting(const Ray &ray, const Object &obj,
                            const Point &impact) const;
};

} // namespace rtx

#endif