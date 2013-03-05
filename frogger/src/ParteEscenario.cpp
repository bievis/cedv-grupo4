#include <sstream>
#include "AdvancedOgreFramework.hpp"
#include "ParteEscenario.h"

ParteEscenario::ParteEscenario (const string &nombre, SceneNode* nodo, TipoParte tipo) {
  _nombre = nombre;
  _nodo = nodo;
  _tipo = tipo;
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

TipoParte ParteEscenario::getTipo() const {
  return _tipo;
}

ParteEscenario& ParteEscenario::operator= (const ParteEscenario &p) {
  copiar(p);
  return *this;
}

std::vector<Carril*>& ParteEscenario::getCarriles() {
  return _carriles;
}

void ParteEscenario::addCarril (const char *nombre, const double velocidad, 
                  const double separacion, const Direccion direccion, 
                  const double posicionZ, SceneManager*	pSceneMgr) {
  _carriles.push_back (new Carril(nombre, velocidad, separacion, 
                      direccion, posicionZ, pSceneMgr, _nodo->getPosition()));
}

// Añade un modelo, para representar los elementos, de un carril determinado
void ParteEscenario::addModeloElementoCarril(const char *nombreCarril, const char *mesh) {
  Carril *carril = NULL;
  std::vector<Carril*>::const_iterator
      mit (_carriles.begin()),
      mend(_carriles.end());
  // Reccoremos todos los carriles y indicamos que se muevan sus elementos
  for(;mit!=mend && carril == NULL;++mit) {
    carril = (*mit);
    if (carril->getNombre() == string(nombreCarril)) {
      carril->addModelo(string(mesh));
    } else {
      carril = NULL;
    }
  }
}

void ParteEscenario::addCarril (Carril *carril) {
  _carriles.push_back (carril);
}

void ParteEscenario::copiar(const ParteEscenario &p) {
  _nombre = p.getNombre();
  _nodo = p.getNodo();
  _tipo = p.getTipo();
}

// Destructor
ParteEscenario::~ParteEscenario () {
  _nodo = NULL;
  _carriles.clear();
}

// Otras funciones
string ParteEscenario::toString () {
  stringstream out;
  
  out << "Parte del Escenario: " << getNombre() << endl;
  
  return out.str();
}

// Vueve los elemtnos de los carriles de la parte del escenario
void ParteEscenario::mover(const double deltaT, const double tiempo) {
  Carril *carril = NULL;
  std::vector<Carril*>::const_iterator
      mit (_carriles.begin()),
      mend(_carriles.end());
  // Reccoremos todos los carriles y indicamos que se muevan sus elementos
  for(;mit!=mend;++mit) {
    carril = (*mit);
    carril->mover(deltaT, tiempo);
  }
}
