#include "Matrix.h"

using namespace rtx;

float Matrix::operator()(int i, int j) const { return data[i + j * 4]; };
float &Matrix::operator()(int i, int j) { return data[i + j * 4]; };

Matrix Matrix::operator*(const Matrix &rhs) const {
  int sum;
  Matrix res;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      sum = 0;
      for (int k = 0; k < 4; k++) {
        sum = sum + data[i + k * 4] * rhs(k, j);
      }
      res(i, j) = sum;
    }
  }
  return res;
}

Vector Matrix::operator*(const Vector &rhs) const {
  int sum;
  Vector res;
  for (int i = 0; i < 3; i++) {
    sum = 0;
    for (int j = 0; j < 3; j++) {
      sum = sum + data[i + j * 4] * rhs[j];
    }
    res[i] = sum + data[i + 4 * 4];
  }
  return res;
}

Matrix Matrix::inverse() const {
  Matrix inv;
  return inv;
}