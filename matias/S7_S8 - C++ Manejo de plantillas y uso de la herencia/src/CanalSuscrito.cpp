#include <sstream>
#include "CanalSuscrito.h"

CanalSuscrito::CanalSuscrito (const string &nombre) {
  _nombre = nombre;
}

// Constructor de copia
CanalSuscrito::CanalSuscrito (const CanalSuscrito &c) {
  _nombre = c.getNombre();
}

string CanalSuscrito::getNombre () const {
  return _nombre;
}

CanalSuscrito& CanalSuscrito::operator= (const CanalSuscrito &c) {
  _nombre = c.getNombre();
  return *this;
}

bool CanalSuscrito::operator == (const CanalSuscrito &c) const {
  return c.getNombre() == getNombre();
}

// Otras funciones
string CanalSuscrito::toString () {
  stringstream out;
 
  out << "CanalSuscrito: '" << getNombre() << endl;
  
  return out.str();
}
