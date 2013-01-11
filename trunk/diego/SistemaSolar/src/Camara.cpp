#include "Camara.h"
#include <iostream>

using namespace std;

void Camara::Copia ( const Camara& origen )
{
  _eyeX = origen.getEyeX();
  _eyeY = origen.getEyeY();
  _eyeZ = origen.getEyeZ();

  _centerX = origen.getCenterX();
  _centerY = origen.getCenterY();
  _centerZ = origen.getCenterZ();

  _upX = origen.getUpX();
  _upY = origen.getUpY();
  _upZ = origen.getUpZ();
}


Camara::Camara ( const Camara& origen )
{
  Copia ( origen );
}

Camara Camara::operator= ( const Camara& origen )
{
  Copia ( origen );
  return *this;
}

void Camara::print()
{
  cout << "Eye    : X(" << _eyeX << ") Y(" << _eyeY << ") Z(" << _eyeZ << ")" << endl;
  cout << "Center : X(" << _centerX << ") Y(" << _centerY << ") Z(" << _centerZ << ")" << endl;
  cout << "Up     : X(" << _upX << ") Y(" << _upY << ") Z(" << _upZ << ")" << endl;
}
