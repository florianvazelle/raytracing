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
Color Scene::getAmbiant() const { return Color(1.0f, 1.0f, 1.0f); };

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
  Point closestImpact;
  Object *closestObject = nullptr;
  float minDist = std::numeric_limits<float>::max();

  for (Object *obj : objects) {
    if (obj->intersect(ray, impact)) {
      float dist = ray.origin.distance(impact);
      if (minDist > dist) {
        minDist = dist;
        closestObject = obj;
        closestImpact = impact;
      }
    }
  }

  impact = closestImpact;
  return closestObject;
};

/**
 * Retourne la couleur correspondant au pixel (i, j) de l'image.
 */
Color Scene::castRayForPixel(const Camera &cam, float i, float j) const {
  Ray ray = cam.getRay(i, j);
  return castRay(ray);
}

/**
 * Retourne la couleur correspondant au ray passé en paramètre.
 */
Color Scene::castRay(const Ray &ray, int raycast) const {
  Point impact;
  Object *obj = getClosestIntersection(ray, impact);
  Color color =
      (obj) ? performLighting(ray, *obj, impact, raycast) : getBackground();
  return color;
}

/**
 * Retourne la couleur de l'objet intersecté, tout en prenant en compte le
 * lighting.
 */
Color Scene::performLighting(const Ray &ray, const Object &obj,
                             const Point &impact, int raycast) const {
  Color ambiante = getAmbianteLighting(obj, impact);
  Color diffuse = getDiffuseLighting(ray, obj, impact);
  Color specular = getSpecularLighting(ray, obj, impact);
  Color reflective = getReflectiveLighting(ray, obj, impact, raycast);

  return ambiante + diffuse + specular + reflective;
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
    float objectDistance = impact.distance(shadowImpact);
    float lightDistance = impact.distance(lightPosition);

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
  const Material mat = obj.getMaterial(impact);

  Color diffuse;
  const Ray normal = obj.getNormal(impact, ray.origin);

  for (int i = 0; i < nbLights(); i++) {
    const Light *l = getLight(i);

    Vector N = normal.vector;
    Vector L = l->getVectorToLight(impact);
    float dot = N.dot(L);

    if (dot >= 0.0f) {
      // if (isInShadow(*l, impact))
      //   continue;

      float lambertian = std::max(0.f, N.dot(L));
      diffuse += (mat.kd * l->id) * lambertian;
    }
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

    Vector N = normal.vector;
    Vector L = l->getVectorToLight(impact);
    float dot = N.dot(L);

    if (dot >= 0.0f) {
      // if (isInShadow(*l, impact))
      //   continue;

      Vector V = -ray.vector;
      Vector R = reflect(L, N);

      float dot = std::max(R.dot(V), 0.0f);

      float specAngle = std::pow(dot, mat.shininess);
      specular += (mat.ks * l->is) * specAngle;
    }
  }

  return specular;
}

Color Scene::getReflectiveLighting(const Ray &ray, const Object &obj,
                                   const Point &impact, int raycast) const {
  if (raycast > 1)
    return Color();

  Material mat = obj.getMaterial(impact);

  const float reflectivity = mat.reflectivity;

  const Ray normal = obj.getNormal(impact, ray.origin);
  const Vector N = normal.vector;

  Color reflective;
  if (reflectivity > 0) {
    Vector reflected = reflect(ray.origin, N);
    Ray reflectedRay(impact, reflected);

    reflective = castRay(reflectedRay, raycast + 1) * reflectivity;
  }

  return reflective;
}

Vector Scene::reflect(const Vector &I, const Vector &N) const {
  return (N * 2.f * I.dot(N)) - I;
}