#include "Noticia.h"
#include <sstream>

Noticia::Noticia (const string &titulo, const string &cuerpo) {
  _titulo = titulo;
  _cuerpo = cuerpo;
}

// Constructor de copia
Noticia::Noticia (const Noticia &n) {
  _titulo = n.getTitulo();
  _cuerpo = n.getCuerpo();
}

string Noticia::getTitulo () const {
  return _titulo;
}

string Noticia::getCuerpo () const {
  return _cuerpo;
}

Noticia& Noticia::operator= (const Noticia &n) {
  _titulo = n.getTitulo();
  _cuerpo = n.getCuerpo();
  return *this;
}

bool Noticia::operator == (const Noticia &n) const {
  return n.getTitulo() == getTitulo();
}

// Otras funciones
string Noticia::toString () {
  stringstream out;
  out << "\t\t\t\t- Titulo: '" << getTitulo() << "'\n\t\t\t\t  Cuerpo: '" << getCuerpo() << "'";
  return out.str();
}

