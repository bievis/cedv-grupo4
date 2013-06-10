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

  for ( unsigned int i = 0; i < source.getNumPoints(); i++ )
    {
      source.getPoint ( i, v );
      addPoint ( v );
    }
}

void EnemyRoute::addPoint ( const Ogre::Vector3& v )
{
  _vPoints.push_back ( v );
}

void EnemyRoute::getPoint ( unsigned int index, Ogre::Vector3& v )
{
  if ( ( index < _vPoints.size() ) && ( index >= 0 ) )
    {
      v = _vPoints[index];
    }
}

void EnemyRoute::print()
{
  cout << "========================" << endl;
  cout << " - Route " << _id << " : Closed = " << (_closed?"true":"false") << endl;
  cout << "========================" << endl;
  for ( unsigned int i = 0; i < _vPoints.size(); i++ )
    {
      cout << "Coordenada : " << _vPoints[i] << endl;
    }

}
