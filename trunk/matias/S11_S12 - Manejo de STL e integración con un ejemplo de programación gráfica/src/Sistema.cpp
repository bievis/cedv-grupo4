#include "Sistema.h"
#include <sstream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

Sistema::Sistema (const string id, const Astro astroCentral) {
  _id = id;
  _astroCentral = astroCentral;
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

inline Astro& Sistema::getAstroCentral() {
  return _astroCentral;
}

inline void Sistema::setAstroCentral(const Astro astroCentral) {
  _astroCentral = astroCentral;
}

std::map<string, Planeta>& Sistema::getPlanetas() {
  return _planetas;
}


Sistema& Sistema::operator= (const Sistema &s) {
  copiar(s);
  return *this;
}

void Sistema::copiar(const Sistema &s) {
  _id = s.getId();
  _astroCentral = s._astroCentral;
  _planetas = s._planetas;
}

string Sistema::toString () {
  stringstream out;
  
  out << "Sistema: " << _id << endl;
  out << "Centro: " << endl << _astroCentral.toString() << endl;
  std::map<string, Planeta>::const_iterator
    mit (_planetas.begin()),
    mend(_planetas.end());
  for(;mit!=mend;++mit) out << mit->second.toString() << endl;
  
  return out.str();
}

void Sistema::pintar(long tiempo) const {
  glClear( GL_COLOR_BUFFER_BIT ); 
  glPushMatrix();
  _astroCentral.pintar(tiempo);
  
  std::map<string, Planeta>::const_iterator
    mit (_planetas.begin()),
    mend(_planetas.end());
  for(;mit!=mend;++mit) {
    glPushMatrix();
    mit->second.pintar(tiempo);
    glPopMatrix();
  }
  
  glutSwapBuffers();      
  glPopMatrix();
}
