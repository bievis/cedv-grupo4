#include "Triangle.h"
#include <iostream>
using namespace std;

Triangle::Triangle ()
{
#ifdef __DEBUG__
  cout << "Triangle::Constructor (default)" << endl;
#endif
  _v1 = new Vec3 ();
  _v2 = new Vec3 ();
  _v3 = new Vec3 ();
}

Triangle::Triangle ( const Vec3 &v1, const Vec3 &v2, const Vec3 &v3 )
{
#ifdef __DEBUG__
  cout << "Triangle::Constructor (vectores)" << endl;
#endif
  _v1 = new Vec3 ( v1.getX(), v1.getY(), v1.getZ() );
  _v2 = new Vec3 ( v2.getX(), v2.getY(), v2.getZ() );
  _v3 = new Vec3 ( v3.getX(), v3.getY(), v3.getZ() );
}

Triangle::Triangle ( const Triangle &t )
{
#ifdef __DEBUG__
  cout << "Triangle::Constructor (triangulo)" << endl;
#endif
  copy ( t );
}

Triangle::~Triangle()
{
#ifdef __DEBUG__
  cout << "Triangle::Destructor" << endl;
#endif
  if ( _v1 )
    delete _v1;
  if ( _v2 )
    delete _v2;
  if ( _v3 )
    delete _v3;

  _v1 = _v2 = _v3 = 0;

}  

Vec3 Triangle::getV1() const
{
  return *_v1;
}

Vec3 Triangle::getV2() const
{
  return *_v2;
}

Vec3 Triangle::getV3() const
{
  return *_v3;
}

void Triangle::setV1(const Vec3 &v)
{
  if (_v1)
    delete _v1;

  _v1 = new Vec3(v);
}

void Triangle::setV2(const Vec3 &v)
{
  if (_v2)
    delete _v2;

  _v2 = new Vec3(v);
}

void Triangle::setV3(const Vec3 &v)
{
  if (_v3)
    delete _v3;

  _v3 = new Vec3(v);
}

Triangle& Triangle::operator= ( const Triangle &t )
{
  cout << "Triangle::operator=" << endl;
  copy ( t );

  return *this;
}

void Triangle::copy ( const Triangle &t ) 
{
#ifdef __DEBUG__
  cout << "Triangle::copy" << endl;
#endif
  if ( _v1 )
    delete _v1;
  if ( _v2 )
    delete _v2;
  if ( _v3 )
    delete _v3;
  
  _v1 = new Vec3 ( t.getV1().getX(), t.getV1().getY(), t.getV1().getZ() );
  _v2 = new Vec3 ( t.getV2().getX(), t.getV2().getY(), t.getV2().getZ() );
  _v3 = new Vec3 ( t.getV3().getX(), t.getV3().getY(), t.getV3().getZ() );
}
