#include "Colision.h"

Colision::Colision (const TipoColision tipo, ElementoCarril* elementoColision) {
  _tipo = tipo;
  _elementoColision = elementoColision;
}

// Constructor de copia
Colision::Colision (const Colision &c) {
  copiar(c);
}

TipoColision Colision::getTipo () const {
  return _tipo;
}

ElementoCarril* Colision::getElementoColision() const {
  return _elementoColision;
}

Colision& Colision::operator= (const Colision &c) {
  copiar(c);
  return *this;
}

void Colision::copiar(const Colision &c) {
  _tipo = c.getTipo();
  _elementoColision = c.getElementoColision();
}

// Destructor
Colision::~Colision () {
  _elementoColision = NULL;
}
