#ifndef H_VECTOR
#define H_VECTOR

#include <ostream>

#include <rtx/Point.h>

namespace rtx {

struct Point;

struct Vector {
  union {
    struct {
      float x, y, z;
    };

    struct {
      float r, g, b;
    };
  };

  Vector() : x(0), y(0), z(0){};
  Vector(float x, float y, float z) : x(x), y(y), z(z){};
  Vector(const Vector &v) : x(v.x), y(v.y), z(v.z){};
  Vector(const Point &v);

  Vector &operator=(const Vector &rhs);
  Vector &operator=(const Point &rhs);

  bool operator==(const Vector &rhs) const;

  Vector operator+(const float &rhs) const;
  Vector operator-(const float &rhs) const;
  Vector operator*(const float &rhs) const;
  Vector operator/(const float &rhs) const;

  Vector operator+(const Vector &rhs) const;
  Vector operator-(const Vector &rhs) const;
  Vector operator*(const Vector &rhs) const;
  Vector operator/(const Vector &rhs) const;

  Vector &operator+=(const float &rhs);
  Vector &operator-=(const float &rhs);
  Vector &operator*=(const float &rhs);
  Vector &operator/=(const float &rhs);

  Vector &operator+=(const Vector &rhs);
  Vector &operator-=(const Vector &rhs);
  Vector &operator*=(const Vector &rhs);
  Vector &operator/=(const Vector &rhs);

  Vector operator-() const;

  float operator[](const int &rhs) const;
  float &operator[](const int &rhs);

  float norm() const;
  float length() const;
  Vector &normalized();
  float dot(const Vector &rhs) const;
  Vector cross(const Vector &rhs) const;
  float distance(const Vector &rhs) const;
  Vector &saturate();

  friend std::ostream &operator<<(std::ostream &os, const Vector &v);
};

using Color = Vector;

} // namespace rtx

#endif