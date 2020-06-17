#include <cmath>

#include "Entity.h"

/**
 * Effectue une translation de vecteur (x,y,z)
 */
void Entity::translate(float x, float y, float z) {
  trans(0, 3) += x;
  trans(1, 3) += y;
  trans(2, 3) += z;

  transInv = trans.inverse();
}

/**
 * Effectue une rotation sur l'axe X, de deg radians
 */
void Entity::rotateX(float deg) {
  Matrix rotate;
  rotate(0, 0) = 1.0f;
  rotate(1, 1) = std::cos(deg);
  rotate(1, 2) = -sin(deg);
  rotate(2, 1) = sin(deg);
  rotate(2, 2) = std::cos(deg);
  rotate(3, 3) = 1.0f;

  trans = rotate * trans;
  transInv = trans.inverse();
}

/**
 * Effectue une rotation sur l'axe Y, de deg radians
 */
void Entity::rotateY(float deg) {
  Matrix rotate;
  rotate(0, 0) = std::cos(deg);
  rotate(1, 1) = 1.0f;
  rotate(2, 0) = -sin(deg);
  rotate(0, 2) = sin(deg);
  rotate(2, 2) = std::cos(deg);
  rotate(3, 3) = 1.0f;

  trans = rotate * trans;
  transInv = trans.inverse();
}

/**
 * Effectue une rotation sur l'axe Z, de deg radians
 */
void Entity::rotateZ(float deg) {
  Matrix rotate;
  rotate(0, 0) = std::cos(deg);
  rotate(1, 1) = 1.0f;
  rotate(0, 1) = -sin(deg);
  rotate(1, 0) = sin(deg);
  rotate(1, 1) = std::cos(deg);
  rotate(3, 3) = 1.0f;

  trans = rotate * trans;
  transInv = trans.inverse();
}

/**
 * Effectue un redimensionnement de facteur factor
 */
void Entity::scale(float f) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      trans(i, j) *= f;
    }
  }

  transInv = trans.inverse();
}

Vector Entity::localToGlobal(const Vector &v) const {
  Vector v1 = transInv * v;
  return v1;
}

Ray Entity::localToGlobal(const Ray &r) const {
  Ray r1;
  r1.origin = transInv * r.origin;
  r1.vector = transInv * r.vector;
  return r1;
}

Vector Entity::globalToLocal(const Vector &v) const {
  Vector v1 = trans * v;
  return v1;
}

Ray Entity::globalToLocal(const Ray &r) const {
  Ray r1;
  r1.origin = trans * r.origin;
  r1.vector = trans * r.vector;
  return r1;
}