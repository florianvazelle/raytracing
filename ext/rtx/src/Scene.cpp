#include <cmath>
#include <iostream>
#include <limits>

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
Object *Scene::getClosestIntersection(const Ray &ray, Point &impact) const {
  Object *closer_obj = nullptr;
  float minDist = std::numeric_limits<float>::max();

  for (Object *obj : objects) {
    if (obj->intersect(ray, impact)) {
      float dist = Vector::distance(ray.origin, impact);
      if (minDist > dist) {
        minDist = dist;
        closer_obj = obj;
      }
    }
  }

  return closer_obj;
};

/**
 * Retourne la couleur correspondant au point (i, j) de l'image.
 */
Color Scene::castRayForPixel(const Camera &cam, float i, float j) const {
  Color color;
  Point impact;
  Ray ray = cam.getRay(i, j);
  Object *obj = getClosestIntersection(ray, impact);

  color = (obj) ? performLighting(ray, *obj, impact) : getBackground();

  return color;
}

/**
 * Retourne la couleur de l'objet intersecté, tout en prenant en compte le
 * lighting.
 */
Color Scene::performLighting(const Ray &ray, const Object &obj,
                             const Point &impact) const {
  Color ambiante = getAmbianteLighting(obj, impact);
  Color diffuse = getDiffuseLighting(ray, obj, impact);
  Color specular = getSpecularLighting(ray, obj, impact);

  return ambiante + diffuse + specular;
}

/**
 * Retourne vrai si un object se situe entre le point d'intersection et la
 * lumière, sinon retourne faux.
 */
bool Scene::isInShadow(const Light &light, const Point &impact) const {
  Ray R = light.getRayToLight(impact);
  Point lightPosition = light.getRayFromLight(impact).origin;

  Point shadowImpact;
  Object *obj = getClosestIntersection(R, shadowImpact);
  if (obj) {
    float objectDistance = Vector::distance(R.origin, shadowImpact);
    float lightDistance = Vector::distance(R.origin, lightPosition);

    return objectDistance < lightDistance;
  }
  return false;
}

/**
 * Retourne la couleur ambiante.
 */
Color Scene::getAmbianteLighting(const Object &obj, const Point &impact) const {
  Material mat = obj.getMaterial(impact);
  return mat.ka * getAmbiant();
}

/**
 * Retourne la couleur diffuse.
 */
Color Scene::getDiffuseLighting(const Ray &ray, const Object &obj,
                                const Point &impact) const {
  Material mat = obj.getMaterial(impact);

  Color diffuse;
  const Ray normal = obj.getNormal(impact, ray.origin);

  for (int i = 0; i < nbLights(); i++) {
    const Light *l = getLight(i);

    if (isInShadow(*l, impact))
      continue;

    Vector N = normal.vector;
    Vector L = l->getVectorToLight(impact);

    float lambertian = std::max(0.f, N.dot(L));
    diffuse += (mat.kd * l->id) * lambertian;
  }

  return diffuse;
}

/**
 * Retourne la couleur speculaire.
 */
Color Scene::getSpecularLighting(const Ray &ray, const Object &obj,
                                 const Point &impact) const {
  Material mat = obj.getMaterial(impact);

  Color specular;
  const Ray normal = obj.getNormal(impact, ray.origin);

  for (int i = 0; i < nbLights(); i++) {
    const Light *l = getLight(i);

    if (isInShadow(*l, impact))
      continue;

    Vector N = normal.vector;
    Vector L = l->getVectorToLight(impact);
    Vector V = -ray.vector;
    Vector R = (N * 2.0f * L.dot(N)) - L;

    float dot = std::max(R.dot(V), 0.0f);

    float specAngle = std::pow(dot, mat.shininess);
    specular += (mat.ks * l->is) * specAngle;
  }

  return specular;
}