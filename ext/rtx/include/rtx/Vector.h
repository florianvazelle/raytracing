#ifndef H_VECTOR
#define H_VECTOR

namespace rtx {

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

  Vector operator*(const float t) const;
  Vector operator+(const Vector &rhs) const;
  Vector operator+(const float &rhs) const;
  Vector &operator+=(const Vector &rhs);
  Vector operator-(const Vector &rhs) const;
  Vector operator-(const float &rhs) const;
  Vector operator-() const;
  Vector operator*(const Vector &rhs) const;
  Vector &operator*=(const Vector &rhs);
  Vector &operator*=(const float &rhs);
  Vector operator/(const float &rhs) const;

  float operator[](const int &rhs) const;
  float &operator[](const int &rhs);

  Vector &normalized();
  float dot(const Vector &rhs) const;
};

using Point = Vector;
using Color = Vector;

} // namespace rtx

#endif