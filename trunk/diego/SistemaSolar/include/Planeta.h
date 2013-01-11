#ifndef _PLANETA_H_
#define _PLANETA_H_

#include <iostream>
#include <list>
#include "Satelite.h"

using namespace std;

//Planeta, para modelar los distintos planetas del Sistema Solar. Un planeta es un tipo especial de astro que tiene una rotación alrededor del Sol (traslación), una rotación interna y una serie de satélites que giran a su alrededor.

class Planeta : public Astro {

  float _traslacion;
  float _rotInterna;

  void Copia ( const Planeta& origen );

 public:
 
  list<Satelite> satelites;

  Planeta() : _traslacion(0.0), _rotInterna(0.0) {};
  virtual ~Planeta() {};

  Planeta ( const Planeta& origen );
  Planeta operator= ( const Planeta& origen );

  inline float getTraslacion ( float hours ) const { return (_traslacion * hours); };
  inline void setTraslacion ( float traslacion ) { _traslacion = traslacion; };

  inline float getRotacion() const { return _rotInterna; };
  inline void setRotacion ( float rotacion ) { _rotInterna = rotacion; };
  
  void print();

};

#endif



