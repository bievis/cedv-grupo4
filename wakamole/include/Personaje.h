#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <iostream>
#include <Ogre.h>
#include <OIS/OIS.h>

using namespace std;
using namespace Ogre;

#define FINAL 2.5
#define VELOCIDAD 2.8 // Velocidad inicial del personaje
#define VELOCIDAD_MUERTO 14 // Velocidad del personaje al estar muerto

enum EstadoPersonaje  { PARADADO, MOVIMIENTO, MUERTO };

class Personaje {
 public:
  Personaje (const string &nombre, SceneNode* nodo);
  Personaje (const Personaje &p);

  string getNombre () const;
  SceneNode* getNodo() const;
  bool isSubiendo() const;
  EstadoPersonaje getEstado() const;
  void setEstado(const EstadoPersonaje estado);
  
  // Sobre carga de Operadores
  Personaje& operator= (const Personaje &p);

  // Destructor
  ~Personaje ();

  // Otras funciones
  string toString (); // Imprime todas las noticias del canal
  void mover(const Ogre::Real deltaT, int acelerar); // Mueve el personaje

 private:
  void copiar(const Personaje &p);

  string _nombre;
  SceneNode* _nodo;
  bool _subiendo;
  EstadoPersonaje _estado;
  Ogre::Real _posInicial;
  Ogre::Real _posFinal;
};

#endif
