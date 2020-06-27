#include <cmath>
#include <iostream>
#include <stdexcept>

#include <rtx/Matrix.h>

using namespace rtx;

Matrix::Matrix() {
  for (int i = 0; i < 16; i++) {
    data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
  }
}

float Matrix::operator()(int i, int j) const { return data[i + j * 4]; };
float &Matrix::operator()(int i, int j) { return data[i + j * 4]; };

Matrix Matrix::operator*(const Matrix &rhs) const {
  float sum;
  Matrix res;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      sum = 0.0f;
      for (int k = 0; k < 4; k++) {
        sum = sum + data[i + k * 4] * rhs(k, j);
      }
      res(i, j) = sum;
    }
  }
  return res;
}

Vector Matrix::operator*(const Vector &rhs) const {
  float sum;
  float result[4];
  float rhsData[4] = {rhs.x, rhs.y, rhs.z, 0};

  for (int i = 0; i < 4; i++) {
    sum = 0.0f;
    for (int j = 0; j < 4; j++) {
      sum = sum + data[i + j * 4] * rhsData[j];
    }
    result[i] = sum;
  }
  return Vector(result[0], result[1], result[2]);
}

Point Matrix::operator*(const Point &rhs) const {
  float sum;
  float result[4];
  float rhsData[4] = {rhs.x, rhs.y, rhs.z, 1};

  for (int i = 0; i < 4; i++) {
    sum = 0.0f;
    for (int j = 0; j < 4; j++) {
      sum = sum + data[i + j * 4] * rhsData[j];
    }
    result[i] = sum;
  }
  return Point(result[0], result[1], result[2]);
}

// int Matrix::determinant(const float data[16], int n) const {
//   int det = 0;
//   float submatrix[16];

//   if (n == 2)
//     return ((data[0] * data[5]) - (data[1] * data[4]));
//   else {
//     for (int x = 0; x < n; x++) {
//       int subi = 0;
//       for (int i = 1; i < n; i++) {
//         int subj = 0;
//         for (int j = 0; j < n; j++) {
//           if (j == x)
//             continue;
//           submatrix[subi + subj * 4] = data[i + j * 4];
//           subj++;
//         }
//         subi++;
//       }
//       det =
//           det + (std::pow(-1, x) * data[x * 4] * determinant(submatrix, n -
//           1));
//     }
//   }
//   return det;
// }

Matrix Matrix::inverse() const {
  float inv[16], det;

  inv[0] = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] -
           data[9] * data[6] * data[15] + data[9] * data[7] * data[14] +
           data[13] * data[6] * data[11] - data[13] * data[7] * data[10];

  inv[4] = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] +
           data[8] * data[6] * data[15] - data[8] * data[7] * data[14] -
           data[12] * data[6] * data[11] + data[12] * data[7] * data[10];

  inv[8] = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] -
           data[8] * data[5] * data[15] + data[8] * data[7] * data[13] +
           data[12] * data[5] * data[11] - data[12] * data[7] * data[9];

  inv[12] = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] +
            data[8] * data[5] * data[14] - data[8] * data[6] * data[13] -
            data[12] * data[5] * data[10] + data[12] * data[6] * data[9];

  inv[1] = -data[1] * data[10] * data[15] + data[1] * data[11] * data[14] +
           data[9] * data[2] * data[15] - data[9] * data[3] * data[14] -
           data[13] * data[2] * data[11] + data[13] * data[3] * data[10];

  inv[5] = data[0] * data[10] * data[15] - data[0] * data[11] * data[14] -
           data[8] * data[2] * data[15] + data[8] * data[3] * data[14] +
           data[12] * data[2] * data[11] - data[12] * data[3] * data[10];

  inv[9] = -data[0] * data[9] * data[15] + data[0] * data[11] * data[13] +
           data[8] * data[1] * data[15] - data[8] * data[3] * data[13] -
           data[12] * data[1] * data[11] + data[12] * data[3] * data[9];

  inv[13] = data[0] * data[9] * data[14] - data[0] * data[10] * data[13] -
            data[8] * data[1] * data[14] + data[8] * data[2] * data[13] +
            data[12] * data[1] * data[10] - data[12] * data[2] * data[9];

  inv[2] = data[1] * data[6] * data[15] - data[1] * data[7] * data[14] -
           data[5] * data[2] * data[15] + data[5] * data[3] * data[14] +
           data[13] * data[2] * data[7] - data[13] * data[3] * data[6];

  inv[6] = -data[0] * data[6] * data[15] + data[0] * data[7] * data[14] +
           data[4] * data[2] * data[15] - data[4] * data[3] * data[14] -
           data[12] * data[2] * data[7] + data[12] * data[3] * data[6];

  inv[10] = data[0] * data[5] * data[15] - data[0] * data[7] * data[13] -
            data[4] * data[1] * data[15] + data[4] * data[3] * data[13] +
            data[12] * data[1] * data[7] - data[12] * data[3] * data[5];

  inv[14] = -data[0] * data[5] * data[14] + data[0] * data[6] * data[13] +
            data[4] * data[1] * data[14] - data[4] * data[2] * data[13] -
            data[12] * data[1] * data[6] + data[12] * data[2] * data[5];

  inv[3] = -data[1] * data[6] * data[11] + data[1] * data[7] * data[10] +
           data[5] * data[2] * data[11] - data[5] * data[3] * data[10] -
           data[9] * data[2] * data[7] + data[9] * data[3] * data[6];

  inv[7] = data[0] * data[6] * data[11] - data[0] * data[7] * data[10] -
           data[4] * data[2] * data[11] + data[4] * data[3] * data[10] +
           data[8] * data[2] * data[7] - data[8] * data[3] * data[6];

  inv[11] = -data[0] * data[5] * data[11] + data[0] * data[7] * data[9] +
            data[4] * data[1] * data[11] - data[4] * data[3] * data[9] -
            data[8] * data[1] * data[7] + data[8] * data[3] * data[5];

  inv[15] = data[0] * data[5] * data[10] - data[0] * data[6] * data[9] -
            data[4] * data[1] * data[10] + data[4] * data[2] * data[9] +
            data[8] * data[1] * data[6] - data[8] * data[2] * data[5];

  det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] +
        data[3] * inv[12];

  if (det == 0)
    std::overflow_error("Divide by zero exception");

  det = 1.0 / det;

  Matrix out;
  for (int i = 0; i < 16; i++) {
    out.data[i] = inv[i] * det;
  }

  return out;
}