#include <iomanip>

#include "JsonHelper.h"
#include "RaytracingApp.h"

rtx::Scene RaytracingApp::openScene(std::string path) {
  Json::Value root;

  std::ifstream file;
  file.open(path);

  Json::CharReaderBuilder builder;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, file, &root, &errs)) {
    std::cout << errs << std::endl;
  }

  rtx::Scene scene;

  const Json::Value objs = root["objects"];
  for (int i = 0; i < objs.size(); ++i) {
    const Json::Value obj = objs[i];

    std::string type = obj["type"].asString();
    rtx::Vector pos = JsonHelper::toVector(obj["position"]);
    float scale = obj["scale"].asFloat();
    rtx::Material mat = JsonHelper::toMaterial(obj["material"]);

    rtx::Object *object;

    if (type == "Cube") {
      object = new rtx::Cube(mat);
    }

    object->scale(scale);
    object->translate(pos.x, pos.y, pos.z);
    scene.objects.push_back(object);
  }

  const Json::Value ligs = root["lights"];
  for (int i = 0; i < ligs.size(); ++i) {
    const Json::Value lig = ligs[i];

    rtx::Vector pos = JsonHelper::toVector(lig["position"]);

    rtx::Light *light = new rtx::Light();
    light->translate(pos.x, pos.y, pos.z);
    scene.lights.push_back(light);
  }

  return scene;
}

rtx::Color RaytracingApp::getImpactColor(const rtx::Ray &ray,
                                         const rtx::Object &obj,
                                         const rtx::Point &impact,
                                         const rtx::Scene &scene) const {
  rtx::Material mat = obj.getMaterial(impact);
  rtx::Color ambiante = mat.ka * scene.getAmbiant();

  rtx::Color diffuse;
  const rtx::Ray normal = obj.getNormal(impact, ray.origin);
  for (int i = 0; i < scene.nbLights(); i++) {
    const rtx::Light *l = scene.getLight(i);

    rtx::Vector N = normal.vector;
    rtx::Vector L = l->getVectorToLight(impact);

    float lambertian = std::max(0.f, N.dot(L));
    diffuse += (mat.kd * l->id) * lambertian;
  }
  return ambiante + diffuse;
}

rtx::Color RaytracingApp::tracer(const rtx::Scene &scene,
                                 const rtx::Camera &cam, float i,
                                 float j) const {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "pix(" << i << ", " << j << ")";
  rtx::Color color;
  rtx::Point impact;
  rtx::Ray ray = cam.getRay(i, j);
  rtx::Object *obj = scene.closer_intersected(ray, impact);

  if (obj) {
    color = rtx::Color(0, 1, 0); // getImpactColor(ray, *obj, impact, scene);
    std::cout << " : " << impact;
  } else {
    color = scene.getBackground();
  }

  std::cout << std::endl;

  return color;
}

Image RaytracingApp::raytracing(const rtx::Scene &scene) const {
  const float width = 4.f;
  const float height = 4.f;

  const float w = 1.f / width;
  const float h = 1.f / height;

  rtx::Camera cam(3.0f);
  cam.translate(0, 0, 6);
  std::vector<rtx::Color> colors;

  for (float i = 0.f; i <= 1.f; i += w) {
    for (float j = 0.f; j <= 1.f; j += h) {
      colors.push_back(tracer(scene, cam, i, j));
    }
  }

  return Image(width, height, colors);
}