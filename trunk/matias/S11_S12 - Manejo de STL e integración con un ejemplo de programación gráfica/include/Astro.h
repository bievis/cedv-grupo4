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
  inline string getId () const;
  inline float getPosicion () const;
  inline int getColorRojo () const;
  inline int getColorVerde () const;
  inline int getColorAzul () const;
  inline double getRadio () const;
  inline int getDivisiones () const;
  inline float getTranslacion () const;
  inline float getRotacion () const;
  inline void setId(const string id);
  inline void setPosicion(const float posicion);
  inline void setColorRojo(const int colorRojo);
  inline void setColorVerde(const int colorVerde);
  inline void setColorAzul(const int colorAzul);
  inline void setRadio(const double radio);
  inline void setDivisiones(const int divisiones);
  inline void setTranslacion(const float translacion);
  inline void setRotacion(const float rotacion);
  
  // Sobre carga de Operadores
  Astro& operator= (const Astro &a);
  bool operator == (const Astro &a) const;

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
