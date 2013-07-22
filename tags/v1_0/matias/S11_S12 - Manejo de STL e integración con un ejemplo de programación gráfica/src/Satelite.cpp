#include "Satelite.h"
#include <sstream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

Satelite::Satelite (const string id, const float posicion,
        const int colorRojo, const int colorVerde, const int colorAzul,
        const double radio, const int divisiones, const float translacion, const float rotacion,
        const string idPlaneta) 
        : Astro (id, posicion, colorRojo, colorVerde, colorAzul, radio, 
                 divisiones, translacion, rotacion) {
  _idPlaneta = idPlaneta;
}

Satelite::Satelite (const Satelite &s) { // Constructor copia
  copiar(s);
}

// Gets y Sets
inline string Satelite::getIdPlaneta () const {
  return _idPlaneta;
}

inline void Satelite::setIdPlaneta(const string idPlaneta) {
  _idPlaneta = idPlaneta;
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
  _idPlaneta = s.getIdPlaneta();
}

string Satelite::toString () const {
  stringstream out;
  
  out << "\t\tId: " << _id << endl;
  out << "\t\tPosicion: " << _posicion << endl;
  out << "\t\tColor Rojo: " << _colorRojo << endl;
  out << "\t\tColor Verde: " << _colorVerde << endl;
  out << "\t\tColor Azul: " << _colorAzul << endl;
  out << "\t\tRadio: " << _radio << endl;
  out << "\t\tDivisiones: " << _divisiones << endl;
  out << "\t\tRotacion: " << _rotacion << endl;
  out << "\t\tTranslacion: " << _translacion << endl;
  out << "\t\tPertenece a: " << _idPlaneta << endl;
  out << "\t\t----------------------------------------" << endl;  

  return out.str();
}

void Satelite::pintar(long tiempo) const {
  glRotatef (tiempo * getTranslacion(), 0.0, 0.0, 1.0);
  glTranslatef(getPosicion(), 0.0, 0.0);
  glPushMatrix();
  glRotatef (tiempo * getRotacion(), 0.0, 0.0, 1.0); 
  glColor3ub (getColorRojo(), getColorVerde(), getColorAzul());
  glutWireSphere (getRadio(), getDivisiones(), getDivisiones());
  glPopMatrix();
}
