#include "Astro.h"
#include <sstream>

Astro::Astro (const string id, const float posicion, 
        const int colorRojo, const int colorVerde, const int colorAzul,
        const double radio, const int divisiones) {
  _id = id;
  _posicion = posicion;
  _colorRojo = colorRojo;
  _colorVerde = colorVerde;
  _colorAzul = colorAzul;
  _radio = radio;
  _divisiones = divisiones;
  _translacion = 0;
  _rotacion = 0;
}

Astro::Astro (const string id, const float posicion,
        const int colorRojo, const int colorVerde, const int colorAzul,
        const double radio, const int divisiones, const float translacion, const float rotacion) {
  _id = id;
  _posicion = posicion;
  _colorRojo = colorRojo;
  _colorVerde = colorVerde;
  _colorAzul = colorAzul;
  _radio = radio;
  _divisiones = divisiones;
  _translacion = translacion;
  _rotacion = rotacion;
}

Astro::Astro (const Astro &a) { // Constructor copia
  copiar(a);
}

// Gets y Sets
inline string Astro::getId () const {
  return _id;
}

inline float Astro::getPosicion () const {
  return _posicion;
}

inline int Astro::getColorRojo () const {
  return _colorRojo;
}

inline int Astro::getColorVerde () const {
  return _colorVerde;
}

inline int Astro::getColorAzul () const {
  return _colorAzul;
}

inline double Astro::getRadio () const {
  return _radio;
}

inline int Astro::getDivisiones () const {
  return _divisiones;
}

inline float Astro::getTranslacion () const {
  return _translacion;
}

inline float Astro::getRotacion () const {
  return _rotacion;
}

inline void Astro::setId(const string id) {
  _id = id;
}

inline void Astro::setPosicion(const float posicion){
  _posicion = posicion;
}

inline void Astro::setColorRojo(const int colorRojo){
  _colorRojo = colorRojo;
}

inline void Astro::setColorVerde(const int colorVerde){
  _colorVerde = colorVerde;
}

inline void Astro::setColorAzul(const int colorAzul){
  _colorAzul = colorAzul;
}

inline void Astro::setRadio(const double radio){
  _radio = radio;
}

inline void Astro::setDivisiones(const int divisiones){
  _divisiones = divisiones;
}

inline void Astro::setRotacion(const float rotacion) {
  _rotacion = rotacion;
}

inline void Astro::setTranslacion(const float translacion) {
  _translacion = translacion;
}

Astro& Astro::operator= (const Astro &a) {
  copiar(a);
  return *this;
}

bool Astro::operator == (const Astro &a) const {
  return a.getId() == getId();
}

void Astro::copiar(const Astro &a) {
  _id = a.getId();
  _posicion = a.getPosicion();
  _colorRojo = a.getColorRojo();
  _colorVerde = a.getColorVerde();
  _colorAzul = a.getColorAzul();
  _radio = a.getRadio();
  _divisiones = a.getDivisiones();
  _rotacion = a.getRotacion();
  _translacion = a.getTranslacion();
}

string Astro::toString () const {
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
  out << "----------------------------------------" << endl;  

  return out.str();
}
