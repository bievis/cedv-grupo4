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
    Ogre::Vector3 size = Ogre::Vector3::ZERO;
    _entityDummy = sceneMgr->createEntity ( name + "DUMMY", MESH_FILE_WITHOUT_EXTENSION + string ( ".mesh" ) );
    _entityDummy->setCastShadows(false);
    _entityDummy->setVisible(false);

    _nodeDummy = sceneMgr->createSceneNode ( name + "DUMMY" );
	_nodeDummy->setPosition(0, (_entityDummy->getBoundingBox().getSize().y / -2), 0);
    _nodeDummy->attachObject ( _entityDummy );
	// Atachamos el dummy para que se mueva con el personaje
    _node->addChild ( _nodeDummy );

    //Material del Heroe
    _entity->setMaterialName ( "MaterialAzul" );

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
