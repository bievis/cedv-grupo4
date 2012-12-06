#include <sstream>
#include "Canal.h"

Canal::Canal (const string &nombre) : _listaNoticias() {
  _nombre = nombre;
}

// Constructor de copia
Canal::Canal (const Canal &c) {
  _nombre = c.getNombre();
  _listaNoticias = c.getListaNoticias();
}

string Canal::getNombre () const {
  return _nombre;
}

Lista<Noticia>& Canal::getListaNoticias () const {
  return const_cast<Lista<Noticia>&>(_listaNoticias);
}

Canal& Canal::operator= (const Canal &c) {
  _nombre = c.getNombre();
  _listaNoticias = c.getListaNoticias();
  return *this;
}

bool Canal::operator == (const Canal &c) const {
  return c.getNombre() == getNombre();
}

// Operaciones de noticias
void Canal::insertarNoticia(Noticia& noticia) {
  _listaNoticias.insertarFinal (noticia);
}

// Otras funciones
string Canal::toString () {
  stringstream out;
  int size = _listaNoticias.size() - 1;
  Noticia *pNoticia = NULL;
  
  out << "\t\tCanal: '" << getNombre() << endl;
  for (int i = size; 0 <= i; i--) {
    pNoticia = _listaNoticias.getN(i);
    if (pNoticia != NULL) {
      out << "\t\t\t" << pNoticia->toString() << endl;
    }
  }
  
  return out.str();
}

string Canal::toStringTop5 () {
  stringstream out;
  int size = _listaNoticias.size() - 1;
  int count = 0;
  Noticia *pNoticia = NULL;
  
  out << "\t\tCanal: '" << getNombre() << endl;
  for (int i = size; 0 <= i && count < N_TOP; i--) {
    pNoticia = _listaNoticias.getN(i);
    if (pNoticia != NULL) {
      out << pNoticia->toString() << endl;
    }
    count++;
  }
  
  return out.str();
}

