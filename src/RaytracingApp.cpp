#include "RaytracingApp.h"
#include "JsonHelper.h"

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
    rtx::Material mat = JsonHelper::toMaterial(obj["material"]);

    rtx::Object *object;

    if (type == "Cube") {
      object = new rtx::Cube(mat);
    }

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

Image RaytracingApp::raytracing(rtx::Scene scene) {
  const float width = 800.f;
  const float height = 600.f;

  rtx::Camera cam;
  std::vector<rtx::Color> colors;

  for (float i = 0.f; i < width; i++) {
    for (float j = 0.f; j < height; j++) {
      rtx::Point impact;
      rtx::Ray ray = cam.getRay(i, j);
      rtx::Object *obj = scene.closer_intersected(ray, impact);

      if (obj) {
        colors.push_back(getImpactColor(ray, *obj, impact, scene));
      } else {
        colors.push_back(scene.getBackground());
      }
    }
  }

  return Image(width, height, colors);
}