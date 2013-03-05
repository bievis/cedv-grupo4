#ifndef PARTE_ESCENARIO_H
#define PARTE_ESCENARIO_H

#include <iostream>
#include <Ogre.h>
#include <vector>
#include "Carril.h"

using namespace std;
using namespace Ogre;

enum TipoParte  { CARRETERA, AGUA};

class ParteEscenario {
 public:
  ParteEscenario (const string &nombre, SceneNode* nodo, TipoParte tipo);
  ParteEscenario (const ParteEscenario &p);

  string getNombre () const;
  SceneNode* getNodo() const;
  TipoParte getTipo() const;
  std::vector<Carril*>& getCarriles();
  void addCarril (const char *nombre, const double velocidad, 
          const double separacion, const Direccion direccion, 
        const double posicionZ, SceneManager*	pSceneMgr);
  void addCarril (Carril* carril);
  void addModeloElementoCarril(const char *nombreCarril, const char *mesh);
  
  // Sobre carga de Operadores
  ParteEscenario& operator= (const ParteEscenario &p);

  // Destructor
  ~ParteEscenario ();

  // Otras funciones
  string toString (); // Imprime todas las noticias del canal
  void mover(const double deltaT, const double tiempo);

 private:
  void copiar(const ParteEscenario &p);

  string _nombre;
  SceneNode* _nodo;
  std::vector<Carril*> _carriles;
  TipoParte _tipo;
};

#endif
