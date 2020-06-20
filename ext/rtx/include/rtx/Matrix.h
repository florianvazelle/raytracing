#ifndef H_MATRIX
#define H_MATRIX

#include <rtx/Vector.h>

namespace rtx {

/**
 * Représente une Matrice 4x4 de transformation.
 */
struct Matrix {
  float data[16];

  Matrix operator*(const Matrix &rhs) const;
  Vector operator*(const Vector &rhs) const;

  float operator()(int i, int j) const;
  float &operator()(int i, int j);

  // int determinant(const float data[16], int n) const;
  Matrix inverse() const;
};

} // namespace rtx

#endif