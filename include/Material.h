#ifndef H_MATERIAL
#define H_MATERIAL

#include "Color.h"

/**
 * Représente le matériau d'un objet.
 */
struct Material {
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

#endif