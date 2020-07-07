#ifndef H_LINE
#define H_LINE

#include <rtx/Material.h>

#include <cmath>

namespace rtx {

/**
 * Représente le matériau d'un objet.
 */
struct Line : Material {
  float scale;

  Line() : Material(), scale(1){};
  Line(const Color &ka, const Color &kd, const Color &ks,
       const int &shininess = 0, const float &reflectivity = 0,
       const float &refractivity = 0, const float &scale = 1)
      : Material(ka, kd, ks, shininess, reflectivity, refractivity),
        scale(scale){};

  const Color getAmbiante(const Point &impact) const {
    float total = std::floor(impact.x / scale);
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