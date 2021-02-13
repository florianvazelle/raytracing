#include "RaytracingApp.h"

#include <rtx/Cube.h>
#include <rtx/InfiniteCylinder.h>
#include <rtx/Plan.h>
#include <rtx/Sphere.h>
#include <rtx/Square.h>
#include <rtx/Triangle.h>

#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <thread>

#include "../common/ThreadPool.h"

void RaytracingApp::raytracing(rtx::Scene& scene, GLTexture& texture) const {
  rtx::Camera cam(3.0f);
  cam.translate(0, 0, 3);

  // create thread pool with the number of concurrent threads supported
  ThreadPool pool(5);
  std::vector<std::future<void>> results;

  scene.useShadow = useShadow;
  scene.useAmbiantOcclusion = useAmbiantOcclusion;

  int x = 0;
  int y = 0;
  int w = _width;
  int h = 1;

  for (y = 0; y < _height; y++) {
    // enqueue and store future
    results.emplace_back(pool.enqueue(
        /**
         *  Trace all ray.
         *  Apply super sampling & correction gamma.
         */
        [this](GLTexture::View view, const rtx::Scene& scene, const rtx::Camera& cam) {
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
        },
        texture.view(x, y, w, h), scene, cam));
  }

  for (auto&& result : results) result.get();
}