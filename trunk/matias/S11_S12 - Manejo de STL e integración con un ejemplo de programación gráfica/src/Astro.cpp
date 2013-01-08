#include "Astro.h"

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
string Astro::getId () const {
  return _id;
}

float Astro::getPosicion () const {
  return _posicion;
}

int Astro::getColorRojo () const {
  return _colorRojo;
}

int Astro::getColorVerde () const {
  return _colorVerde;
}

int Astro::getColorAzul () const {
  return _colorAzul;
}

double Astro::getRadio () const {
  return _radio;
}

int Astro::getDivisiones () const {
  return _divisiones;
}

float Astro::getTranslacion () const {
  return _translacion;
}

float Astro::getRotacion () const {
  return _rotacion;
}

void Astro::setId(const string id) {
  _id = id;
}

void Astro::setPosicion(const float posicion){
  _posicion = posicion;
}

void Astro::setColorRojo(const int colorRojo){
  _colorRojo = colorRojo;
}

void Astro::setColorVerde(const int colorVerde){
  _colorVerde = colorVerde;
}

void Astro::setColorAzul(const int colorAzul){
  _colorAzul = colorAzul;
}

void Astro::setRadio(const double radio){
  _radio = radio;
}

void Astro::setDivisiones(const int divisiones){
  _divisiones = divisiones;
}

void Astro::setRotacion(const float rotacion) {
  _rotacion = rotacion;
}

void Astro::setTranslacion(const float translacion) {
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
