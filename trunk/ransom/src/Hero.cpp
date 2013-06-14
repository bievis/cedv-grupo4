#include "Hero.h"

Hero::Hero( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    const Ogre::Vector3& initial_pos ) : Character ( sceneMgr,
                                                        world,
                                                        name,
                                                        initial_pos,
                                                        false )
  {
    //Material del Heroe
    _entity->setMaterialName ( "MaterialAzul" );

    _entityDummy->setMaterialName ( "MaterialAzul" );
  }

Hero::~Hero()
{
}

Hero::Hero(const Hero& other) : Character ( other )
{
  copy ( other );
}

Hero& Hero::operator=(const Hero& rhs)
{
  if (this == &rhs) return *this; // handle self assignment

  copy ( rhs );

  return *this;
}

void Hero::copy ( const Hero& source )
  {
	
  }

void Hero::print()
{
  cout << "==============" << endl;
  cout << "Hero Info" << endl;
  cout << "==============" << endl;

  Character::print();

  printf ( "entity DUMMY ref.: %p\n", _entityDummy );
  printf ( "node ref.        : %p\n", _nodeDummy );
  cout << "==============" << endl;
}
