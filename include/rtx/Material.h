#ifndef H_MATERIAL
#define H_MATERIAL

#include <rtx/Vector.h>

namespace rtx {

/**
 * Représente le matériau d'un objet.
 */
struct Material {
  Material();
  Material(const Color& ka, const Color& kd, const Color& ks, const int& shininess = 0,
           const float& reflectivity = 0, const float& refractivity = 0);

  bool operator==(const Material& rhs) const {
    return ka == rhs.ka && kd == rhs.kd && ks == rhs.ks && shininess == rhs.shininess &&
           reflectivity == rhs.reflectivity && refractivity == rhs.refractivity;
  };

  /**
   * La composante ambiante, qui se multiplie par la lumière ambiante de la
   * scène.
   */
  Color ka;
  virtual const Color getAmbiant(const Point& impact) const { return ka; };

  /**
   * La composante diffuse, qui se multiplie par la composante diffuse de chaque
   * lumière.
   */
  Color kd;

  /**
   * La composante spéculaire, qui se multiplie par la composante spéculaire de
   * chaque lumière.
   */
  Color ks;

  /**
   * L'exposant de brillance du matériel, utilisé dans le modèle de Phong.
   */
  int shininess;

  /**
   *
   */
  float reflectivity;

  /**
   *
   */
  float refractivity;
};

}  // namespace rtx

#endif