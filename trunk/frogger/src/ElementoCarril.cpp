#include <sstream>
#include "AdvancedOgreFramework.hpp"
#include "ElementoCarril.h"

ElementoCarril::ElementoCarril (const string &nombre, SceneNode* nodo, const Movimiento movimiento) {
  _nombre = nombre;
  _nodo = nodo;
  _movimiento = movimiento;
}

// Constructor de copia
ElementoCarril::ElementoCarril (const ElementoCarril &e) {
  copiar(e);
}

string ElementoCarril::getNombre () const {
  return _nombre;
}

SceneNode* ElementoCarril::getNodo() const {
  return _nodo;
}

Movimiento ElementoCarril::getMovimiento() const {
  return _movimiento;
}

void ElementoCarril::setMovimiento(const Movimiento movimiento) {
  _movimiento = movimiento;
}

ElementoCarril& ElementoCarril::operator= (const ElementoCarril &e) {
  copiar(e);
  return *this;
}

void ElementoCarril::copiar(const ElementoCarril &e) {
  _nombre = e.getNombre();
  _nodo = e.getNodo();
  _movimiento = e.getMovimiento();
}

// Destructor
ElementoCarril::~ElementoCarril () {
  _nodo = NULL;
}

// Otras funciones
string ElementoCarril::toString () {
  stringstream out;
  
  out << getNombre() 
      << " - Posicion: " << getNodo()->getPosition() << endl;
  
  return out.str();
}

void ElementoCarril::mover(const double deltaT, const double velocidad) {
  if (_movimiento != NONE) {
    Ogre::Real posicion;
    double incremento;
    if (_movimiento == DER) incremento = 1;
    else if (_movimiento == IZQ) incremento = -1;
    // Para avanzar el ElementoCarril
    _nodo->translate(velocidad * deltaT * incremento, 0, 0);
    posicion = _nodo->getPosition().x;
    // Para parar el ElementoCarril
    if (_movimiento == IZQ) {
      if (posicion <= LIMITE_IZQ) {
        _nodo->setPosition(LIMITE_DER, _nodo->getPosition().y, _nodo->getPosition().z);
        _movimiento = NONE;
      }
    } else if (_movimiento == DER) {
      if (posicion >= LIMITE_DER) {
        _nodo->setPosition(LIMITE_IZQ, _nodo->getPosition().y, _nodo->getPosition().z);
        _movimiento = NONE;
      }
    }
  }
}
