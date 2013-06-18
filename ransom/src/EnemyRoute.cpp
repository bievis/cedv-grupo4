#include "EnemyRoute.h"
#include <iostream>

using namespace std;

EnemyRoute::EnemyRoute()
{
  clear();
}

EnemyRoute::~EnemyRoute()
{
  clear();
}

void EnemyRoute::clear()
{
  _closed = false;

  _id = 0;

  _vPoints.clear();
}

EnemyRoute::EnemyRoute(const EnemyRoute& other)
{
  copy ( other );
}

EnemyRoute& EnemyRoute::operator=(const EnemyRoute& rhs)
{
  if (this == &rhs) return *this; // handle self assignment

  copy ( rhs );

  return *this;
}

void EnemyRoute::copy ( const EnemyRoute& source )
{
  Ogre::Vector3 v;

  clear();

  _id = source.getID();

  _closed = source.getRouteClosed();

  _proximity = source.getProximity();

  for ( int i = 0; i < source.getNumPoints(); i++ )
    {
      v = ((EnemyRoute&) source).getPoint ( i );
      addPoint ( v );
    }
}

void EnemyRoute::addPoint ( const Ogre::Vector3& v )
{
  _vPoints.push_back ( v );
}

const Ogre::Vector3& EnemyRoute::getPoint ( unsigned int index ) const
{
  assert ( index < _vPoints.size() );

  return _vPoints[index];
}

void EnemyRoute::print()
{
  cout << "========================" << endl;
  cout << " - Route " << _id << " : Closed = " << (_closed?"true":"false") << " : Proximity = " << _proximity << endl;
  cout << "========================" << endl;
  for ( unsigned int i = 0; i < _vPoints.size(); i++ )
    {
      cout << "Coordenada : " << _vPoints[i] << endl;
    }

}
