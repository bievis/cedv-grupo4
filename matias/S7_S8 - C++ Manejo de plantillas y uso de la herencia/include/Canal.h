#ifndef CANAL_H
#define CANAL_H

#include <iostream>
#include "Lista.h"
#include "Noticia.h"

using namespace std;

const int N_TOP = 5;

class Canal {
 public:
  Canal () : _listaNoticias() { };
  Canal (const string &nombre);
  Canal (const Canal &c);

  string getNombre () const;
  Lista<Noticia>& getListaNoticias() const;
  
  // Sobre carga de Operadores
  Canal& operator= (const Canal &c);
  bool operator == (const Canal &c) const;

  // Operaciones de noticias
  void insertarNoticia(Noticia& noticia);

  // Otras funciones
  string toString (); // Imprime todas las noticias del canal
  string toStringTop5 (); // Imprime todas las 5 ultimas noticias del canal

 private:
  string _nombre;
  Lista<Noticia> _listaNoticias;
};

#endif
