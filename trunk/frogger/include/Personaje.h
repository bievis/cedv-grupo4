#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <iostream>
#include <Ogre.h>
#include <OIS/OIS.h>

using namespace std;
using namespace Ogre;

#define ALTURA_SALTO 0.5
#define AVANCE 1
#define VELOCIDAD 2

enum EstadoPersonaje  { PARADO, MOVIMIENTO, MUERTO };
enum MovimientoPersonaje  { DELANTE, ATRAS, IZQUIERDA, DERECHA, NINGUNO };

class Personaje {
 public:
  Personaje (const string &nombre, SceneNode* nodo);
  Personaje (const Personaje &p);

  string getNombre () const;
  SceneNode* getNodo() const;
  EstadoPersonaje getEstado() const;
  void setEstado(const EstadoPersonaje estado);
  MovimientoPersonaje getMovimiento() const;
  void setMovimiento(const MovimientoPersonaje movimiento);
  
  // Sobre carga de Operadores
  Personaje& operator= (const Personaje &p);

  // Destructor
  ~Personaje ();

  // Otras funciones
  string toString (); // Imprime todas las noticias del canal
  void mover(const double deltaT); // Mueve el personaje

 private:
  void copiar(const Personaje &p);

  string _nombre;
  SceneNode* _nodo;
  EstadoPersonaje _estado;
  double _posFinal;
  int _incremento;
  MovimientoPersonaje _movimiento;
};

#endif
