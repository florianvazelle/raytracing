#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <thread>

#include <rtx/Cube.h>
#include <rtx/InfiniteCylinder.h>
#include <rtx/Plan.h>
#include <rtx/Sphere.h>
#include <rtx/Square.h>
#include <rtx/Triangle.h>

#include "JsonHelper.h"
#include "ThreadPool.h"
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
  for (unsigned int i = 0; i < objs.size(); ++i) {
    const Json::Value obj = objs[i];

    std::string type = obj["type"].asString();
    rtx::Point pos = JsonHelper::toPoint(obj["position"]);
    rtx::Point rot = JsonHelper::toPoint(obj["rotation"]);
    float scale = obj["scale"].asFloat();
    rtx::Material *mat = JsonHelper::toMaterial(obj["material"]);

    std::shared_ptr<rtx::Object> object;

    if (type == "Cube") {
      object = std::make_shared<rtx::Cube>(mat);
    } else if (type == "Plan") {
      object = std::make_shared<rtx::Plan>(mat);
    } else if (type == "Square") {
      object = std::make_shared<rtx::Square>(mat);
    } else if (type == "Sphere") {
      object = std::make_shared<rtx::Sphere>(mat);
    } else if (type == "InfiniteCylinder") {
      object = std::make_shared<rtx::InfiniteCylinder>(mat);
    } else if (type == "Triangle") {
      object = std::make_shared<rtx::Triangle>(mat);
    } else {
      throw std::invalid_argument("Type not supported");
    }

    object->scale(scale);
    object->rotateX(rot.x);
    object->rotateY(rot.y);
    object->rotateZ(rot.z);
    object->translate(pos.x, pos.y, pos.z);
    scene.objects.push_back(object); 
  }

  const Json::Value ligs = root["lights"];
  for (unsigned int i = 0; i < ligs.size(); ++i) {
    const Json::Value lig = ligs[i];

    rtx::Point pos = JsonHelper::toPoint(lig["position"]);
    rtx::Color id = JsonHelper::toColor(lig["id"]);
    rtx::Color is = JsonHelper::toColor(lig["is"]);

    std::shared_ptr<rtx::Light> light = std::make_shared<rtx::Light>(id, is);
    light->translate(pos.x, pos.y, pos.z);
    scene.lights.push_back(light); 
  }

  return scene;
}

void RaytracingApp::traceRays(GLTexture::View view, const rtx::Scene &scene,
                              const rtx::Camera &cam) const {
  const float w = 1.f / _width;
  const float h = 1.f / _height;

  const float ws = w / (spp + 1.f);
  const float hs = h / (spp + 1.f);

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

          color += scene.castRayForPixel(cam, cx, cy);
        }
      }
      color = color / (spp * spp);

      // Correction Gamma
      color.saturate();

      color.r = powf(color.r, 1.f / 2.2f);
      color.g = powf(color.g, 1.f / 2.2f);
      color.b = powf(color.b, 1.f / 2.2f);

      row[i] = color;
    }
  }
}

void RaytracingApp::raytracing(rtx::Scene &scene, int threadsCount,
                               GLTexture &texture) const {

  rtx::Camera cam(3.0f);
  cam.translate(0, 0, 3);

  // create thread pool with the number of concurrent threads supported
  ThreadPool pool;
  std::vector<std::future<void>> results;

  scene.useShadow = useShadow;
  scene.useAmbiantOcclusion = useAmbiantOcclusion;

  int x = 0;
  int y = 0;
  int w = _width;
  int h = 1;

  for (y = 0; y < _height; y++) {
    // enqueue and store future
    results.emplace_back(pool.enqueue([this](GLTexture::View view, const rtx::Scene &scene, const rtx::Camera &cam) {
      const float w = 1.f / _width;
      const float h = 1.f / _height;

      const float ws = w / (spp + 1.f);
      const float hs = h / (spp + 1.f);

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

              color += scene.castRayForPixel(cam, cx, cy);
            }
          }
          color = color / (spp * spp);

          // Correction Gamma
          color.saturate();

          color.r = powf(color.r, 1.f / 2.2f);
          color.g = powf(color.g, 1.f / 2.2f);
          color.b = powf(color.b, 1.f / 2.2f);

          row[i] = color;
        }
      }
    }, texture.view(x, y, w, h), scene, cam));
  }

  for(auto && result: results)
    result.get();
}