#ifndef ELEMENTO_ESCENARIO_H
#define ELEMENTO_ESCENARIO_H

#include <iostream>
#include <Ogre.h>
#include <OIS/OIS.h>

using namespace std;
using namespace Ogre;

#define VELOCIDAD 10

enum Movimiento  { IZQUIERDA, DERECHA, NINGUNO };

class ElementoEscenario {
 public:
  ElementoEscenario (const string &nombre, SceneNode* nodo);
  ElementoEscenario (const ElementoEscenario &e);

  string getNombre () const;
  SceneNode* getNodo() const;
  double getPosFinal() const;
  void setPosFinal(const double posFinal);
  Movimiento getMovimiento() const;
  void setMovimiento(const Movimiento movimiento);
  bool isFin() const;
  
  // Sobre carga de Operadores
  ElementoEscenario& operator= (const ElementoEscenario &e);

  // Destructor
  ~ElementoEscenario ();

  // Otras funciones
  string toString (); // Imprime todas las noticias del canal
  void mover(const double deltaT); // Mueve el personaje

 private:
  void copiar(const ElementoEscenario &e);

  string _nombre;
  SceneNode* _nodo;
  double _posFinal;
  Movimiento _movimiento;
  bool _fin;
};

#endif
