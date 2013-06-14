#include "Hostage.h"

Hostage::Hostage( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    const Ogre::Vector3& initial_pos ) : Character ( sceneMgr,
                                                        world,
                                                        name,
                                                        initial_pos,
                                                        HOSTAGE, MOVE_ANIMATION )
  {
    //Material del Hostagee
    _entity->setMaterialName ( "MaterialAmarillo" );

    _entityDummy->setMaterialName ( "MaterialAmarillo" );
  }

Hostage::~Hostage()
{
}

Hostage::Hostage(const Hostage& other) : Character ( other )
{
  copy ( other );
}

Hostage& Hostage::operator=(const Hostage& rhs)
{
  if (this == &rhs) return *this; // handle self assignment

  copy ( rhs );

  return *this;
}

void Hostage::copy ( const Hostage& source )
  {
	
  }

void Hostage::changeAnimation(string nameAnimation) {

}

void Hostage::print()
{
  cout << "==============" << endl;
  cout << "Hostage Info" << endl;
  cout << "==============" << endl;

  Character::print();
}
