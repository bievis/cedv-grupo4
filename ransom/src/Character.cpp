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
	_timerParticleDeath = 0.0;
	_distanceWithOtherCaracter = 0.0;

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

	// Cargamos los sonidos
    _soundDeath1FX = SoundFXManager::getSingleton().load("death1.wav");
    _soundDeath2FX = SoundFXManager::getSingleton().load("death2.wav");
    _soundDeath3FX = SoundFXManager::getSingleton().load("death3.wav");

	// Creamos el sistema de partículas
	_particleDeath = _sceneMgr->createParticleSystem("particleDeath" + _name, "particleDeath");
	_particleDeath->setVisible(false);
	// Creamos un nodo
	_particleDeathNode = _node->createChildSceneNode("particleDeathNode" + _name);
	// Ajuntamos las partículas al nodo
	_particleDeathNode->attachObject(_particleDeath);

	// Creamos el disparo, cuando no sea Rehen
	if (type != HOSTAGE) {
		_soundShootFX = SoundFXManager::getSingleton().load("shoot.wav");

		_nodeShot = sceneMgr->getRootSceneNode()->createChildSceneNode( name + "SHOT" );
		_nodeShot->setPosition((_rigidBody->getCenterOfMassOrientation() * POSITION_SHOT)
								+ _rigidBody->getCenterOfMassPosition());
		_nodeShot->setOrientation(_rigidBody->getCenterOfMassOrientation());
		Utilities::getSingleton().put_shot_in_scene(sceneMgr, name,
													_nodeShot, &_shot);
		_nodeShot->setVisible(false);
	}

    _soundHurt1FX = SoundFXManager::getSingleton().load("hurt1.wav");
    _soundHurt2FX = SoundFXManager::getSingleton().load("hurt2.wav");

    _currentSoundHurt = 1;

    //_soundWalkFX = SoundFXManager::getSingleton().load("walk.wav");

    _soundMiss1FX = SoundFXManager::getSingleton().load("missed_shot1.wav");
    _soundMiss2FX = SoundFXManager::getSingleton().load("missed_shot2.wav");

 }

Character::~Character()
{
	_node->detachAllObjects();
	_sceneMgr->destroySceneNode(_node);
	_rigidBody = NULL;
	if (_nodeShot) {
		_nodeShot->detachAllObjects();
		_sceneMgr->destroySceneNode(_nodeShot);
		_sceneMgr->destroyManualObject(_shot);
	}
	_sceneMgr->destroyEntity(_entity);
	_nodeDummy->detachAllObjects();
	_sceneMgr->destroySceneNode(_nodeDummy);
	_sceneMgr->destroyEntity(_entityDummy);
	_visible = true;
	_world = NULL;
	_particleDeathNode->detachAllObjects();
	_sceneMgr->destroySceneNode(_particleDeathNode);
	_sceneMgr->destroyParticleSystem(_particleDeath);
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
	_soundShootFX = source._soundShootFX;
	_soundDeath1FX = source._soundDeath1FX;
	_soundDeath2FX = source._soundDeath2FX;
	_soundDeath3FX = source._soundDeath3FX;
	_soundHurt1FX = source._soundHurt1FX;
	_soundHurt2FX = source._soundHurt2FX;
//	_soundWalkFX = source._soundWalkFX;
	_soundMiss1FX = source._soundHurt1FX;
	_soundMiss2FX = source._soundHurt2FX;
	_timerParticleDeath = source._timerParticleDeath;
	_particleDeath = source._particleDeath;
	_particleDeathNode = source._particleDeathNode;
	_distanceWithOtherCaracter = source._distanceWithOtherCaracter;
}

void Character::setHealth ( float newHealth )
{
	if ( _health <= 100 )
	{
		if ( newHealth > 100 )
			_health = 100;
		else if ( newHealth <= 0 ) {
			// Si ha muerto
			death();
		}
		else
			_health = newHealth;
	}
}

void Character::death() {
	_health = 0;
	_stateCaracter = DEAD;
	setVisible (false);
	_particleDeath->setVisible(true);
	play_sound_death();
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
	if (_stateCaracter != END) {
		if (_stateCaracter == DEAD) {
			_timerParticleDeath += timeSinceLastFrame;
			_particleDeath->setEmitting(true);
			if (_timerParticleDeath > TIMER_PATICLE_DEATH) {
				_particleDeath->setEmitting(false);
				_stateCaracter = END;
			}
		} else {
			if ( _currentAnimation != NULL )
			{
				_currentAnimation->addTime(timeSinceLastFrame * VELOCIDAD_ANIMACION);
			}

			if (_nodeShot && _isShooting) {
				updateShot(timeSinceLastFrame, vCharacteres);
			}

			_timerParticleDeath = 0.0;
		}
	}
}

void Character::updateShot ( double timeSinceLastFrame, std::vector<Character*>   vCharacteres) {
	// Colocamos el disparo
	_nodeShot->setPosition((_rigidBody->getCenterOfMassOrientation() * POSITION_SHOT)
							+ _rigidBody->getCenterOfMassPosition());
	_nodeShot->setOrientation(_rigidBody->getCenterOfMassOrientation());
	// Vemos si el disparo ha colisionado con algun elemento
	Character* shootingCharacter = NULL;
	if (detectCollisionShot(_world, vCharacteres, &shootingCharacter)) {
		// Si hemos dado a algo
		_nodeShot->setVisible(false);
		_isShooting = false;
		// Vemos a que le hemos dado otro personaje
		if (shootingCharacter) {
				double rate = 0.0;

				// Vemos si le hemos pasado la distancia para que calcule aleatoriamente si le damos o no
				if (_distanceWithOtherCaracter == ZERO_DISTANCE || 
					validate_success_rate ( _distanceWithOtherCaracter, &rate ) )
				{
					cout << "TOCADO!!! (" << rate << "%) distance = " << _distanceWithOtherCaracter << endl;
	    		shootingCharacter->setHealth(shootingCharacter->getHealth() - HEALTH_SHOT);
                  play_sound_hurt();
		        }
				else
                 {
					cout << "AGUA!!! (" << rate << "%) distance = " << _distanceWithOtherCaracter << endl;
                    play_sound_miss();
                 }
			}
         else
          {
            cout << "AGUA!!!" << endl;
            play_sound_miss();
          }
	} else {
		// Vamos alargando el disparo
		setScaleShot ((timeSinceLastFrame * VELOCITY_SHOT) + _nodeShot->getScale().z);
	}
}

bool Character::validate_success_rate ( double distance, double* rate )
  {
    bool res = false;

    //Numeros aleatorios entre 1 y 100
    *rate = 1 + rand() % ( 100 );

    // Si la distancia al heroe está entre 5 y 10
    // entonces si el valor está por encima del 50%
    // nos habrá dado, sino fallo
    if ( ( distance > 5 ) && ( distance < 10 ) )
      {
        // Del 50% en adelante nos habrá acertado
        if ( *rate > 50 )
        {
          res = true;
        }
      }
    // Si la distancia al heroe está entre 10 y 15
    // entonces si el valor está por encima del 65%
    // nos habrá dado, sino fallo
    if ( ( distance > 10 ) && ( distance < 15 ) )
      {
        // Del 65% en adelante nos habrá acertado
        if ( *rate > 65 )
          {
            res = true;
          }
      }
    // Si la distancia al heroe está es mayor a 20
    // entonces si el valor está por encima del 75%
    // nos habrá dado, sino fallo
    if ( distance > 20 )
      {
        // Del 75% en adelante nos habrá acertado
        if ( *rate > 75 )
          {
            res = true;
          }
      }

    return res;
  }

void Character::shoot(double distanceWithOtherCaracter) {
	if (!_isShooting && _stateCaracter == LIVE) {
		_soundShootFX->play();
		_nodeShot->setVisible(true);
		setScaleShot (0.1f);
		_isShooting = true;
		_distanceWithOtherCaracter = distanceWithOtherCaracter;
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
					if (obOB_A == character->getRigidBody() && character->_stateCaracter == LIVE) {
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

	if (!_isShooting && _stateCaracter == LIVE) {
		if (reverse) velocidad *= -1;

//    NOTA: Al estar dentro del update, hace que se repita muchas veces el sonido y peta
//    if ( _type == HERO )
//      _soundWalkFX->play();

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
	if (!_isShooting && _stateCaracter == LIVE) {
		_rigidBody->enableActiveState();
		_node->yaw ( Ogre::Radian(Ogre::Math::HALF_PI / 32) );
		btQuaternion quaternion = OgreBulletCollisions::OgreBtConverter::to(_node->getOrientation());
		_rigidBody->getBulletRigidBody()->getWorldTransform().setRotation(quaternion);
	}
  }

void Character::turn_right()
  {

    assert ( _node );
	if (!_isShooting && _stateCaracter == LIVE) {
		_rigidBody->enableActiveState();
		_node->yaw ( Ogre::Radian((-1)* Ogre::Math::HALF_PI / 32) );
		btQuaternion quaternion = OgreBulletCollisions::OgreBtConverter::to(_node->getOrientation());
		_rigidBody->getBulletRigidBody()->getWorldTransform().setRotation(quaternion);
	}
  }

void Character::play_sound_death()
{
	//Numeros aleatorios entre 1 y 3
	double _currentSoundDeath = 1 + rand() % ( 3 );
	if ( _currentSoundDeath == 1 )
	{
		_soundDeath1FX->play();
	}
	else if ( _currentSoundDeath == 2 )
	{
		_soundDeath2FX->play();
	}
	else
	{
		_soundDeath3FX->play();
	}
}

void Character::showDummy(bool show) {
	this->getEntity()->setVisible(!show);
	this->getEntityDummy()->setVisible(show);
	if (_isShooting)
		this->getNodeShot()->setVisible(!show);
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

void Character::play_sound_hurt()
{
  if ( _currentSoundHurt == 1 )
    {
      _soundHurt1FX->play();
      _currentSoundHurt = 2;
    }
  else
    {
      _soundHurt2FX->play();
      _currentSoundHurt = 1;
    }

  }

void Character::play_sound_miss()
{
  if ( _currentSoundMiss == 1 )
    {
      _soundMiss1FX->play();
      _currentSoundMiss = 2;
    }
  else
    {
      _soundMiss2FX->play();
      _currentSoundMiss = 1;
    }

  }
