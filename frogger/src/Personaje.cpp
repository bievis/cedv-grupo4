#include <sstream>
#include "AdvancedOgreFramework.hpp"
#include "Personaje.h"
#include "math.h"

Personaje::Personaje (const string &nombre, SceneNode* nodo) {
  _nombre = nombre;
  _nodo = nodo;
  _estado = PARADO;
  _movimiento = NINGUNO;
}

// Constructor de copia
Personaje::Personaje (const Personaje &p) {
  copiar(p);
}

string Personaje::getNombre () const {
  return _nombre;
}

SceneNode* Personaje::getNodo() const {
  return _nodo;
}

EstadoPersonaje Personaje::getEstado() const {
  return _estado;
}

void Personaje::setEstado(const EstadoPersonaje estado) {
  _estado = estado;
}

MovimientoPersonaje Personaje::getMovimiento() const {
  return _movimiento;
}

void Personaje::setMovimiento(const MovimientoPersonaje movimiento) {
  if (_estado == PARADO) _movimiento = movimiento;
}

Personaje& Personaje::operator= (const Personaje &p) {
  copiar(p);
  return *this;
}

void Personaje::copiar(const Personaje &p) {
  _nombre = p.getNombre();
  _nodo = p.getNodo();
  _estado = p.getEstado();
  _movimiento = p.getMovimiento();
}

// Destructor
Personaje::~Personaje () {
  _nodo = NULL;
}

// Otras funciones
string Personaje::toString () {
  stringstream out;
  
  out << getNombre() 
      << " - Posicion: " << getNodo()->getPosition() << endl;
  
  return out.str();
}

void Personaje::mover(const double deltaT) {
  if (_movimiento != NINGUNO) {
    if (_estado == MOVIMIENTO || _estado == PARADO) {
      Ogre::Real posicion;
      if (_movimiento == IZQUIERDA || _movimiento == ATRAS) _incremento = 1;
      else if (_movimiento == DELANTE || _movimiento == DERECHA) _incremento = -1;
      // Si esta parado iniciamos el movimiento
      if (_estado == PARADO) {
        _estado = MOVIMIENTO;
        
        if (_movimiento == IZQUIERDA || _movimiento == DERECHA) _posFinal = _nodo->getPosition().z + _incremento;
        else if (_movimiento == DELANTE || _movimiento == ATRAS) _posFinal = _nodo->getPosition().x + _incremento;      
      }
      // Para avanzar el personaje
      if (_movimiento == IZQUIERDA || _movimiento == DERECHA) {
        posicion = _nodo->getPosition().z;
        _nodo->translate(0, 0, VELOCIDAD * deltaT * _incremento);
      } else if (_movimiento == DELANTE || _movimiento == ATRAS) {
        posicion = _nodo->getPosition().x;
        _nodo->translate(VELOCIDAD * deltaT * _incremento, 0, 0);
      }
      // Para parar el personaje
      if (_movimiento == DELANTE || _movimiento == DERECHA) {
        if (posicion <= _posFinal) {
          _estado = PARADO;
          _movimiento = NINGUNO;
        }
      } else if (_movimiento == IZQUIERDA || _movimiento == ATRAS) {
        if (posicion >= _posFinal) {
          _estado = PARADO;
          _movimiento = NINGUNO;
        }
      }
    }
  }
}
