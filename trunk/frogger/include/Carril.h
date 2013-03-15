#ifndef CARRIL_H
#define CARRIL_H

#include <iostream>
#include <Ogre.h>
#include <vector>
#include "ElementoCarril.h"

using namespace std;
using namespace Ogre;

enum Direccion  { DIR_IZQ, DIR_DER };

class Carril {
 public:
  Carril (const string &nombre, const double velocidad, 
          const double separacion, const Direccion direccion,
          const double posicionZ, SceneManager*	pSceneMgr, 
          const Vector3 posParteEscenario);
  Carril (const Carril &c);

  string getNombre () const;
  double getPosicionZ () const;
  double getVelocidad () const;
  double getSeparacion () const;
  Direccion getDireccion () const;
  void setDireccion(const Direccion direccion);
  std::vector<ElementoCarril*>& getElementos();
  void addElemento (ElementoCarril* elemento);
  bool addElemento (string nombre);
  void addModelo (string modelo);
  
  // Sobre carga de Operadores
  Carril& operator= (const Carril &c);

  // Destructor
  ~Carril ();

  // Otras funciones
  string toString (); // Imprime todas las noticias del canal
  void mover(const double deltaT, const double tiempo);
  // Limpia los elementos del carril
  void limpiarElementos();
 private:
  void copiar(const Carril &c);

  string _nombre;
  // Velocidad del los elementos del carril
  // Separacion entre los elementos del carril
  double _velocidad, _separacion;
  Direccion _direccion;
  std::vector<ElementoCarril*> _elementos;
  std::vector<string> _modelos;
  double _posicionZ;
  SceneManager*	_pSceneMgr;
  Vector3 _posParteEscenario;
};

#endif
