#include "Satelite.h"

Satelite::Satelite (const string id, const float posicion,
        const int colorRojo, const int colorVerde, const int colorAzul,
        const double radio, const int divisiones, const float translacion, const float rotacion,
        const Planeta &planeta) 
        : Astro (id, posicion, colorRojo, colorVerde, colorAzul, radio, 
                 divisiones, translacion, rotacion) {
  _planeta = planeta;
}

Satelite::Satelite (const Satelite &s) { // Constructor copia
  copiar(s);
}

// Gets y Sets
Planeta Satelite::getPlaneta () const {
  return _planeta;
}

void Satelite::setPlaneta(const Planeta planeta) {
  _planeta = planeta;
}


Satelite& Satelite::operator= (const Satelite &s) {
  copiar(s);
  return *this;
}

void Satelite::copiar(const Satelite &s) {
  _id = s.getId();
  _posicion = s.getPosicion();
  _colorRojo = s.getColorRojo();
  _colorVerde = s.getColorVerde();
  _colorAzul = s.getColorAzul();
  _radio = s.getRadio();
  _divisiones = s.getDivisiones();
  _rotacion = s.getRotacion();
  _translacion = s.getTranslacion();
  _planeta = planeta;
}
