#ifndef H_COLOR
#define H_COLOR

/**
 * Représente une couleur, en trois composantes RGB, accessibles comme dans un
 * tableau de 3 réels. Les valeurs sont comprises entre 0.0 et 1.0.
 */
struct Color {
  float r, g, b, a;

  Color() : r(0), g(0), b(0), a(0){};
  Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a){};
};

#endif