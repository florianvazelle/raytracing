#ifndef H_MATERIAL
#define H_MATERIAL

#include <rtx/Vector.h>

namespace rtx {

/**
 * Représente le matériau d'un objet.
 */
struct Material {

  Material() : shininess(0){};
  Material(int shininess) : shininess(shininess){};
  Material(Color ka, Color kd, Color ks, int shininess)
      : ka(ka), kd(kd), ks(ks), shininess(shininess){};

  /**
   * La composante ambiante, qui se multiplie par la lumière ambiante de la
   * scène.
   */
  Color ka;

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
};

} // namespace rtx

#endif