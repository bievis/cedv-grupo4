#include <sstream>
#include "AdvancedOgreFramework.hpp"
#include "Personaje.h"

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

Entity* Personaje::getEntidad() const {
  return static_cast <Entity *> (_nodo->getAttachedObject(_nombre));
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

double Personaje::getPosFinal() const {
  return _posFinal;
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
  _posFinal = p.getPosFinal();
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
  static int anguloInicial = 0;
  if (_movimiento != NINGUNO) {
    if (_estado == MOVIMIENTO || _estado == PARADO) {
      Ogre::Real posicion;
      double incremento;
      if (_movimiento == DERECHA || _movimiento == ATRAS) incremento = AVANCE * 1;
      else if (_movimiento == DELANTE || _movimiento == IZQUIERDA) incremento = AVANCE * -1;
      // Si esta parado iniciamos el movimiento
      if (_estado == PARADO) {
        _estado = MOVIMIENTO;
        
        if (_movimiento == IZQUIERDA || _movimiento == DERECHA) {
          _posFinal = _nodo->getPosition().x + incremento;
          if (_movimiento == IZQUIERDA) {
            _nodo->yaw(Ogre::Degree(90));
            anguloInicial = -90;
          } else {
            _nodo->yaw(Ogre::Degree(-90));
            anguloInicial = 90;
          }
        }
        else if (_movimiento == DELANTE || _movimiento == ATRAS) {
          _posFinal = _nodo->getPosition().z + incremento;
          if (_movimiento == ATRAS) {
            _nodo->yaw(Ogre::Degree(-180));
            anguloInicial = 180;
          } else {
            _nodo->yaw(Ogre::Degree(0));
            anguloInicial = 0;
          }      
        }
      }
      // Para avanzar el personaje
      if (_movimiento == IZQUIERDA || _movimiento == DERECHA) {
        posicion = _nodo->getPosition().x;
        _nodo->translate(VELOCIDAD * deltaT * incremento, 0, 0);
      } else if (_movimiento == DELANTE || _movimiento == ATRAS) {
        posicion = _nodo->getPosition().z;
        _nodo->translate(0, 0, VELOCIDAD * deltaT * incremento);
      }
      // Para parar el personaje
      if (_movimiento == DELANTE || _movimiento == IZQUIERDA) {
        if (posicion <= _posFinal) {
          _estado = PARADO;
        }
      } else if (_movimiento == DERECHA || _movimiento == ATRAS) {
        if (posicion >= _posFinal) {
          _estado = PARADO;
        }
      }
      // Si lo acabamos de parar lo ponemos en la posicion final exasta
      if (_estado == PARADO) {
        _nodo->yaw(Ogre::Degree(anguloInicial));
        if (_movimiento == IZQUIERDA || _movimiento == DERECHA) _nodo->setPosition(_posFinal, _nodo->getPosition().y, _nodo->getPosition().z);
        else if (_movimiento == DELANTE || _movimiento == ATRAS) _nodo->setPosition(_nodo->getPosition().x, _nodo->getPosition().y, _posFinal);
        _movimiento = NINGUNO;
      }
    }
  }
}

void Personaje::moverConElemento(const double deltaT, ElementoCarril* elemento, const double velocidad) {
   if (_movimiento == NINGUNO) { // Si esta parado
     double incremento;
     // Segun la direcci�n del elemento sobre el que esta se mueve una direcci�n
     // o otra.
     if (elemento->getMovimiento() == DER) incremento = 1;
     else if (elemento->getMovimiento() == IZQ) incremento = -1;
     // Para avanzar el ElementoCarril
     _nodo->translate(velocidad * deltaT * incremento, 0, 0); 
   }
}
