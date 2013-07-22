#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <iostream>
#include <Ogre.h>
#include "ElementoCarril.h"
#include "AnimationBlender.h"

using namespace std;
using namespace Ogre;

#define VELOCIDAD_AHOGADO 0.98
#define AVANCE 1.5
#define VELOCIDAD 2

enum EstadoPersonaje  { PARADO, MOVIMIENTO, MUERTO, MUERTO_AHOGADO };
enum MovimientoPersonaje  { DELANTE, ATRAS, IZQUIERDA, DERECHA, NINGUNO };

class Personaje {
 public:
  Personaje (const string &nombre, SceneNode* nodo, SceneNode* nodoMuerto, SceneNode* nodoEstrellas);
  Personaje (const Personaje &p);

  string getNombre () const;
  SceneNode* getNodo() const;
  Entity* getEntidad() const;
  EstadoPersonaje getEstado() const;
  void setEstado(const EstadoPersonaje estado);
  MovimientoPersonaje getMovimiento() const;
  void setMovimiento(const MovimientoPersonaje movimiento);
  double getPosFinal() const;
  Vector3 getPosInicial() const;
  
  // Sobre carga de Operadores
  Personaje& operator= (const Personaje &p);

  // Destructor
  ~Personaje ();

  // Otras funciones
  string toString (); // Imprime todas las noticias del canal
  void mover(const double deltaT); // Mueve el personaje
  // Mueve el personaje cuando este encima de un elemento
  void moverConElemento(const double deltaT, ElementoCarril* elemento, const double velocidad);
  // Pone el personaje en la poscion inicial
  void volverAInicio();
 private:
  void copiar(const Personaje &p);

  string _nombre;
  SceneNode* _nodo;
  SceneNode* _nodoMuerto;
  SceneNode* _nodoEstrellas;
  EstadoPersonaje _estado;
  double _posFinal;
  MovimientoPersonaje _movimiento;
  Vector3 _posInicial;
  AnimationBlender *_animacion;
};

#endif
