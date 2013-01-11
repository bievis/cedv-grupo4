#ifndef _ASTRO_H_
#define _ASTRO_H_

#include <iostream>

using namespace std;

//Astro, para modelar cuerpos celetes. Un astro tiene un nombre (id único), una posición en el espacio 3D, un color y un radio.
class Astro {

  string _id;
  float _posX, _posY, _posZ;
  int _rojo, _verde, _azul;
  float _radio;

 public:

  Astro() : _id(""), _posX(0.0), _posY(0.0), _posZ(0.0), _rojo(0), _verde(0), _azul(0), _radio(0.0) {};
  virtual ~Astro() {};

  Astro ( const Astro& origen );
  Astro operator= ( const Astro& origen );
  
  inline string getID() const { return _id; };
  inline void setID ( string id ) { _id = id; };
  
  inline float getPosX() const { return _posX; };
  inline void setPosX ( float posX ) { _posX = posX; };

  inline float getPosY() const { return _posY; };
  inline void setPosY ( float posY ) { _posY = posY; };

  inline float getPosZ() const { return _posZ; };
  inline void setPosZ ( float posZ ) { _posZ = posZ; };

  inline float getRojo() const { return _rojo; };
  inline void setRojo ( float rojo ) { _rojo = rojo; };

  inline float getVerde() const { return _verde; };
  inline void setVerde ( float verde ) { _verde = verde; };

  inline float getAzul() const { return _azul; };
  inline void setAzul ( float azul ) { _azul = azul; };

  inline float getRadio() const { return _radio; };
  inline void setRadio ( float radio ) { _radio = radio; };

  void print();

  void Copia ( const Astro& origen );

};

#endif
