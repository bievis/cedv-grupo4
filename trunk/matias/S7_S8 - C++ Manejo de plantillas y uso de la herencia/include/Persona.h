#ifndef PERSONA_H
#define PERSONA_H

#include <iostream>
#include "Lista.h"
#include "Canal.h"
#include "CanalSuscrito.h"

using namespace std;

enum TipoUsuario { USER, ADMIN };

class Persona {
 public:
  Persona () : _listaCanales() { };
  Persona (const string &nombre, const string &clave, const TipoUsuario tipo);
  Persona (const Persona &p);

  string getNombre () const;
  string getClave () const;
  TipoUsuario getTipo () const;
  Lista<CanalSuscrito>& getListaCanales () const;
  
  // Sobre carga de Operadores
  Persona& operator= (const Persona &p);
  bool operator == (const Persona &p) const;

  // Otras funciones
  string toString ();

  // Da de alta al usuario en un canal
  void altaCanal (string nombreCanal);
  
  // Da de baja al usuario en un canal
  void bajaCanal (string nombreCanal);

  // Imprimir ultimas noticias
  void ultimasNoticias (Lista<Canal> &listaCanalEntera);
 private:
  string _nombre, _clave;
  TipoUsuario _tipo;
  Lista<CanalSuscrito> _listaCanales;
};

#endif
