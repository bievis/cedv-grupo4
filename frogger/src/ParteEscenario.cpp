#include <sstream>
#include "AdvancedOgreFramework.hpp"
#include "ParteEscenario.h"

ParteEscenario::ParteEscenario (const string &nombre, SceneNode* nodo) {
  _nombre = nombre;
  _nodo = nodo;
}

// Constructor de copia
ParteEscenario::ParteEscenario (const ParteEscenario &p) {
  copiar(p);
}

string ParteEscenario::getNombre () const {
  return _nombre;
}

SceneNode* ParteEscenario::getNodo() const {
  return _nodo;
}

ParteEscenario& ParteEscenario::operator= (const ParteEscenario &p) {
  copiar(p);
  return *this;
}

void ParteEscenario::copiar(const ParteEscenario &p) {
  _nombre = p.getNombre();
  _nodo = p.getNodo();
}

// Destructor
ParteEscenario::~ParteEscenario () {
  _nodo = NULL;
}

// Otras funciones
string ParteEscenario::toString () {
  stringstream out;
  
  out << "Parte del Escenario: " << getNombre() 
      << " - Posicion: " << getNodo()->getPosition() << endl;
  
  return out.str();
}
