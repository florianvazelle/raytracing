#include <cmath>

#include <rtx/Entity.h>

using namespace rtx;

/**
 * Effectue une translation de vecteur (x,y,z)
 */
void Entity::translate(float x, float y, float z) {
  Matrix translation;
  translation(0, 3) = x;
  translation(1, 3) = y;
  translation(2, 3) = z;

  trans = translation * trans;
  transInv = trans.inverse();
}

/**
 * Effectue une rotation sur l'axe X, de deg radians
 */
void Entity::rotateX(float deg) {
  Matrix rotate;
  rotate(1, 1) = std::cos(deg);
  rotate(1, 2) = -sin(deg);
  rotate(2, 1) = sin(deg);
  rotate(2, 2) = std::cos(deg);

  trans = rotate * trans;
  transInv = trans.inverse();
}

/**
 * Effectue une rotation sur l'axe Y, de deg radians
 */
void Entity::rotateY(float deg) {
  Matrix rotate;
  rotate(0, 0) = std::cos(deg);
  rotate(2, 0) = -sin(deg);
  rotate(0, 2) = sin(deg);
  rotate(2, 2) = std::cos(deg);

  trans = rotate * trans;
  transInv = trans.inverse();
}

/**
 * Effectue une rotation sur l'axe Z, de deg radians
 */
void Entity::rotateZ(float deg) {
  Matrix rotate;
  rotate(0, 0) = std::cos(deg);
  rotate(0, 1) = -sin(deg);
  rotate(1, 0) = sin(deg);
  rotate(1, 1) = std::cos(deg);

  trans = rotate * trans;
  transInv = trans.inverse();
}

/**
 * Effectue un redimensionnement de facteur factor
 */
void Entity::scale(float f) {
  Matrix scale;
  scale(0, 0) = f;
  scale(1, 1) = f;
  scale(2, 2) = f;

  trans = scale * trans;
  transInv = trans.inverse();
}

Vector Entity::localToGlobal(const Vector &v) const { return transInv * v; }

Point Entity::localToGlobal(const Point &p) const { return transInv * p; }

Ray Entity::localToGlobal(const Ray &r) const {
  Ray r1;
  r1.origin = transInv * r.origin;
  r1.vector = transInv * r.vector;
  return r1;
}

Vector Entity::globalToLocal(const Vector &v) const { return trans * v; }

Point Entity::globalToLocal(const Point &p) const { return trans * p; }

Ray Entity::globalToLocal(const Ray &r) const {
  Ray r1;
  r1.origin = trans * r.origin;
  r1.vector = trans * r.vector;
  return r1;
}