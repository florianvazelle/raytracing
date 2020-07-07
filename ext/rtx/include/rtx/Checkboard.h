#ifndef H_CHECKBOARD
#define H_CHECKBOARD

#include <rtx/Material.h>

#include <cmath>

namespace rtx {

/**
 * Représente le matériau d'un objet.
 */
struct Checkboard : Material {
  Checkboard();
  Checkboard(const Color &ka, const Color &kd, const Color &ks,
             const int &shininess = 0, const float &reflectivity = 0,
             const float &refractivity = 0)
      : Material(ka, kd, ks, shininess, reflectivity, refractivity){};

  const Color getAmbiante(const Point &impact) const {
    float total = std::floor(impact.x) + std::floor(impact.y);
    bool isEven = std::fmod(total, 2.0f) == 0.0f;

    if (isEven) {
      return ka;
    } else {
      return Color(1, 1, 1);
    }
  }
};

} // namespace rtx

#endif