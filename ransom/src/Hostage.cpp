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
	_timerParticleLiberate = 0.0;

    //Material del Hostagee
    _entity->setMaterialName ( "MaterialAmarillo" );

    _entityDummy->setMaterialName ( "MaterialAmarillo" );

	_state = CAPTURE;

	// Creamos el sistema de partículas
	_particleLiberation = _sceneMgr->createParticleSystem("particleHostage" + _name, "particleHostage");
	_particleLiberation->setVisible(false);
	// Creamos un nodo
	_particleLiberationNode = _node->createChildSceneNode("particleHostageNode" + _name);
	// Ajuntamos las partículas al nodo
	_particleLiberationNode->attachObject(_particleLiberation);
  }

Hostage::~Hostage()
{ 
	// Destruimos el nodo
	_sceneMgr->destroySceneNode(_particleLiberationNode);
	// Destruimos el sistema de partículas
	_sceneMgr->destroyParticleSystem(_particleLiberation);
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
	  _particleLiberation = source._particleLiberation;
	  _particleLiberationNode = source._particleLiberationNode;
	  _timerParticleLiberate = source._timerParticleLiberate;
  }

void Hostage::changeAnimation(string nameAnimation) {

}

void Hostage::liberate() {
	_state = LIBERATE;
	setVisible(false);
}

void Hostage::setVisible ( const bool visible ) {
	Character::setVisible(visible);
	_particleLiberation->setVisible(!visible);
	if (visible) _state = CAPTURE;
}

void Hostage::update ( double timeSinceLastFrame, std::vector<Character*>   vCharacteres) {
	Character::update(timeSinceLastFrame, vCharacteres);
	
	if (_stateCaracter != END) {
		if (_state == LIBERATE) {
			_timerParticleLiberate += timeSinceLastFrame;
			_particleLiberation->setEmitting(true);
			if (_timerParticleLiberate > TIMER_PATICLE_LIBERATE) {
				_particleLiberation->setEmitting(false);
				_state = LIBERATED;
				_stateCaracter = END;
			}
		} else {
			_timerParticleLiberate = 0.0;
		}
	}
}

void Hostage::print()
{
  cout << "==============" << endl;
  cout << "Hostage Info" << endl;
  cout << "==============" << endl;

  Character::print();
}
