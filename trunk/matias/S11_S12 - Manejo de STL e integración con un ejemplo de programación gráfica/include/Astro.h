#ifndef ASTRO_H
#define ASTRO_H

#include <iostream>

using namespace std;

class Astro {
 public:
  Astro () { };
  Astro (const string id, const float posicion, 
        const int colorRojo, const int colorVerde, const int colorAzul,
        const double radio, const int divisiones);
  Astro (const string id, const float posicion, 
        const int colorRojo, const int colorVerde, const int colorAzul,
        const double radio, const int divisiones, const float translacion, const float rotacion);
  Astro (const Astro &p); // Constructor copia

  // Gets y Sets
  string getId () const;
  float getPosicion () const;
  int getColorRojo () const;
  int getColorVerde () const;
  int getColorAzul () const;
  double getRadio () const;
  int getDivisiones () const;
  float getTranslacion () const;
  float getRotacion () const;
  void setId(const string id);
  void setPosicion(const float posicion);
  void setColorRojo(const int colorRojo);
  void setColorVerde(const int colorVerde);
  void setColorAzul(const int colorAzul);
  void setRadio(const double radio);
  void setDivisiones(const int divisiones);
  void setTranslacion(const float translacion);
  void setRotacion(const float rotacion);
  
  // Sobre carga de Operadores
  Astro& operator= (const Astro &a);
  bool operator == (const Astro &a) const;

  // Otras funciones
  virtual string toString() const;
  virtual void pintar(long tiempo) const;
 protected:
  string _id;
  float _posicion; // Distancia respecto al planeta del que orbita
  int _colorRojo;
  int _colorVerde;
  int _colorAzul;
  double _radio;
  int _divisiones;
  float _translacion; // Translación sobre el astro al que pertenece
  float _rotacion; // Rotacion sobre si mismo

 private:
  void copiar(const Astro &a);
};

#endif
