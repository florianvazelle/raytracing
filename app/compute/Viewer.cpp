#include <cxxopts.hpp>

#include <rtx/Cube.h>
#include <rtx/InfiniteCylinder.h>
#include <rtx/Plan.h>
#include <rtx/Sphere.h>
#include <rtx/Square.h>
#include <rtx/Camera.h>
#include <rtx/Triangle.h>
#include <rtx/parser/JsonParser.h>

#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <thread>
#include <iostream>
#include <string>

#include "../common/ThreadPool.h"
#include "../common/GLTexture.h"


bool hasEnding(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

bool getSize(std::string const& fullString, int& width, int& height) {
  std::size_t found = fullString.find('x');

  if (found == std::string::npos) return false;

  std::string w = fullString.substr(0, found); 
  std::string h = fullString.substr(found + 1); 

  width = std::stoi(w);
  height = std::stoi(h);

  return true;
}

template <typename Iterator>
std::string join(Iterator begin, Iterator end, char separator = '.')
{
    std::ostringstream o;
    if(begin != end)
    {
        o << *begin++;
        for(;begin != end; ++begin)
            o  << separator << *begin;
    }
    return o.str();
}

int main(int argc , char** argv) {
  cxxopts::Options options(argv[0], "A ray tracing program to render a scene !");
  options.positional_help("FILE").show_positional_help();

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("o,output", "Path to save the output image", cxxopts::value<std::string>()->default_value("./output.png"), "FILE")
    ("scene", "Scene to render", cxxopts::value<std::string>())
    ("s,size", "Size of the output image", cxxopts::value<std::string>()->default_value("64x64"), "[width]x[height]")
    ("ao", "Use ambient occlusion", cxxopts::value<bool>()->default_value("false"))
    ("spp", "Number of sample per pixel", cxxopts::value<int>()->default_value("1"))
  ;
  // clang-format on

  options.parse_positional({"scene"});

  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help();
    return 0;
  }

  std::string scene_path = "";
  if (result.count("scene")) {
    scene_path = result["scene"].as<std::string>();
  } else {
    std::cout << "You must specify a scene to load.\n"
              << "Use « compute --help » for more information.\n";
    return 0;
  }

  std::string supported_input_extensions[] = {"json"};
  bool is_supported_input = false;
  for (const std::string& extension : supported_input_extensions) {
     is_supported_input = is_supported_input || hasEnding(scene_path, extension);
  }

  if (!is_supported_input) {
    std::cout << "The specified scene is not supported.\n";
    std::cout << "The supported scenes are : ";
    std::cout << join(begin(supported_input_extensions), end(supported_input_extensions), ',');
    std::cout << ".\n";
    return 0;
  }

  int width, height;
  if (!getSize(result["size"].as<std::string>(), width, height)) {
    std::cout << "The specified size is not valid.\n";
    return 0;
  }

  std::string output_path = result["output"].as<std::string>();
  std::string supported_output_extensions[] = {"png", "jpg"};
  bool is_supported_output = false;
  for (const std::string& extension : supported_output_extensions) {
     is_supported_output = is_supported_output || hasEnding(output_path, extension);
  }

  if (!is_supported_output) {
    std::cout << "The specified output extension is not supported.\n";
    std::cout << "The supported output extensions are : ";
    std::cout << join(begin(supported_output_extensions), end(supported_output_extensions), ',');
    std::cout << ".\n";
    return 0;
  }

  int spp = result["spp"].as<int>();
  rtx::Scene scene = rtx::parser::parse_json(scene_path.c_str());
  GLTexture texture(width, height);
  
  {
    rtx::Camera cam(3.0f);
    cam.translate(0, 0, 3);

    // create thread pool with the number of concurrent threads supported
    ThreadPool pool(5);
    std::vector<std::future<void>> results;

    int x = 0;
    int y = 0;
    int w = texture.w();
    int h = 1;

    for (y = 0; y < texture.h(); y++) {
      // enqueue and store future
      results.emplace_back(pool.enqueue(
          /**
           *  Trace all ray.
           *  Apply super sampling & correction gamma.
           */
          [&](GLTexture::View view, const rtx::Scene& scene, const rtx::Camera& cam) {
            const float w = 1.f / texture.w();
            const float h = 1.f / texture.h();

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

  texture.save(output_path.c_str());

  return 0;
}