#include <algorithm>
#include <cmath>
#include <stdexcept>

#include <rtx/Vector.h>

using namespace rtx;

Vector::Vector(const Point &v) {
  x = v.x;
  y = v.y;
  z = v.z;
};

Vector &Vector::operator=(const Vector &rhs) {
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  return *this;
}

Vector &Vector::operator=(const Point &rhs) {
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  return *this;
}

bool Vector::operator==(const Vector &rhs) const {
  return (rhs.x == x && rhs.y == y && rhs.z == z);
}

/**
 * Simple Operator with float
 */

Vector Vector::operator+(const float &rhs) const {
  return {x + rhs, y + rhs, z + rhs};
}

Vector Vector::operator-(const float &rhs) const {
  return {x - rhs, y - rhs, z - rhs};
}

Vector Vector::operator*(const float &rhs) const {
  return {x * rhs, y * rhs, z * rhs};
}

Vector Vector::operator/(const float &rhs) const {
  return {x / rhs, y / rhs, z / rhs};
}

/**
 * Simple Operator with Vector
 */

Vector Vector::operator+(const Vector &rhs) const {
  return {x + rhs.x, y + rhs.y, z + rhs.z};
}

Vector Vector::operator-(const Vector &rhs) const {
  return {x - rhs.x, y - rhs.y, z - rhs.z};
}

Vector Vector::operator*(const Vector &rhs) const {
  return {x * rhs.x, y * rhs.y, z * rhs.z};
}

Vector Vector::operator/(const Vector &rhs) const {
  return {x / rhs.x, y / rhs.y, z / rhs.z};
}

/**
 * Assignement Operator with float
 */

Vector &Vector::operator+=(const float &rhs) {
  x += rhs;
  y += rhs;
  z += rhs;
  return *this;
}

Vector &Vector::operator-=(const float &rhs) {
  x -= rhs;
  y -= rhs;
  z -= rhs;
  return *this;
}

Vector &Vector::operator*=(const float &rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
  return *this;
}

Vector &Vector::operator/=(const float &rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
  return *this;
}

/**
 * Assignement Operator with Vector
 */

Vector &Vector::operator+=(const Vector &rhs) {
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}

Vector &Vector::operator-=(const Vector &rhs) {
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}

Vector &Vector::operator*=(const Vector &rhs) {
  x *= rhs.x;
  y *= rhs.y;
  z *= rhs.z;
  return *this;
}

Vector &Vector::operator/=(const Vector &rhs) {
  x /= rhs.x;
  y /= rhs.y;
  z /= rhs.z;
  return *this;
}

Vector Vector::operator-() const { return {-x, -y, -z}; }

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
  float inv_length = 1.f / length();
  x *= inv_length;
  y *= inv_length;
  z *= inv_length;
  return *this;
}

float Vector::dot(const Vector &rhs) const {
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

float Vector::distance(const Vector &rhs) const {
  float x1 = x - rhs.x;
  float y1 = y - rhs.y;
  float z1 = z - rhs.z;
  return std::sqrt((x1 * x1) + (y1 * y1) + (z1 * z1));
}

Vector Vector::cross(const Vector &v) const {
  return {(y * v.z - z * v.y), -(x * v.z - z * v.x), (x * v.y - y * v.x)};
}

float Vector::norm() const { return x * x + y * y + z * z; }
float Vector::length() const { return std::sqrt(norm()); }

Vector &Vector::saturate() {
  r = std::clamp<float>(r, 0, 1);
  g = std::clamp<float>(g, 0, 1);
  b = std::clamp<float>(b, 0, 1);
  return *this;
}

std::ostream &rtx::operator<<(std::ostream &os, const Vector &f) {
  return os << "(" << f.x << ", " << f.y << ", " << f.z << ")";
}