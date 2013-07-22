#ifndef CANAL_SUSCRITO_H
#define CANAL_SUSCRITO_H

#include <iostream>

using namespace std;

class CanalSuscrito {
 public:
  CanalSuscrito () { };
  CanalSuscrito (const string &nombre);
  CanalSuscrito (const CanalSuscrito &c);

  string getNombre () const;
  
  // Sobre carga de Operadores
  CanalSuscrito& operator= (const CanalSuscrito &c);
  bool operator == (const CanalSuscrito &c) const;

  // Otras funciones
  string toString (); // Imprime todas las noticias del canal
 private:
  string _nombre;
};

#endif
