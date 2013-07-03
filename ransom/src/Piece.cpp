#include "Piece.h"
#include <iostream>

Piece::Piece()
  {
    clear();
  }

Piece::~Piece()
  {
    clear();
  }

void Piece::clear()
{
  for ( std::deque<EnemyRoute *>::iterator itRoute = m_routes.begin(); m_routes.end() != itRoute; ++itRoute )
    {
      if ( *itRoute )
        delete *itRoute;
    }
  m_routes.clear();
  m_height = 0;
  m_width = 0;
  m_pos = Ogre::Vector3::ZERO;

}

Piece::Piece(const Piece& other)
{
  copy ( other );
}

Piece& Piece::operator=(const Piece& rhs)
{
  if (this == &rhs) return *this; // handle self assignment

  copy ( rhs );

  return *this;
}

void Piece::copy ( const Piece& source )
  {
    m_height = source.getHeight();
    m_width = source.getWidth();
    m_pos = source.getPosition();

    for ( unsigned int i = 0; i != source.getNumEnemyRoutes(); ++i )
      {
        if ( source.m_routes[i] )
          {
            addEnemyRoute ( *(source.m_routes[i]) );
          }
      }
  }

void Piece::print()
  {
    std::cout << "   Height        : " << m_height << std::endl;
    std::cout << "   Width         : " << m_width << std::endl;
    std::cout << "   Position      : " << m_pos << std::endl;
    std::cout << "   Num. Routes   : " << m_routes.size() << std::endl;

    for ( std::deque<EnemyRoute *>::const_iterator itRoute = m_routes.begin(); m_routes.end() != itRoute; ++itRoute )
      {
        if ( *itRoute)
          std::cout << "      Route ID   : " << (*itRoute)->getID() << std::endl;
  //        (*itRoute)->print();
      }
  }

void Piece::addEnemyRoute ( const EnemyRoute& newRoute )
  {
    EnemyRoute* ptr = new EnemyRoute ( newRoute );
    m_routes.push_back ( ptr );
  }

EnemyRoute* Piece::getEnemyRoute ( unsigned int index )
  {
    EnemyRoute* ptr = NULL;

    if ( index < m_routes.size() )
      ptr = m_routes[index];

    return ptr;
  }
