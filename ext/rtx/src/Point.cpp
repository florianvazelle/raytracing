#include <cmath>
#include <stdexcept>

#include <rtx/Point.h>

using namespace rtx;

Point::Point(const Vector &v) {
  x = v.x;
  y = v.y;
  z = v.z;
};

Point &Point::operator=(const Point &rhs) {
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  return *this;
}

Point &Point::operator=(const Vector &rhs) {
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  return *this;
}

bool Point::operator==(const Point &rhs) const {
  return (rhs.x == x && rhs.y == y && rhs.z == z);
}

/**
 * Simple Operator with float
 */

Point Point::operator+(const float &rhs) const {
  return {x + rhs, y + rhs, z + rhs};
}

Point Point::operator-(const float &rhs) const {
  return {x - rhs, y - rhs, z - rhs};
}

Point Point::operator*(const float &rhs) const {
  return {x * rhs, y * rhs, z * rhs};
}

Point Point::operator/(const float &rhs) const {
  return {x / rhs, y / rhs, z / rhs};
}

/**
 * Simple Operator with Point
 */

Point Point::operator+(const Point &rhs) const {
  return {x + rhs.x, y + rhs.y, z + rhs.z};
}

Point Point::operator-(const Point &rhs) const {
  return {x - rhs.x, y - rhs.y, z - rhs.z};
}

Point Point::operator*(const Point &rhs) const {
  return {x * rhs.x, y * rhs.y, z * rhs.z};
}

Point Point::operator/(const Point &rhs) const {
  return {x / rhs.x, y / rhs.y, z / rhs.z};
}

/**
 * Assignement Operator with float
 */

Point &Point::operator+=(const float &rhs) {
  x += rhs;
  y += rhs;
  z += rhs;
  return *this;
}

Point &Point::operator-=(const float &rhs) {
  x -= rhs;
  y -= rhs;
  z -= rhs;
  return *this;
}

Point &Point::operator*=(const float &rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
  return *this;
}

Point &Point::operator/=(const float &rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
  return *this;
}

/**
 * Assignement Operator with Point
 */

Point &Point::operator+=(const Point &rhs) {
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}

Point &Point::operator-=(const Point &rhs) {
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}

Point &Point::operator*=(const Point &rhs) {
  x *= rhs.x;
  y *= rhs.y;
  z *= rhs.z;
  return *this;
}

Point &Point::operator/=(const Point &rhs) {
  x /= rhs.x;
  y /= rhs.y;
  z /= rhs.z;
  return *this;
}

Point Point::operator-() const { return {-x, -y, -z}; }

float Point::operator[](const int &rhs) const {
  if (rhs == 0)
    return x;
  if (rhs == 1)
    return y;
  if (rhs == 2)
    return z;
  throw std::out_of_range("Out of Range");
}

float &Point::operator[](const int &rhs) {
  if (rhs == 0)
    return x;
  if (rhs == 1)
    return y;
  if (rhs == 2)
    return z;
  throw std::out_of_range("Out of Range");
}

Point &Point::normalized() {
  float inv_length = 1.f / length();
  x *= inv_length;
  y *= inv_length;
  z *= inv_length;
  return *this;
}

float Point::dot(const Point &rhs) const {
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

float Point::distance(const Point &rhs) const {
  float x1 = x - rhs.x;
  float y1 = y - rhs.y;
  float z1 = z - rhs.z;
  return std::sqrt((x1 * x1) + (y1 * y1) + (z1 * z1));
}

Point Point::cross(const Point &v) const {
  return {(y * v.z - z * v.y), -(x * v.z - z * v.x), (x * v.y - y * v.x)};
}

float Point::norm() const { return x * x + y * y + z * z; }
float Point::length() const { return std::sqrt(norm()); }

std::ostream &rtx::operator<<(std::ostream &os, const Point &f) {
  return os << "(" << f.x << ", " << f.y << ", " << f.z << ")";
}