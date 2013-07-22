#include "Colision.h"

Colision::Colision (const TipoColision tipo, ElementoCarril* elementoColision
            , const double velocidad) {
  _tipo = tipo;
  _elementoColision = elementoColision;
  _velocidad = velocidad; 
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

double Colision::getVelocidad() const {
  return _velocidad;
}

Colision& Colision::operator= (const Colision &c) {
  copiar(c);
  return *this;
}

void Colision::copiar(const Colision &c) {
  _tipo = c.getTipo();
  _elementoColision = c.getElementoColision();
  _velocidad = c.getVelocidad();
}

// Destructor
Colision::~Colision () {
  _elementoColision = NULL;
}
