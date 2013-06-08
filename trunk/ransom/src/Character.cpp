#include "Character.h"
#include <cassert>

Character::Character ( Ogre::SceneManager* sceneMgr,
                        OgreBulletDynamics::DynamicsWorld* world,
                        const string& name,
                        float initial_pos_X,
                        float initial_pos_Y,
                        float initial_pos_Z ) : _name(name),
                                                _sceneMgr(sceneMgr),
                                                _posX(initial_pos_X),
                                                _posY(initial_pos_Y),
                                                _posZ(initial_pos_Z)
  {
    _mutex_turn = PTHREAD_MUTEX_INITIALIZER;
    _mutex_walk = PTHREAD_MUTEX_INITIALIZER;

    _node = NULL;
    _rigidBody = NULL;

    _health = 100.0;

    Utilities::getSingleton().put_cube_in_scene ( sceneMgr,
                                                  world,
                                                  MESH_FILE_WITHOUT_EXTENSION,
                                                  name,
                                                  initial_pos_X,
                                                  initial_pos_Y,
                                                  initial_pos_Z,
                                                  &_entity,
                                                  &_node,
                                                  &_rigidBody,
                                                  true );

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
    _posX = source.getInitial_PosX();
    _posY = source.getInitial_PosY();
    _posZ = source.getInitial_PosZ();
    _node = source.getSceneNode();
    _rigidBody = source.getRigidBody();
  }

void Character::setHealth ( float newHealth )
  {
    if ( _health < 100 )
    {
      if ( newHealth > 100 )
        _health = 100;
      else
        _health = newHealth;
    }
  }

void Character::walk ( bool reverse )
  {
    pthread_mutex_lock( &_mutex_walk );

    assert ( _rigidBody );

    float velocidad = VELOCIDAD;
    if (reverse) velocidad *= -1;

    _rigidBody->enableActiveState();
    Ogre::Vector3 orientacion = _rigidBody->getCenterOfMassOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
    orientacion.y = 0.0; // Movemos solo en los ejes Z y X
    _rigidBody->setLinearVelocity(orientacion * velocidad);

    pthread_mutex_unlock( &_mutex_walk );
  }

void Character::walk_to ( const Ogre::Vector3& p )
  {
    Ogre::Vector3 o = _rigidBody->getSceneNode()->getPosition();

    Ogre::Vector3 v = p - o; // 1er vector

    Ogre::Vector3 orientacion = _rigidBody->getCenterOfMassOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z; // 2do vector

    Ogre::Radian angle = orientacion.angleBetween ( v );
    Ogre::Real distance = o.distance ( p );

    if ( angle.valueRadians() > 0.05 )
      turn ( angle.valueRadians() );

    walk();
  }

void Character::stop_move()
  {
    _rigidBody->setLinearVelocity(Ogre::Vector3(0.0, 0.0, 0.0));
  }

void Character::turn ( Ogre::Real angle )
  {
    pthread_mutex_lock( &_mutex_turn );

    assert ( _node );

    _rigidBody->enableActiveState();
    _node->yaw ( Ogre::Radian(angle) );
    btQuaternion quaternion = OgreBulletCollisions::OgreBtConverter::to(_node->getOrientation());
    _rigidBody->getBulletRigidBody()->getWorldTransform().setRotation(quaternion);

    pthread_mutex_unlock( &_mutex_turn );
  }

void Character::print()
  {
    cout << "name           : " << _name << endl;
    cout << "health         : " << _health << endl;
    printf ( "node ref.      : %p\n", _node );
    printf ( "body ref.      : %p\n", _rigidBody );
    cout << "initial position" << endl;
    cout << "X              : " << _posX << endl;
    cout << "Y              : " << _posY << endl;
    cout << "Z              : " << _posZ << endl;
  }
