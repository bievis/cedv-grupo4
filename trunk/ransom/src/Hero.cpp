#include "Hero.h"

Hero::Hero( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    float initial_pos_X,
                    float initial_pos_Y,
                    float initial_pos_Z ) : Character ( sceneMgr,
                                                        world,
                                                        name,
                                                        initial_pos_X,
                                                        initial_pos_Y,
                                                        initial_pos_Z,
                                                        false )
  {
    Ogre::Vector3 size = Ogre::Vector3::ZERO;
    _entityDummy = sceneMgr->createEntity ( name + "DUMMY", MESH_FILE_WITHOUT_EXTENSION + string ( ".mesh" ) );
    _entityDummy->setCastShadows(false);
    _entityDummy->setVisible(false);

    _nodeDummy = sceneMgr->createSceneNode ( name + "DUMMY" );

    _nodeDummy->attachObject ( _entityDummy );

    _nodeDummy->setPosition ( Ogre::Vector3 ( initial_pos_X, initial_pos_Y, initial_pos_Z ) );

    sceneMgr->getRootSceneNode()->addChild ( _nodeDummy );

    //Material del Heroe
    //_entity->setMaterialName ( "MaterialAzul" );

    _entityDummy->setMaterialName ( "MaterialBlanco" );
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
    _entityDummy = source.getEntityDUMMY();
    _nodeDummy = source.getSceneNodeDUMMY();
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

void Hero::walk ( bool reverse )
  {
    Character::walk ( reverse );

    Ogre::Quaternion qt = Character::getSceneNode()->getOrientation();

    _nodeDummy->setOrientation ( qt );

    Ogre::Vector3 v = Character::getSceneNode()->getPosition();

    _nodeDummy->setPosition ( v );
  }

void Hero::turn ( Ogre::Real angle )
  {
    Character::turn ( angle );

    Ogre::Quaternion qt = Character::getSceneNode()->getOrientation();

    _nodeDummy->setOrientation ( qt );

    Ogre::Vector3 v = Character::getSceneNode()->getPosition();

    _nodeDummy->setPosition ( v );
  }
