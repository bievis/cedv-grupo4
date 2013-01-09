#include "Planeta.h"
#include <sstream>

Planeta::Planeta (const string id, const float posicion, 
        const int colorRojo, const int colorVerde, const int colorAzul,
        const double radio, const int divisiones, const float translacion, const float rotacion) 
        : Astro (id, posicion, colorRojo, colorVerde, colorAzul, radio, 
                divisiones, translacion, rotacion) {
  
}

Planeta::Planeta (const Planeta &p) { // Constructor copia
  copiar(p);
}

// Gets y Sets
std::vector<Satelite>& Planeta::getSatelites() {
  return _satelites;
}


Planeta& Planeta::operator= (const Planeta &p) {
  copiar(p);
  return *this;
}

void Planeta::copiar(const Planeta &p) {
  _id = p.getId();
  _posicion = p.getPosicion();
  _colorRojo = p.getColorRojo();
  _colorVerde = p.getColorVerde();
  _colorAzul = p.getColorAzul();
  _radio = p.getRadio();
  _divisiones = p.getDivisiones();
  _rotacion = p.getRotacion();
  _translacion = p.getTranslacion();
  _satelites = p._satelites;
}

string Planeta::toString () const {
  stringstream out;
  
  out << "Id: " << _id << endl;
  out << "Posicion: " << _posicion << endl;
  out << "Color Rojo: " << _colorRojo << endl;
  out << "Color Verde: " << _colorVerde << endl;
  out << "Color Azul: " << _colorAzul << endl;
  out << "Radio: " << _radio << endl;
  out << "Divisiones: " << _divisiones << endl;
  out << "Rotacion: " << _rotacion << endl;
  out << "Translacion: " << _translacion << endl;
  out << "Satelites: " << endl;
  std::vector<Satelite>::const_iterator
    mit (_satelites.begin()),
    mend(_satelites.end());
  for(;mit!=mend;++mit) out << mit->toString() << endl;
  out << "**************************************" << endl;  

  return out.str();
}
