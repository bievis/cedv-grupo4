#ifndef ELEMENTO_CARRIL_H
#define ELEMENTO_CARRIL_H

#include <iostream>
#include <Ogre.h>

using namespace std;
using namespace Ogre;

enum Movimiento  { IZQ, DER, NONE };

class ElementoCarril {
 public:
  ElementoCarril (const string &nombre, SceneNode* nodo, const Movimiento movimiento);
  ElementoCarril (const ElementoCarril &e);

  string getNombre () const;
  SceneNode* getNodo() const;
  Movimiento getMovimiento() const;
  void setMovimiento(const Movimiento movimiento);
  
  // Sobre carga de Operadores
  ElementoCarril& operator= (const ElementoCarril &e);

  // Destructor
  ~ElementoCarril ();

  // Otras funciones
  string toString (); // Imprime todas las noticias del canal
  void mover(const double deltaT, const double velocidad); // Mueve el personaje

 private:
  void copiar(const ElementoCarril &e);

  string _nombre;
  SceneNode* _nodo;
  Movimiento _movimiento;
};

#endif
