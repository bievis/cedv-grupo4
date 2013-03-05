#include <sstream>
#include "AdvancedOgreFramework.hpp"
#include "Carril.h"

Carril::Carril (const string &nombre, const double velocidad, 
                const double separacion, const Direccion direccion,
                const double posicionZ, SceneManager*	pSceneMgr, 
                const Vector3 posParteEscenario) {
  _nombre = nombre;
  _velocidad = velocidad;
  _separacion = separacion;
  _direccion = direccion;
  _posicionZ = posicionZ;
  _pSceneMgr = pSceneMgr;
  _posParteEscenario = posParteEscenario;
}

// Constructor de copia
Carril::Carril (const Carril &c) {
  copiar(c);
}

string Carril::getNombre () const {
  return _nombre;
}

Direccion Carril::getDireccion() const {
  return _direccion;
}

double Carril::getPosicionZ () const {
  return _posicionZ;
}

double Carril::getVelocidad () const {
  return _velocidad;
}

double Carril::getSeparacion () const {
  return _separacion;
}

void Carril::setDireccion(const Direccion direccion) {
  _direccion = direccion;
}

Carril& Carril::operator= (const Carril &c) {
  copiar(c);
  return *this;
}

std::vector<ElementoCarril*>& Carril::getElementos() {
  return _elementos;
}

void Carril::copiar(const Carril &c) {
  _nombre = c.getNombre();
  _velocidad = c.getVelocidad();
  _separacion = c.getSeparacion();
  _direccion = c.getDireccion();
  _posicionZ = c.getPosicionZ();
  _pSceneMgr = c._pSceneMgr;
  _posParteEscenario = c._posParteEscenario;
}

// Destructor
Carril::~Carril () {
  _pSceneMgr = NULL;
  _elementos.clear();
  _modelos.clear();
}

// Otras funciones
string Carril::toString () {
  stringstream out;
  
  out << "Carril: " << getNombre()  << endl;
  
  return out.str();
}

void Carril::addElemento (ElementoCarril* elemento) {
  _elementos.push_back (elemento);
}

void Carril::addModelo (string modelo) {
  _modelos.push_back (modelo);
}

void Carril::mover(const double deltaT, const double tiempo) {
  static unsigned int nElementos = 0;
  ElementoCarril *elemento = NULL;
  unsigned int count = _elementos.size();
  
  for (int i = count-1; i >= 0; i -= 1)
  {
    elemento = _elementos[i];
    elemento->mover(deltaT, getVelocidad());
    // Miramos si ya ah llegado al final
    if (elemento->getMovimiento() == NONE) {
      _elementos.erase (_elementos.begin()+i);
      _pSceneMgr->destroySceneNode(elemento->getNombre());
      _pSceneMgr->destroyEntity(elemento->getNombre());
    }
  }
  // Insertamos el ementos si hay que insertarlos
  stringstream nombre;
  bool insertado = true;
  while (insertado) {
    nombre.str(string()); // Limpiamos
    nElementos++;
    nombre << "Elemento" << nElementos;
    insertado = addElemento(nombre.str());
    if (!insertado) nElementos--;
  }  
}

// Añade un elemento al carril y indica si se ha añadido o no
bool Carril::addElemento (string nombre) {
  SceneNode* nodoElemento = NULL;
  double posX = 0;
  unsigned int size = _elementos.size();
  bool insertado = true;
  Movimiento movimiento = NONE;

  if (size == 0) {
    // Es el primero del carril, por lo que insertamos desde la posicion final hasta la inicial
    if (getDireccion() == DIR_IZQ) {
      posX = LIMITE_IZQ;
    } else {
      posX = LIMITE_DER;
    }
  } else {
    // Cogemos del ultimo su posicion en X
    double posXElem = _elementos[size - 1]->getNodo()->getPosition().x;
    // Insertamos detras del primero, segun la direccion
    if (getDireccion() == DIR_IZQ) {
      posX = posXElem + getSeparacion();
      movimiento = IZQ;
    } else {
      posX = posXElem - getSeparacion();
      movimiento = DER;
    }
    if (posX < LIMITE_IZQ || posX > LIMITE_DER) insertado = false;
  }  
  
  if (insertado && !_modelos.empty()) {
    // Del 0 al (size - 1) de _modelos
    unsigned int aleatorio = Math ::Ceil(Math::RangeRandom (-1, _modelos.size() - 1));
    nodoElemento = GameManager::getSingleton().crearNodo(_pSceneMgr, nombre.c_str(), _modelos[aleatorio].c_str(), 
          posX, _posParteEscenario.y, getPosicionZ());
    addElemento(new ElementoCarril(string(nombre), nodoElemento, movimiento));
  } else {
    insertado = false;
  }

  return insertado;
}
