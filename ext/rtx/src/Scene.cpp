#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <algorithm> 

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
  Color reflectiveRefractive =
      getReflectiveRefractive(ray, obj, impact, raycast);

  return ambiante + diffuse + specular + reflectiveRefractive;
}

/**
 * Retourne vrai si un object se situe entre le point d'intersection et la
 * lumière, sinon retourne faux.
 */
bool Scene::isInShadow(const Light &light, const Point &impact) const {
  Ray shadowFeeler = light.getRayToLight(impact);
  Point lightPosition = light.getRayFromLight(impact).origin;

  Point shadowImpact;
  Object *obj = getClosestIntersection(shadowFeeler, shadowImpact);
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
  const Vector N = obj.getNormal(impact, ray.origin).vector;

  for (int i = 0; i < nbLights(); i++) {
    const Light *l = getLight(i);

    Vector L = l->getVectorToLight(impact);

    if (isInShadow(*l, impact))
      continue;

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
  const Vector N = obj.getNormal(impact, ray.origin).vector;
  const Vector V = -ray.vector;

  for (int i = 0; i < nbLights(); i++) {
    const Light *l = getLight(i);

    Vector L = l->getVectorToLight(impact);

    if (isInShadow(*l, impact))
      continue;

    Vector R = reflect(L, N);

    float dot = std::max(R.dot(V), 0.0f);

    float specAngle = std::pow(dot, mat.shininess);
    specular += (mat.ks * l->is) * specAngle;
  }

  return specular;
}

Color Scene::getReflectiveRefractive(const Ray &ray, const Object &obj,
                                     const Point &impact, int raycast) const {
  if (raycast >= DEPTH_COMPLEXITY)
    return Color();

  const Material mat = obj.getMaterial(impact);

  float reflectivity = mat.reflectivity;
  float refractivity = mat.refractivity;

  const Vector N = obj.getNormal(impact, ray.origin).vector;

  if (refractivity > 0) {
    const float K = schlickFresnel(ray.vector, N, 1.0, refractivity);

    reflectivity = K;
    refractivity = (1 - K);
  }

  Color color;
  if (reflectivity > 0)
    color += getReflectivity(ray, obj, impact, raycast) * reflectivity;

  if (refractivity > 0)
    color += getRefractivity(ray, obj, impact, raycast) * refractivity;

  return color;
}

Color Scene::getReflectivity(const Ray &ray, const Object &obj,
                             const Point &impact, int raycast) const {
  const Vector N = obj.getNormal(impact, ray.origin).vector;

  Vector reflected = reflect(ray.origin, N);
  Ray reflectedRay(impact, reflected);

  return castRay(reflectedRay, raycast + 1);
}

Color Scene::getRefractivity(const Ray &ray, const Object &obj,
                             const Point &impact, int raycast) const {
  const Material mat = obj.getMaterial(impact);

  const Vector N = obj.getNormal(impact, ray.origin).vector;

  Vector refracted = refract(ray.vector, N, 1, mat.refractivity);
  Ray refractedRay(impact, refracted);

  return castRay(refractedRay, DEPTH_COMPLEXITY - 2);
}

/**
 * L’équation de Fresnel permet de déterminer les taux respectifs de
 * réfraction et réflexion en fonction de l’indice de réfraction du matériau.
 * L’approximation de Schlick simplifie l’équation de Fresnel qui ne
 * devient dépendante que du vecteur V qui est le vecteur du rayon incident.
 * https://en.wikipedia.org/wiki/Schlick%27s_approximation
 */
float Scene::schlickFresnel(const Vector &I, const Vector &N, float n1,
                            float n2) const {
  float R = (n1 - n2) / (n1 + n2); // coefficient de réfraction au degré zéro
  R *= R;

  float angle = N.dot(I);
  if (angle > 0) {
    float n = n1 / n2;
    angle = sqrt(1.0f - (n * n * (1.0f - angle * angle)));
  } else {
    angle *= -1;
  }

  return R + (1 - R) * pow(1 - angle, 5);
}

Vector Scene::reflect(const Vector &I, const Vector &N) const {
  return ((N * 2.f * I.dot(N)) - I).normalized();
}

/**
 * Forme vectorielle des lois de Snell-Descartes
 * https://fr.wikipedia.org/wiki/Lois_de_Snell-Descartes#Forme_vectorielle_des_lois_de_Snell-Descartes
 */
Vector Scene::refract(const Vector &I, const Vector &N, float n1,
                      float n2) const {
  float n = n1 / n2;
  float dot = N.dot(-I);
  float R = sqrt(1.0f - ((n * n) * (1.0f - (dot * dot))));

  Vector refracted = I * n + N * (n * dot + ((dot > 0) ? -R : R));
  return refracted.normalized();
}