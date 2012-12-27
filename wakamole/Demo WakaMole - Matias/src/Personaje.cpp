#include <sstream>
#include "Personaje.h"

Personaje::Personaje (const string &nombre, SceneNode* nodo) {
  _nombre = nombre;
  _nodo = nodo;
  _subiendo = true;
  _estado = PARADADO;
  _posInicial = nodo->getPosition().y;
  _posFinal = _posInicial + FINAL;
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

bool Personaje::isSubiendo() const {
  return _subiendo;
}

EstadoPersonaje Personaje::getEstado() const {
  return _estado;
}

void Personaje::setEstado(const EstadoPersonaje estado) {
  _estado = estado;
  if (_estado == MUERTO) _subiendo = false;
}

Personaje& Personaje::operator= (const Personaje &p) {
  copiar(p);
  return *this;
}

void Personaje::copiar(const Personaje &p) {
  _nombre = p.getNombre();
  _nodo = p.getNodo();
  _subiendo = p.isSubiendo();
  _estado = p.getEstado();
  _posInicial = p._posInicial;
  _posFinal = p._posFinal;
}

// Destructor
Personaje::~Personaje () {
  _nodo = NULL;
}

// Otras funciones
string Personaje::toString () {
  stringstream out;
  
  out << getNombre() 
      << " - Posicion: " << getNodo()->getPosition() 
      << " - Subiendo: " << isSubiendo() << endl;
  
  return out.str();
}

void Personaje::mover(const Ogre::Real deltaT) {
  if (_estado == MOVIMIENTO || _estado == MUERTO) {
    Ogre::Real velocidad = VELOCIDAD;

    if(_nodo->getPosition().y >= _posFinal) _subiendo = false;
    else if(_nodo->getPosition().y < _posInicial) {
      _subiendo = true;
      _estado = PARADADO;
      _nodo->setPosition(_nodo->getPosition().x, _posInicial, _nodo->getPosition().z);
    }
    if (_estado != PARADADO) {
      if(!_subiendo) velocidad = VELOCIDAD * -1;
      if (_estado == MUERTO) velocidad = velocidad * VELOCIDAD_MUERTO;
      _nodo->translate(0, velocidad * deltaT, 0);
    }
  }
}
