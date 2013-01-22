#include <sstream>
#include "Personaje.h"

Personaje::Personaje (const string &nombre, SceneNode* nodo) {
  _nombre = nombre;
  _nodo = nodo;
  _subiendo = true;
  _estado = PARADADO;
  _posInicial = nodo->getPosition().y;
  _posFinal = _posInicial + FINAL;
  _material = 1;
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
  stringstream nombre;
  Entity *ent = NULL;
  if (_estado == MUERTO) {
    // Mostramos la muerta
    nombre << _nombre << "Muerta";
    ent = static_cast <Entity *> (_nodo->getAttachedObject(nombre.str()));
    nombre.str(string()); // Limpiamos
    nombre << "MateriaCucarachaMuerta" << _material;
    ent->setMaterialName(nombre.str());
    ent->setVisible(true);
    // Ocultamos la viva
    ent = static_cast <Entity *> (_nodo->getAttachedObject(_nombre));
    ent->setVisible(false);
  } else {
    if (_estado == MOVIMIENTO) {
      // Mostramos la viva
      ent = static_cast <Entity *> (_nodo->getAttachedObject(_nombre));
      nombre << "MateriaCucaracha";
      unsigned int aleatorio = Math ::Ceil(Math::RangeRandom (0, 3)); // Del 1 al 3
      nombre << aleatorio;
      _material = aleatorio;
      ent->setMaterialName(nombre.str());
      ent->setVisible(true);
      // Ocultamos la muerta
      nombre.str(string()); // Limpiamos
      nombre << _nombre << "Muerta";
      ent = static_cast <Entity *> (_nodo->getAttachedObject(nombre.str()));
      ent->setVisible(false);
    }
  }
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
  _material = p._material;
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

void Personaje::mover(const Ogre::Real deltaT, int acelerar) {
  if (_estado == MOVIMIENTO || _estado == MUERTO) {
    Ogre::Real velocidad = VELOCIDAD + acelerar;

    if(_nodo->getPosition().y >= _posFinal) _subiendo = false; // A llegado el personaje arriba
    else if(_nodo->getPosition().y < _posInicial) { // A llegado el personaje abajo
      _subiendo = true;
      _estado = PARADADO;
      _nodo->setPosition(_nodo->getPosition().x, _posInicial, _nodo->getPosition().z);
    }
    if (_estado != PARADADO) {
      if(!_subiendo || _estado == MUERTO) velocidad = velocidad * -1;
      _nodo->translate(0, velocidad * deltaT, 0);
    }
  }
}
