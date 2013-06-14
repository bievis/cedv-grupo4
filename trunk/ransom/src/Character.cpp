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
    _node = NULL;
    _rigidBody = NULL;

    _health = MAX_HEALTH;
	
    string mesh = HERO_MESH_FILE_WITHOUT_EXTENSION;

	if ( type == ENEMY )
    {
        mesh = ENEMY_MESH_FILE_WITHOUT_EXTENSION;
	} else if (type == HOSTAGE) {
		mesh = HOSTAGE_MESH_FILE_WITHOUT_EXTENSION;
	}

    Utilities::getSingleton().put_character_in_scene ( sceneMgr,
                                                  world,
                                                  mesh,
                                                  name,
                                                  v_pos.x,
                                                  v_pos.y,
                                                  v_pos.z,
                                                  &_entity,
                                                  &_node,
                                                  &_rigidBody,
                                                  true,
                                                  animation);
    if (animation == "") {
        _currentAnimation = NULL;
    } else {
        _currentAnimation = _entity->getAnimationState(animation);
    }

	// Creamos el Dummy para la vision y para el mini mapa
	_entityDummy = sceneMgr->createEntity ( name + "DUMMY", MESH_FILE_WITHOUT_EXTENSION + string ( ".mesh" ) );
    _entityDummy->setCastShadows(false);
    _entityDummy->setVisible(false);

    _nodeDummy = sceneMgr->createSceneNode ( name + "DUMMY" );
	_nodeDummy->setPosition(0, (_entityDummy->getBoundingBox().getSize().y / -2), 0);
    _nodeDummy->attachObject ( _entityDummy );
	// Atachamos el dummy para que se mueva con el personaje
    _node->addChild ( _nodeDummy );
  }

Character::~Character()
  {
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
    _health = source.getHealth();
    _v_pos = source.getInitial_Pos();
    _node = source.getSceneNode();
    _rigidBody = source.getRigidBody();
    _currentAnimation = source._currentAnimation;
  }

void Character::setHealth ( float newHealth )
  {
    if ( _health <= 100 )
    {
      if ( newHealth > 100 )
        _health = 100;
      else if ( newHealth < 0 )
        _health = 0;
      else
        _health = newHealth;
    }
  }

void Character::changeAnimation(string nameAnimation) {
    if (_currentAnimation != NULL &&
        _currentAnimation->getAnimationName() != nameAnimation) {
        Ogre::AnimationState *animation;
        if (STOP_ANIMATION == nameAnimation) {
            animation = (_entity)->getAnimationState(MOVE_ANIMATION);
            animation->setEnabled(false);
        } else if (MOVE_ANIMATION == nameAnimation) {
            animation = (_entity)->getAnimationState(STOP_ANIMATION);
            animation->setEnabled(false);
        }
        animation = (_entity)->getAnimationState(nameAnimation);
        animation->setEnabled(true);
        animation->setLoop(true);
        _currentAnimation = animation;
    }
}

void Character::update(double timeSinceLastFrame) {
    if (_currentAnimation != NULL) {
        _currentAnimation->addTime(timeSinceLastFrame * VELOCIDAD_ANIMACION);
    }
}

void Character::walk ( bool reverse, float velocidad )
  {
    assert ( _rigidBody );

//    float velocidad = VELOCIDAD;
    if (reverse) velocidad *= -1;

    _rigidBody->enableActiveState();
    Ogre::Vector3 orientacion = _rigidBody->getCenterOfMassOrientation() * Ogre::Vector3::UNIT_Z;
    orientacion.y = 0.0; // Movemos solo en los ejes Z y X
    _rigidBody->setLinearVelocity(orientacion * velocidad);

    changeAnimation(MOVE_ANIMATION);

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

    _rigidBody->enableActiveState();
    _node->yaw ( Ogre::Radian(Ogre::Math::HALF_PI / 32) );
    btQuaternion quaternion = OgreBulletCollisions::OgreBtConverter::to(_node->getOrientation());
    _rigidBody->getBulletRigidBody()->getWorldTransform().setRotation(quaternion);

  }

void Character::turn_right()
  {

    assert ( _node );

    _rigidBody->enableActiveState();
    _node->yaw ( Ogre::Radian((-1)* Ogre::Math::HALF_PI / 32) );
    btQuaternion quaternion = OgreBulletCollisions::OgreBtConverter::to(_node->getOrientation());
    _rigidBody->getBulletRigidBody()->getWorldTransform().setRotation(quaternion);

  }

void Character::showDummy(bool show) {
	this->getEntity()->setVisible(!show);
	this->getEntityDummy()->setVisible(show);
}

void Character::print()
  {
    cout << "name              : " << _name << endl;
    cout << "health            : " << _health << endl;
    printf ( "node ref.         : %p\n", _node );
    printf ( "body ref.         : %p\n", _rigidBody );
    cout << "initial position  : " << _v_pos << endl;
  }
