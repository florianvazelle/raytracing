#ifndef H_SCENE
#define H_SCENE

#include <rtx/Camera.h>
#include <rtx/Light.h>
#include <rtx/Object.h>
#include <rtx/Ray.h>
#include <rtx/Vector.h>

#include <memory>
#include <vector>

namespace rtx {

/**
 * Représente la scène a rendre.
 * Contient la liste des objets, ainsi que la liste des lumières.
 * Défini également la valeur de la lumière ambiante, et la valeur de la couleur
 * d'arrière plan (quand un rayon ne touche pas d'objet).
 */
struct Scene {
  const static int DEPTH_COMPLEXITY = 10;
  bool useShadow;
  bool useAmbiantOcclusion;

  std::vector<std::shared_ptr<Object>> objects;
  std::vector<std::shared_ptr<Light>> lights;

  Color getBackground() const;
  Color getAmbiant() const;
  int nbLights() const;
  const std::shared_ptr<Light> getLight(int index) const;
  std::shared_ptr<Object> getClosestIntersection(const Ray& ray, Point& impact) const;

  Color castRayForPixel(const Camera& cam, float i, float j) const;
  Color castRay(const Ray& ray, int raycast = 0) const;

  Color performLighting(const Ray& ray, const Object& obj, const Point& impact, int raycast) const;

  /* Lighting stuff */
  bool isInShadow(const Light& light, const Point& impact) const;
  Color getAmbiantLighting(const Object& obj, const Point& impact) const;
  Color getDiffuseLighting(const Ray& ray, const Object& obj, const Point& impact) const;
  Color getSpecularLighting(const Ray& ray, const Object& obj, const Point& impact) const;

  Color getReflectiveRefractive(const Ray& ray, const Object& obj, const Point& impact,
                                int raycast) const;
  Color getReflectivity(const Ray& ray, const Object& obj, const Point& impact, int raycast) const;
  Color getRefractivity(const Ray& ray, const Object& obj, const Point& impact, int raycast) const;

  float schlickFresnel(const Vector& I, const Vector& N, float n1, float n2) const;
  Vector reflect(const Vector& I, const Vector& N) const;
  Vector refract(const Vector& I, const Vector& N, float n1, float n2) const;

  float ambientOcclusion(const Ray& ray, const Object& obj, const Point& impact, int raycast) const;
};

}  // namespace rtx

#endif