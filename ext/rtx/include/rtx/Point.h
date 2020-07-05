#ifndef H_POINT
#define H_POINT

#include <ostream>

#include <rtx/Vector.h>

namespace rtx {

struct Vector;

struct Point {
  float x, y, z;

  Point() : x(0), y(0), z(0){};
  Point(float x, float y, float z) : x(x), y(y), z(z){};
  Point(const Point &p) : x(p.x), y(p.y), z(p.z){};
  Point(const Vector &v);

  Point &operator=(const Point &rhs);
  Point &operator=(const Vector &rhs);

  bool operator==(const Point &rhs) const;

  Point operator+(const float &rhs) const;
  Point operator-(const float &rhs) const;
  Point operator*(const float &rhs) const;
  Point operator/(const float &rhs) const;

  Point operator+(const Point &rhs) const;
  Point operator-(const Point &rhs) const;
  Point operator*(const Point &rhs) const;
  Point operator/(const Point &rhs) const;

  Point &operator+=(const float &rhs);
  Point &operator-=(const float &rhs);
  Point &operator*=(const float &rhs);
  Point &operator/=(const float &rhs);

  Point &operator+=(const Point &rhs);
  Point &operator-=(const Point &rhs);
  Point &operator*=(const Point &rhs);
  Point &operator/=(const Point &rhs);

  Point operator-() const;

  float operator[](const int &rhs) const;
  float &operator[](const int &rhs);

  float norm() const;
  float length() const;
  Point &normalized();
  float dot(const Point &rhs) const;
  Point cross(const Point &rhs) const;
  float distance(const Point &rhs) const;

  friend std::ostream &operator<<(std::ostream &os, const Point &p);
};

} // namespace rtx

#endif