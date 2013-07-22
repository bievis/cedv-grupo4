#include "Persona.h"
#include <sstream>

Persona::Persona (const string &nombre, const string &clave, const TipoUsuario tipo) : _listaCanales() {
  _nombre = nombre;
  _clave = clave;
  _tipo = tipo;
}

// Constructor de copia
Persona::Persona (const Persona &p) {
  _nombre = p.getNombre();
  _clave = p.getClave();
  _tipo = p.getTipo();
  _listaCanales = p.getListaCanales();
}

string Persona::getNombre () const {
  return _nombre;
}

string Persona::getClave () const {
  return _clave;
}

Lista<CanalSuscrito>& Persona::getListaCanales () const {
  return const_cast<Lista<CanalSuscrito>&>(_listaCanales);
}

TipoUsuario Persona::getTipo () const {
  return _tipo;
}

Persona& Persona::operator= (const Persona &p) {
  this->_nombre = p.getNombre();
  this->_clave = p.getClave();
  this->_tipo = p.getTipo();
  this->_listaCanales = p.getListaCanales();
  return *this;
}

bool Persona::operator == (const Persona &p) const {
  return p.getNombre() == getNombre();
}

// Otras funciones
string Persona::toString () {
  stringstream out;
  int sizeCanales = _listaCanales.size();
  out << "\nNombre: '" << getNombre() << "', Clave: '" << getClave() << "' y Tipo: " << getTipo();
  if (sizeCanales > 0) {
    out << "\nLista de canales subcritos: ";
    for (int i = 0; i < sizeCanales; i++) {
      out << "\n\t" << _listaCanales.getN(i)->getNombre() << endl;
    }
  }
  return out.str();
}

// Da de alta al usuario en un canal
void Persona::altaCanal (string nombreCanal) {
  _listaCanales.insertarFinal (nombreCanal);
}
  
// Da de baja al usuario en un canal
void Persona::bajaCanal (string nombreCanal) {
  bool eliminado = false;
  int sizeCanales = _listaCanales.size();
  
  for (int i = 0; i < sizeCanales && !eliminado; i++) {
    if (_listaCanales.getN(i)->getNombre() == nombreCanal) {
      _listaCanales.eliminarN (i);
      eliminado = true;
    }
  }
}

// Imprimir ultimas noticias
void Persona::ultimasNoticias (Lista<Canal> &listaCanalEntera) {
  int sizeCanales = _listaCanales.size();
  int sizeCanalesEnteros = listaCanalEntera.size();
  if (sizeCanales > 0) {
    cout << "\n------ ULTIMAS NOTICIAS ------: ";
    for (int i = 0; i < sizeCanales; i++) {
      for (int j = 0; j < sizeCanalesEnteros; j++) {
        if (listaCanalEntera.getN(j)->getNombre() == _listaCanales.getN(i)->getNombre())
          cout << "\n\t" << listaCanalEntera.getN(j)->toStringTop5() << endl;
      }
    }
  }
}

