#include <cmath>
#include <stdexcept>

#include <rtx/Vector.h>

using namespace rtx;

Vector Vector::operator*(const float t) const { return {x * t, y * t, z * t}; }

Vector Vector::operator+(const Vector &rhs) const {
  return {x + rhs.x, y + rhs.y, z + rhs.z};
}

Vector Vector::operator+(const float &rhs) const {
  return {x + rhs, y + rhs, z + rhs};
}

Vector &Vector::operator+=(const Vector &rhs) {
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}

Vector Vector::operator-(const Vector &rhs) const {
  return {x - rhs.x, y - rhs.y, z - rhs.z};
}

Vector Vector::operator-(const float &rhs) const {
  return {x - rhs, y - rhs, z - rhs};
}

Vector Vector::operator-() const { return {-x, -y, -z}; }

Vector Vector::operator*(const Vector &rhs) const {
  return {x * rhs.x, y * rhs.y, z * rhs.z};
}

Vector &Vector::operator*=(const Vector &rhs) {
  x *= rhs.x;
  y *= rhs.y;
  z *= rhs.z;
  return *this;
}

Vector &Vector::operator*=(const float &rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
  return *this;
}

Vector Vector::operator/(const float &rhs) const {
  return {x / rhs, y / rhs, z / rhs};
}

float Vector::operator[](const int &rhs) const {
  if (rhs == 0)
    return x;
  if (rhs == 1)
    return y;
  if (rhs == 2)
    return z;
  throw std::out_of_range("Out of Range");
}

float &Vector::operator[](const int &rhs) {
  if (rhs == 0)
    return x;
  if (rhs == 1)
    return y;
  if (rhs == 2)
    return z;
  throw std::out_of_range("Out of Range");
}

Vector &Vector::normalized() {
  float inv_length = 1.f / sqrtf(x * x + y * y + z * z);
  x *= inv_length;
  y *= inv_length;
  z *= inv_length;
  return *this;
}

float Vector::dot(const Vector &rhs) const {
  return x * rhs.x + y * rhs.y + z * rhs.z;
}