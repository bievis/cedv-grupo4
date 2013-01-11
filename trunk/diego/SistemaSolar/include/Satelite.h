#ifndef _SATELITE_H_
#define _SATELITE_H_

#include <iostream>
#include "Astro.h"

using namespace std;

//Un satélite está vinculado a un único planeta y tiene asociados movimientos de traslación y rotación.
class Satelite : public Astro {

  float _traslacion;
  float _rotacion;

  void Copia ( const Satelite& origen );

 public:
 
  Satelite() : _traslacion(0.0), _rotacion(0.0) {};
  virtual ~Satelite() {};

  Satelite ( const Satelite& origen );
  Satelite operator= ( const Satelite& origen );

  inline float getTraslacion ( float hours ) const { return ( _traslacion * hours ); };
  inline void setTraslacion ( float traslacion ) { _traslacion = traslacion; };

  inline float getRotacion ( float hours ) const { return ( _rotacion * hours ); };
  inline void setRotacion ( float rotacion ) { _rotacion = rotacion; };
  
  void print();

};

#endif
