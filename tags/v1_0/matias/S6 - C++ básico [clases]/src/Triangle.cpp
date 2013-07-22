#include "Triangle.h"
#include <sstream>
#include <iostream>
using namespace std;

Triangle::Triangle (const Vec3 &v1, const Vec3 &v2, const Vec3 &v3) {
  _v1 = new Vec3(v1.getX(), v1.getY(), v1.getZ());
  _v2 = new Vec3(v2.getX(), v2.getY(), v2.getZ());
  _v3 = new Vec3(v3.getX(), v3.getY(), v3.getZ());
}

// Constructor de copia
Triangle::Triangle (const Triangle &t) {
  _v1 = new Vec3(t.getV1().getX(), t.getV1().getY(), t.getV1().getZ());
  _v2 = new Vec3(t.getV2().getX(), t.getV2().getY(), t.getV2().getZ());
  _v3 = new Vec3(t.getV3().getX(), t.getV3().getY(), t.getV3().getZ());
}

Triangle::~Triangle () {
  delete _v1;
  delete _v2;
  delete _v3;
}

Vec3 Triangle::getV1 () const {
  return *_v1;
}

Vec3 Triangle::getV2 () const {
  return *_v2;
}

Vec3 Triangle::getV3 () const {
  return *_v3;
}

Triangle& Triangle::operator= (const Triangle &t) {
  _v1 = new Vec3(t.getV1().getX(), t.getV1().getY(), t.getV1().getZ());
  _v2 = new Vec3(t.getV2().getX(), t.getV2().getY(), t.getV2().getZ());
  _v3 = new Vec3(t.getV3().getX(), t.getV3().getY(), t.getV3().getZ());
  return *this;
}

// Otras funciones
string Triangle::toString () {
  stringstream out;
  out << "A: " << getV1().toString() << ", B: " << getV2().toString() << ", C: " << getV3().toString();
  return out.str();
}

