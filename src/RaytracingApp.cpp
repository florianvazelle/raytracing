#include <iomanip>
#include <thread>

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
    rtx::Point pos = JsonHelper::toPoint(obj["position"]);
    rtx::Point rot = JsonHelper::toPoint(obj["rotation"]);
    float scale = obj["scale"].asFloat();
    rtx::Material mat = JsonHelper::toMaterial(obj["material"]);

    rtx::Object *object;

    if (type == "Cube") {
      object = new rtx::Cube(mat);
    }

    object->scale(scale);
    object->rotateX(rot.x);
    object->rotateY(rot.y);
    object->rotateZ(rot.z);
    object->translate(pos.x, pos.y, pos.z);
    scene.objects.push_back(object);
  }

  const Json::Value ligs = root["lights"];
  for (int i = 0; i < ligs.size(); ++i) {
    const Json::Value lig = ligs[i];

    rtx::Point pos = JsonHelper::toPoint(lig["position"]);
    rtx::Color id = JsonHelper::toColor(lig["id"]);
    rtx::Color is = JsonHelper::toColor(lig["is"]);

    rtx::Light *light = new rtx::Light(id, is);
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
  rtx::Color color;
  rtx::Point impact;
  rtx::Ray ray = cam.getRay(i, j);
  rtx::Object *obj = scene.closer_intersected(ray, impact);

  if (obj) {
    color = rtx::Color(0, 1, 0); // getImpactColor(ray, *obj, impact,
                                 // scene);
  } else {
    color = scene.getBackground();
  }

  return color;
}

Image RaytracingApp::raytracing(const rtx::Scene &scene) const {
  const float width = 25.f;
  const float height = 25.f;

  const float w = 1.f / width;
  const float h = 1.f / height;

  rtx::Camera cam(3.0f);
  cam.translate(0, 0, -3);

  std::vector<rtx::Color> colors;
  colors.resize(width * height);

  // std::vector<std::thread> threads;

  // const float spp = 3.f; // sample par pixels
  const float ws = w / (spp + 1.f);
  const float hs = h / (spp + 1.f);

  for (float j = 0; j < 1.f; j += h) {
    // threads.push_back(std::thread([&] {
    for (float i = 0; i < 1.f; i += w) {
      rtx::Color color;

      // super-sampling
      for (int l = 1.f; l <= spp; l++) {
        for (float k = 1.f; k <= spp; k++) {
          float x = i + (k * ws);
          float y = j + (l * hs);

          color += tracer(scene, cam, x, y);
        }
      }
      color = color / (spp * spp);

      // Correction Gamma
      // color.r = std::min(1.0f, color.r);
      // color.g = std::min(1.0f, color.g);
      // color.b = std::min(1.0f, color.b);

      // float r = powf(color.r, 1.f / 2.2f);
      // float g = powf(color.g, 1.f / 2.2f);
      // float b = powf(color.b, 1.f / 2.2f);

      // colors.push_back(color);
      colors.at((i * width) + (j * height) * width) =
          color; // rtx::Color(r, g, b);
    }
    // }));
  }

  // for (auto &th : threads)
  // th.join();

  return Image(width, height, colors);
}