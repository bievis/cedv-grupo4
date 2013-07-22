#ifndef NOTICIA_H
#define NOTICIA_H

#include <iostream>

using namespace std;

class Noticia {
 public:
  Noticia () { };
  Noticia (const string &titulo, const string &cuerpo);
  Noticia (const Noticia &n);

  string getTitulo () const;
  string getCuerpo () const;
  
  // Sobre carga de Operadores
  Noticia& operator= (const Noticia &n);
  bool operator == (const Noticia &n) const;

  // Otras funciones
  string toString ();

 private:
  string _titulo, _cuerpo;
};

#endif
