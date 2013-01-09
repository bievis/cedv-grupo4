#include "Sistema.h"
#include <sstream>

Sistema::Sistema (const string id) {
  _id = id;
}

Sistema::Sistema (const Sistema &s) { // Constructor copia
  copiar(s);
}

// Gets y Sets
inline string Sistema::getId() const {
  return _id;
}

inline void Sistema::setId(const string id) {
  _id = id;
}

std::map<string, Astro>& Sistema::getAstros() {
  return _astros;
}


Sistema& Sistema::operator= (const Sistema &s) {
  copiar(s);
  return *this;
}

void Sistema::copiar(const Sistema &s) {
  _id = s.getId();
  _astros = s._astros;
}

string Sistema::toString () {
  stringstream out;
  
  out << "Sistema: " << _id << endl;
  std::map<string, Astro>::const_iterator
    mit (_astros.begin()),
    mend(_astros.end());
  for(;mit!=mend;++mit) out << mit->second.toString() << endl;
  
  return out.str();
}
