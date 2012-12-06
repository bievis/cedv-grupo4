#include "Vec3.h"
#include <iostream>
using namespace std;

Vec3::Vec3 (): _x(0), _y(0), _z(0) 
{
#ifdef __DEBUG__
  cout << "Vec3::Constructor (default)" << endl;
#endif
}

Vec3::Vec3 ( int x, int y, int z ) : _x(x), _y(y), _z(z) 
{
#ifdef __DEBUG__
  cout << "Vec3::Constructor (coordenadas)" << endl;
#endif
}

Vec3::~Vec3()
{
#ifdef __DEBUG__
  cout << "Vec3::Destructor" << endl;
#endif
}

int Vec3::getX() const
{
  return _x;
}

int Vec3::getY() const
{
  return _y;
}

int Vec3::getZ() const
{
  return _z;
}

void Vec3::setX(int x)
{
  _x = x;
}

void Vec3::setY(int y)
{
  _y = y;
}

void Vec3::setZ(int z)
{
  _z = z;
}

void Vec3::operator+ ( const Vec3 &op )
{
#ifdef __DEBUG__
  cout << "Vec3::operator+" << endl;
#endif
  _x += op.getX();
  _y += op.getY();
  _z += op.getZ();
}

void Vec3::operator- ( const Vec3 &op )
{
#ifdef __DEBUG__
  cout << "Vec3::operator-" << endl;
#endif
  _x -= op.getX();
  _y -= op.getY();
  _z -= op.getZ();
}
