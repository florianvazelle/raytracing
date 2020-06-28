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

void RaytracingApp::ray(Image::View view, const rtx::Scene &scene,
                        const rtx::Camera &cam) const {
  const float w = 1.f / _width;
  const float h = 1.f / _height;

  const float ws = w / (spp + 1.f);
  const float hs = h / (spp + 1.f);

  std::cout << view.x << " " << view.y << "\n";

  for (int j = 0; j < view.h; j++) {
    auto row = view[j];
    for (int i = 0; i < view.w; i++) {
      rtx::Color color;
      float x = (i + view.x) * w;
      float y = (j + view.y) * h;

      // super-sampling
      for (int l = 1.f; l <= spp; l++) {
        for (int k = 1.f; k <= spp; k++) {
          float cx = x + (k * ws);
          float cy = y + (l * hs);

          color += tracer(scene, cam, cx, cy);
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

      row[i] = color;
    }
  }
}

Image RaytracingApp::raytracing(const rtx::Scene &scene,
                                int threadsCount = 1) const {

  rtx::Camera cam(3.0f);
  cam.translate(0, 0, -3);

  std::vector<std::thread> threads;

  Image image(_width, _height);

  int x = 0;
  int y = 0;
  int w = _width;
  int h = _height / threadsCount;

  for (int i = 0; i < threadsCount; i++) {
    threads.push_back(std::thread(&RaytracingApp::ray, this,
                                  image.view(x, y, w, h), scene, cam));
    y = y + h;
  }

  for (auto &th : threads)
    th.join();

  return image;
}