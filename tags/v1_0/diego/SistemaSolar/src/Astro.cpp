#include "Astro.h"

void Astro::Copia ( const Astro& origen )
{
  _id = origen.getID();
  _posX = origen.getPosX();
  _posY = origen.getPosY();
  _posZ = origen.getPosZ();
  _rojo = origen.getRojo();
  _verde = origen.getVerde();
  _azul = origen.getAzul();
  _radio = origen.getRadio();
}

Astro::Astro ( const Astro& origen )
{
  Copia ( origen );
}

Astro Astro::operator= ( const Astro& origen )
{
  Copia ( origen );
  return *this;
}

void Astro::print()
{
  cout << "ID    : " << _id << endl;
  cout << "X,Y,Z : " << _posX << "," << _posY << "," << _posZ << endl;
  cout << "R,G,B : " << _rojo << "," << _verde << "," << _azul << endl;
  cout << "Radio : " << _radio << endl;
}
