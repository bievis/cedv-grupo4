#include "Character.h"
#include <cassert>

Character::Character ( Ogre::SceneManager* sceneMgr, OgreBulletDynamics::DynamicsWorld* world, const string& name )
{
//  m_mutex = PTHREAD_MUTEX_INITIALIZER;
  m_name = name;
  m_health = 100.0;
  m_node = Utilities::getSingleton().put_element_in_scene ( sceneMgr, world, "Cube" );
}

Character::~Character()
{
  //dtor
}

Character::Character ( const Character& other )
{
  copy ( other );
}

Character& Character::operator=(const Character& rhs)
{
  if (this == &rhs) return *this; // handle self assignment

  copy ( rhs );

  //assignment operator
  return *this;
}

void Character::copy ( const Character& source )
{
  m_name = source.getName();
  m_health = source.getHealth();
//  m_posX = source.getPosX();
//  m_posY = source.getPosY();
//  m_posZ = source.getPosZ();
}

float Character::getHealth() const
{
  return m_health;
}

void Character::setHealth ( float newHealth )
{
  if ( m_health < 100 )
  {
    if ( newHealth > 100 )
      m_health = 100;
    else
      m_health = newHealth;
  }
}

void Character::move ( float posX, float posY, float posZ )
{
//  pthread_mutex_lock( &m_mutex );

  assert ( m_node != NULL );

  if ( m_node )
    m_node->translate ( posX, posY, posZ );

//  pthread_mutex_unlock( &m_mutex );
}

void Character::print()
{
  cout << "==============" << endl;
  cout << "Character Info" << endl;
  cout << "==============" << endl;
  cout << "name     : " << m_name << endl;
  cout << "health   : " << m_health << endl;
  printf ( "node ref.: %p\n", m_node );
  cout << "==============" << endl;
}
