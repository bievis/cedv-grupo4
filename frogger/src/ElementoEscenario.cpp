#include <sstream>
#include "AdvancedOgreFramework.hpp"
#include "ElementoEscenario.h"

ElementoEscenario::ElementoEscenario (const string &nombre, SceneNode* nodo) {
  _nombre = nombre;
  _nodo = nodo;
  _movimiento = NINGUNO;
  _fin = false;
}

// Constructor de copia
ElementoEscenario::ElementoEscenario (const ElementoEscenario &e) {
  copiar(e);
}

string ElementoEscenario::getNombre () const {
  return _nombre;
}

SceneNode* ElementoEscenario::getNodo() const {
  return _nodo;
}

double ElementoEscenario::getPosFinal() const {
  return _posFinal;
}

void ElementoEscenario::setPosFinal(const double posFinal) {
  _posFinal = posFinal;
}

Movimiento ElementoEscenario::getMovimiento() const {
  return _movimiento;
}

void ElementoEscenario::setMovimiento(const Movimiento movimiento) {
  _movimiento = movimiento;
}

bool ElementoEscenario::isFin() const {
  return _fin;
}

ElementoEscenario& ElementoEscenario::operator= (const ElementoEscenario &e) {
  copiar(e);
  return *this;
}

void ElementoEscenario::copiar(const ElementoEscenario &e) {
  _nombre = e.getNombre();
  _nodo = e.getNodo();
  _posFinal = e.getPosFinal();
  _movimiento = e.getMovimiento();
  _fin = e.isFin();
}

// Destructor
ElementoEscenario::~ElementoEscenario () {
  _nodo = NULL;
}

// Otras funciones
string ElementoEscenario::toString () {
  stringstream out;
  
  out << getNombre() 
      << " - Posicion: " << getNodo()->getPosition() << endl;
  
  return out.str();
}

void ElementoEscenario::mover(const double deltaT) {
  if (!_fin) {
    Ogre::Real posicion;
    double incremento;
    if (_movimiento == DERECHA) incremento = 1;
    else if (_movimiento == IZQUIERDA) incremento = -1;
    // Para avanzar el ElementoEscenario
    posicion = _nodo->getPosition().x;
    _nodo->translate(VELOCIDAD * deltaT * incremento, 0, 0);
    // Para parar el ElementoEscenario
    if (_movimiento == IZQUIERDA) {
      if (posicion <= _posFinal) {
        _nodo->setPosition(_nodo->getPosition().x, _nodo->getPosition().y, _posFinal);
        _movimiento = NINGUNO;
        _fin = true;
      }
    } else if (_movimiento == DERECHA) {
      if (posicion >= _posFinal) {
        _nodo->setPosition(_nodo->getPosition().x, _nodo->getPosition().y, _posFinal);
        _movimiento = NINGUNO;
        _fin = true;
      }
    }
  }
}
