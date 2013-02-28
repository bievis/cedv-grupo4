#ifndef PARTE_ESCENARIO_H
#define PARTE_ESCENARIO_H

#include <iostream>
#include <Ogre.h>

using namespace std;
using namespace Ogre;

class ParteEscenario {
 public:
  ParteEscenario (const string &nombre, SceneNode* nodo);
  ParteEscenario (const ParteEscenario &p);

  string getNombre () const;
  SceneNode* getNodo() const;
  
  // Sobre carga de Operadores
  ParteEscenario& operator= (const ParteEscenario &p);

  // Destructor
  ~ParteEscenario ();

  // Otras funciones
  string toString (); // Imprime todas las noticias del canal

 private:
  void copiar(const ParteEscenario &p);

  string _nombre;
  SceneNode* _nodo;
};

#endif
