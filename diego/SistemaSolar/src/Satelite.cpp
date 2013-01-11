#include "Satelite.h"

void Satelite::Copia ( const Satelite& origen )
{
  Astro::Copia ( origen );
  // _id = origen.getID();
  // _posX = origen.getX();
  // _posY = origen.getY();
  // _posZ = origen.getZ();
  // _rojo = origen.getRojo();
  // _verde = origen.getVerde();
  // _azul = origen.getAzul();
  // _radio = origen.getRadio();

  _traslacion = origen.getTraslacion ( 1 );
  _rotacion = origen.getRotacion ( 1 );
}

Satelite::Satelite ( const Satelite& origen )
{
  Copia ( origen );
}

Satelite Satelite::operator= ( const Satelite& origen )
{
  Copia ( origen );
  return *this;
}

void Satelite::print()
{
  Astro::print();

  cout << "Traslacion  : " << _traslacion << endl;
  cout << "Rot.Interna : " << _rotacion << endl;
}
