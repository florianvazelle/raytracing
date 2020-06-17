#ifndef H_SCENE
#define H_SCENE

#include <vector>

#include "Color.h"
#include "Light.h"
#include "Object.h"
#include "Ray.h"

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
  Object *closer_intersected(const Ray &ray, Point &impact) const;
};

#endif