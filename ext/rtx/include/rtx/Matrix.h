#ifndef H_MATRIX
#define H_MATRIX

#include <rtx/Point.h>

namespace rtx {

/**
 * Représente une Matrice 4x4 de transformation.
 */
struct Matrix {
  float data[16];

  Matrix();

  Matrix operator*(const Matrix &rhs) const;
  Vector operator*(const Vector &rhs) const;
  Point operator*(const Point &rhs) const;
  bool operator==(const Matrix &m) const;

  float operator()(int i, int j) const;
  float &operator()(int i, int j);

  // int determinant(const float data[16], int n) const;
  Matrix inverse() const;

  friend std::ostream &operator<<(std::ostream &os, const Matrix &m);
};

} // namespace rtx

#endif