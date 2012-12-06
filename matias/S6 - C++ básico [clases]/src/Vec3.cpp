#include "Vec3.h"
#include <sstream>

Vec3::Vec3 (int x, int y, int z) : _x(x), _y(y), _z(z) {
  
}

// Destructor
Vec3::~Vec3 () {
  
}

// Funciones de acceso.
int Vec3::getX () const {
  return _x;
}

int Vec3::getY () const {
  return _y;
}

int Vec3::getZ () const {
  return _z;
}

// Funcion toString
string Vec3::toString () {
  stringstream out;
  out << "(" << getX() << ", " << getY() << ", " << getZ() << ")";
  return out.str();
}

// Sobre carga de operadores
void Vec3::operator+ (const Vec3 &op) {
  this->setX(this->getX() + op.getX());
  this->setY(this->getY() + op.getY());
  this->setZ(this->getZ() + op.getZ());
}

void Vec3::operator- (const Vec3 &op) {
  this->setX(this->getX() - op.getX());
  this->setY(this->getY() - op.getY());
  this->setZ(this->getZ() - op.getZ());
}

Vec3& Vec3::operator= (const Vec3 &op) {
  this->setX(op.getX());
  this->setY(op.getY());
  this->setZ(op.getZ());
  return *this;
}
