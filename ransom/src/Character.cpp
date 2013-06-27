#include "Character.h"
#include <cassert>

Character::Character ( Ogre::SceneManager* sceneMgr,
                        OgreBulletDynamics::DynamicsWorld* world,
                        const string& name,
                        const Ogre::Vector3& v_pos,
                        CHARACTER_TYPE type, string animation ) : _name(name),
                                                                  _sceneMgr(sceneMgr),
                                                                  _v_pos(v_pos)
  {
	_type = type;
    _node = NULL;
    _rigidBody = NULL;
	_shot = NULL;
	_nodeShot = NULL;
	_nodeDummy = NULL;
	_entityDummy = NULL;
    _visible = true;
    _world = world;
	_stateCaracter = LIVE;
	_isShooting = false;

    _health = MAX_HEALTH;

    string mesh = HERO_MESH_FILE_WITHOUT_EXTENSION;

    if ( type == ENEMY )
      {
        mesh = ENEMY_MESH_FILE_WITHOUT_EXTENSION;
      }
    else if (type == HOSTAGE)
      {
        mesh = HOSTAGE_MESH_FILE_WITHOUT_EXTENSION;
      }

    Utilities::getSingleton().put_character_in_scene ( sceneMgr,
                                                  world,
                                                  mesh,
                                                  name,
                                                  v_pos,
                                                  &_entity,
                                                  &_node,
                                                  &_rigidBody,
                                                  true,
                                                  animation);

    if (animation == "")
      {
        _currentAnimation = NULL;
      }
    else
      {
        _currentAnimation = _entity->getAnimationState(animation);
      }

    // Creamos el Dummy para la vision y para el mini mapa
    _entityDummy = sceneMgr->createEntity ( name + "DUMMY", CUBE_MESH_FILE_WITHOUT_EXTENSION + string ( ".mesh" ) );
    _entityDummy->setCastShadows(false);
    _entityDummy->setVisible(false);

	_nodeDummy = _node->createChildSceneNode ( name + "DUMMY" );
    _nodeDummy->setPosition(0, (_entityDummy->getBoundingBox().getSize().y / -2), 0);
    _nodeDummy->attachObject ( _entityDummy );

	// Creamos el disparo, cuando no sea Rehen
	if (type != HOSTAGE) {
		_sonidoShootFX = SoundFXManager::getSingleton().load("shoot.wav");

		_nodeShot = sceneMgr->getRootSceneNode()->createChildSceneNode( name + "SHOT" );
		_nodeShot->setPosition((_rigidBody->getCenterOfMassOrientation() * POSITION_SHOT)
								+ _rigidBody->getCenterOfMassPosition());
		_nodeShot->setOrientation(_rigidBody->getCenterOfMassOrientation());
		Utilities::getSingleton().put_shot_in_scene(sceneMgr, name, 
													_nodeShot, &_shot);
		_nodeShot->setVisible(false);
	}
 }

Character::~Character()
  {
	_node = NULL;
    _rigidBody = NULL;
	_shot = NULL;
	_nodeShot = NULL;
	_nodeDummy = NULL;
	_entityDummy = NULL;
    _visible = true;
    _world = NULL;
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
    _name = source.getName();
	_type = source.getType();
    _health = source.getHealth();
    _v_pos = source.getInitial_Pos();
    _node = source.getSceneNode();
    _rigidBody = source.getRigidBody();
    _currentAnimation = source._currentAnimation;
	_shot = source._shot;
	_nodeShot = source._nodeShot;
	_nodeDummy = source._nodeDummy;
	_entityDummy = source._entityDummy;
    _visible = source._visible;
    _world = source._world;
	_stateCaracter = source._stateCaracter;
	_isShooting = source._isShooting;
	_sonidoShootFX = source._sonidoShootFX;
}

void Character::setHealth ( float newHealth )
  {
    if ( _health <= 100 )
      {
        if ( newHealth > 100 )
          _health = 100;
        else if ( newHealth < 0 ) {
			_health = 0;
			_stateCaracter = DEAD;
			setVisible (false);
		}
        else
          _health = newHealth;
      }
  }

void Character::changeAnimation ( const string& nameAnimation )
  {
    if ( ( _currentAnimation != NULL ) &&
        ( _currentAnimation->getAnimationName() != nameAnimation ) )
      {
        Ogre::AnimationState *animation;

        if ( STOP_ANIMATION == nameAnimation )
          {
            animation = (_entity)->getAnimationState(MOVE_ANIMATION);
            animation->setEnabled(false);
          }
        else if ( MOVE_ANIMATION == nameAnimation )
          {
            animation = (_entity)->getAnimationState(STOP_ANIMATION);
            animation->setEnabled(false);
          }

        animation = (_entity)->getAnimationState(nameAnimation);
        animation->setEnabled(true);
        animation->setLoop(true);
        _currentAnimation = animation;
      }
  }

void Character::update ( double timeSinceLastFrame, std::vector<Character*>   vCharacteres)
  {
    if ( _currentAnimation != NULL )
      {
        _currentAnimation->addTime(timeSinceLastFrame * VELOCIDAD_ANIMACION);
      }
	
	if (_nodeShot && _isShooting) {
		// Colocamos el disparo
		_nodeShot->setPosition((_rigidBody->getCenterOfMassOrientation() * POSITION_SHOT)
								+ _rigidBody->getCenterOfMassPosition());
		_nodeShot->setOrientation(_rigidBody->getCenterOfMassOrientation());
		// Vamos alargando el disparo
		setScaleShot ((timeSinceLastFrame * VELOCITY_SHOT) + _nodeShot->getScale().z);
		Character* shootingCharacter = NULL;
		if (detectCollisionShot(_world, vCharacteres, &shootingCharacter)) {
			// Si hemos dado a algo
			_nodeShot->setVisible(false);
			_isShooting = false;
			// Vemos a que le hemos dado otro personaje
			if (shootingCharacter) {
				shootingCharacter->setHealth(shootingCharacter->getHealth() - HEALTH_SHOT);
			}
		}
	}
}

void Character::shoot() {
	if (!_isShooting) {
		_sonidoShootFX->play();
		_nodeShot->setVisible(true);
		setScaleShot (1.0f);
		_isShooting = true;
	}
}

void Character::setScaleShot (Ogre::Real scaleZ) {
	if (_nodeShot) {
		Ogre::Vector3 newScale(1.0f, 1.0f, scaleZ);
		_nodeShot->setScale(newScale);
	}
}

bool Character::detectCollisionShot(OgreBulletDynamics::DynamicsWorld* world,
												std::vector<Character*> characteres,
												Character** characterCollision) {
	bool isCollition = false;
	(*characterCollision) = NULL;

	if (_nodeShot) {
		btVector3 posIni = OgreBulletCollisions::OgreBtConverter::to(_nodeShot->getPosition());
		btVector3 posFin = posIni + OgreBulletCollisions::OgreBtConverter::to(_nodeShot->getOrientation().zAxis() 
																		* _nodeShot->getScale().z);

		// Start and End are vectors
		btCollisionWorld::ClosestRayResultCallback rayCallback(posIni, posFin);

		// Perform raycast
		world->getBulletDynamicsWorld ()->rayTest(posIni, posFin, rayCallback);

		if(rayCallback.hasHit()) {
			btCollisionObject* obA = (btCollisionObject*)(rayCallback.m_collisionObject);
			OgreBulletCollisions::Object *obOB_A = world->findObject(obA);
			int sizeCharacteres = characteres.size();
			Character *character = NULL;

			if (obOB_A != getRigidBody()) {
				for (int i = 0; i < sizeCharacteres && (*characterCollision) == NULL; i++) {
					character = characteres[i];
					if (obOB_A == character->getRigidBody()) {
						(*characterCollision) = character;
					}
				}
			}
			isCollition = true;
		}
	}

	return isCollition;
}

void Character::walk ( bool reverse, float velocidad )
  {
    assert ( _rigidBody );

	if (!_isShooting) {
		if (reverse) velocidad *= -1;

		_rigidBody->enableActiveState();
		Ogre::Vector3 orientacion = _rigidBody->getCenterOfMassOrientation() * Ogre::Vector3::UNIT_Z;
		orientacion.y = 0.0; // Movemos solo en los ejes Z y X
		_rigidBody->setLinearVelocity(orientacion * velocidad);

		changeAnimation(MOVE_ANIMATION);
	}

  }

//bool Character::walk_to ( const Ogre::Vector3& p )
//  {
//    bool res = false;
//    Ogre::Vector3 o = _rigidBody->getSceneNode()->getPosition();
//
//    Ogre::Vector3 v = p - o; // 1er vector
//
//    Ogre::Vector3 orientacion = _rigidBody->getCenterOfMassOrientation() * Ogre::Vector3::UNIT_Z; // 2do vector
//
//    Ogre::Radian angle = orientacion.angleBetween ( v );
//
//    Ogre::Real distance = o.distance ( p );
//
////    cout << " prueba = " << orientacion.getRotationTo(v).getYaw().valueDegrees() << endl;
////    cout << " angle = " << angle.valueAngleUnits() << endl;
////    cout << " distance = " << distance << endl;
//
//    if ( distance < 2.9 )
//      res = true;
//
//    if ( angle.valueDegrees() > 15 )
//    {
//      if ( orientacion.getRotationTo(v).getYaw().valueDegrees() > 0 )
//        turn_left();
//      else
//        turn_right();
//    }
//
//    walk();
//
//    return res;
//  }

void Character::stop_move()
  {
    _rigidBody->setLinearVelocity(Ogre::Vector3::ZERO);
    changeAnimation(STOP_ANIMATION);
  }

//void Character::turn ( Ogre::Real angle )
//  {
//
//    assert ( _node );
//
//    _rigidBody->enableActiveState();
//    _node->yaw ( Ogre::Radian(angle) );
//    btQuaternion quaternion = OgreBulletCollisions::OgreBtConverter::to(_node->getOrientation());
//    _rigidBody->getBulletRigidBody()->getWorldTransform().setRotation(quaternion);
//
//  }

void Character::turn_left()
  {

    assert ( _node );
	if (!_isShooting) {
		_rigidBody->enableActiveState();
		_node->yaw ( Ogre::Radian(Ogre::Math::HALF_PI / 32) );
		btQuaternion quaternion = OgreBulletCollisions::OgreBtConverter::to(_node->getOrientation());
		_rigidBody->getBulletRigidBody()->getWorldTransform().setRotation(quaternion);
	}
  }

void Character::turn_right()
  {

    assert ( _node );
	if (!_isShooting) {
		_rigidBody->enableActiveState();
		_node->yaw ( Ogre::Radian((-1)* Ogre::Math::HALF_PI / 32) );
		btQuaternion quaternion = OgreBulletCollisions::OgreBtConverter::to(_node->getOrientation());
		_rigidBody->getBulletRigidBody()->getWorldTransform().setRotation(quaternion);
	}
  }

void Character::showDummy(bool show) {
	this->getEntity()->setVisible(!show);
	this->getEntityDummy()->setVisible(show);
	
}

void Character::setVisible ( const bool visible ) {
	_visible = visible;
	_node->setVisible(visible, true);
	if (_nodeShot) {
		_nodeShot->setVisible(visible);
	}
	if (visible) {
		_world->getBulletDynamicsWorld()->addCollisionObject(_rigidBody->getBulletObject());
	} else {
		_world->getBulletDynamicsWorld()->removeCollisionObject(_rigidBody->getBulletObject());
	}
}

void Character::print()
  {
    cout << "name              : " << _name << endl;
    cout << "health            : " << _health << endl;
    printf ( "node ref.         : %p\n", _node );
    printf ( "body ref.         : %p\n", _rigidBody );
    cout << "initial position  : " << _v_pos << endl;
  }

const Ogre::Vector3& Character::getPosition()
  {
    assert ( _node );
    return _node->getPosition();
  }

















