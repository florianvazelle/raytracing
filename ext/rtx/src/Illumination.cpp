#include <rtx/Illumination.h>

#include <cmath>

using namespace rtx;

Color Illumination::Lambert(const Ray &ray, const Object &obj,
                            const Point &impact, const Scene &scene) {
  Material mat = obj.getMaterial(impact);
  Color ambiante = mat.ka * scene.getAmbiant();

  Color diffuse;
  const Ray normal = obj.getNormal(impact, ray.origin);
  for (int i = 0; i < scene.nbLights(); i++) {
    const Light *l = scene.getLight(i);

    Vector N = normal.vector;
    Vector L = l->getVectorToLight(impact);

    float lambertian = std::max(0.f, N.dot(L));
    diffuse += (mat.kd * l->id) * lambertian;
  }
  return ambiante + diffuse;
}

Color Illumination::Phong(const Ray &ray, const Object &obj,
                          const Point &impact, const Scene &scene) {
  Material mat = obj.getMaterial(impact);
  Color ambiante = mat.ka * scene.getAmbiant();

  Color diffuse;
  const Ray normal = obj.getNormal(impact, ray.origin);
  for (int i = 0; i < scene.nbLights(); i++) {
    const Light *l = scene.getLight(i);

    Vector N = normal.vector;
    Vector L = l->getVectorToLight(impact);

    float lambertian = std::max(0.f, N.dot(L));
    diffuse += (mat.kd * l->id) * lambertian;
  }

  Color specular;
  for (int i = 0; i < scene.nbLights(); i++) {
    const Light *l = scene.getLight(i);

    Vector N = normal.vector;
    Vector L = l->getVectorToLight(impact);

    Vector V = -ray.vector;
    Vector R = (N * 2.0f * L.dot(N)) - L;

    float dot = std::max(R.dot(V), 0.0f);

    float specAngle = std::pow(dot, mat.shininess);
    specular += (mat.ks * l->is) * specAngle;
  }

  return ambiante + diffuse + specular;
}