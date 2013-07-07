#include "Enemy.h"

static bool forcedIN = false;

Enemy::Enemy( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    const Ogre::Vector3& v_pos,
                    const EnemyRoute &route,
                    Hero* ptrHero ) : Character ( sceneMgr,
                                                        world,
                                                        name,
                                                        v_pos,
                                                        ENEMY, STOP_ANIMATION )
  {

    _lastHealth = 0;

    _timeElapsed_Global = 0;
    _timeElapsed_Watching = 0;
    _timeFirstVision = 0;
    _timeStartChasing = 0;
    _timeElapsed_Shooting = 2;
    _timeBlocked = 0;
	_stopAround = true;
	_aroundNumber = 0;
	_timeSeach = 0;

    _currentPosition = Ogre::Vector3::ZERO;

    _sentinel_dest = false;

    _refHero = ptrHero;

    //Material del enemigo
    _entity->setMaterialName ( "MaterialRojo" );

    _entityDummy->setMaterialName ( "MaterialRojo" );

	_particleDeath->setMaterialName("MaterialRojo");

    // Textura para mostrar la visualizacion de lo que ve el enemigo
    _rtt = Ogre::TextureManager::getSingleton().createManual (
            NAME_TEXTUTE_CAM + name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D, 64, 64, 0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET );

    _rtex = _rtt->getBuffer()->getRenderTarget();
    // Camara de lo que visualiza el enemigo
    _camPOV = sceneMgr->createCamera ( "cameraPOV_" + name );
    _camPOV->setPosition ( Ogre::Vector3 ( 0, 0, 0.2 ) );
    _camPOV->lookAt ( Ogre::Vector3 ( 0, 0, 5 ) );
    _camPOV->setNearClipDistance ( 0.1 );
    _camPOV->setFOVy ( Ogre::Degree ( 38 ) );
    Ogre::SceneNode *nodeCamera = _node->createChildSceneNode ( "nodeCameraPOV_" + name );
	  nodeCamera->attachObject(_camPOV);
    // Vinculamos la textura con la camara del enemigo
    _rtex->addViewport ( _camPOV );
    _rtex->getViewport(0)->setClearEveryFrame ( true );
    _rtex->getViewport(0)->setBackgroundColour ( Ogre::ColourValue::Black );
    _rtex->getViewport(0)->setOverlaysEnabled ( false );
    _rtex->setAutoUpdated(true);

    // Creamos la barra de energia
    _bbSetLife = _sceneMgr->createBillboardSet("lifeBarBillboardSet_" + name, 1);
    _bbSetLife->setMaterialName("lifeBar");
    _bbSetLife->setBillboardType(BBT_POINT);
    _lifeBar = _bbSetLife->createBillboard(Ogre::Vector3(0,
                                           _rigidBody->getCenterOfMassPosition().y +
                                                         (_entity->getBoundingBox().getSize().y / 2)
                                            , 0));
    _lifeBar->setDimensions(SIZE_LIFE_BAR, 0.2);
    _lifeBar->setTexcoordRect(0.0f, 0.0f, 0.50f, 1.0);
    _lifeNode = _node->createChildSceneNode("lifeBar_" + name);
    _lifeNode->attachObject(_bbSetLife);

    // Para poner al personaje principal en material blanco y poder verlo el enemigo
    _textureListener = new MyTextureListener ( _sceneMgr, _rtt );
    _rtex->addListener ( _textureListener );

    _current_point = 1;
    _way = true;

    // Cargamos la ruta asociada al enemigo por donde se va a mover

    _route = route;

    cout << " punto de salida = " << _route.getPoint(0) << endl;

    _currentState = WATCHING;

    _soundAlert1FX = SoundFXManager::getSingleton().load("alert1.wav");
    _soundAlert2FX = SoundFXManager::getSingleton().load("alert2.wav");

    _currentSoundAlert = 1;
  }

Enemy::~Enemy()
  {
    if ( _textureListener )
      {
        _rtex->removeListener ( _textureListener );
        delete _textureListener;
      }

    if ( _camPOV )
      _sceneMgr->destroyCamera ( _camPOV );
	Ogre::MaterialManager::getSingleton().remove(NAME_TEXTUTE_CAM + _name);

    // Destruimos el billboardset
    _lifeNode->detachObject(_bbSetLife);
    _sceneMgr->destroyBillboardSet(_bbSetLife);
    _lifeNode->getParent()->removeChild(_lifeNode);
    _sceneMgr->destroySceneNode(_lifeNode);

	_vReturnsPoints.clear();
  }

Enemy::Enemy(const Enemy& other) : Character ( other )
  {
    copy ( other );
  }

Enemy& Enemy::operator=(const Enemy& rhs)
  {
    if (this == &rhs) return *this; // handle self assignment

    copy ( rhs );

    return *this;
  }

void Enemy::copy ( const Enemy& source )
  {
    _rtex = source.getRenderTexture();
//	#ifndef _WIN32
		_rtt = ((Enemy&)source).getTexturePtr();
//	#endif
    _camPOV = source.getCameraPOV();
	_stopAround = source._stopAround;
	_aroundNumber = source._aroundNumber;
	_vReturnsPoints = source._vReturnsPoints;
	_timeSeach = source._timeSeach;
  }

void Enemy::print()
{
  cout << "==============" << endl;
  cout << "Enemy Info" << endl;
  cout << "==============" << endl;

  Character::print();

  printf ( "render tex ref.: %p\n", _rtex );
  printf ( "texture ref.   : %p\n", _rtt.get() );
  printf ( "camera ref.    : %p\n", _camPOV );

  _route.print();

  cout << "==============" << endl;
}

bool Enemy::haveYouSeenAnybody()
{
  assert ( _textureListener );
  return _textureListener->enemyViewed();
}

void Enemy::updateLifeBar() {

    if ( _health != _lastHealth )
    {
      if ( _lastHealth != 0 )
        {
          setCurrentState( SHOOTING );
          forcedIN = true;
          _timeElapsed_Shooting = _timeElapsed_Global;
        }
      _lastHealth = _health;

      Ogre::Real ratio = _health / MAX_HEALTH;

      if (ratio < 0.0f)
          ratio = 0.0f;

      _lifeBar->setTexcoordRect((1.0 - ratio) / SIZE_LIFE_BAR,
                                0.0f,
                                0.50f + (1.0 - ratio) / SIZE_LIFE_BAR,
                                1.0);
    }
}

void Enemy::update ( double timeSinceLastFrame, std::vector<Character*>   vCharacteres)
  {
	  Character::update(timeSinceLastFrame, vCharacteres);

    if (_stateCaracter == LIVE)
      {
        _timeElapsed_Global += timeSinceLastFrame;

        updateLifeBar();

        // Cambios de estado del enemigo

        switch ( _currentState )
          {
            // ################## ESTADO WATCHING ##################
            // Si estamos en el estado normal de WATCHING
            case WATCHING:

                // Si avistamos al heroe :
                // - tomamos su posición actual
                // - cogemos el tiempo actual
                // - pasamos al estado ALERT
                if ( haveYouSeenAnybody() )
                  {
                    _positionLastViewed = _refHero->getPosition();
                    _timeFirstVision = _timeElapsed_Global;
                    setCurrentState ( ALERT );
                  }
                // Si no vemos al heroe :
                // - reseteamos el tiempo que lo hemos estado viendo a 0
                // - el centinela usado para saber si hemos llegado al punto donde lo vimos lo ponemos a false
                // - seguimos caminando por nuestra ruta
                else
                  {
              // Si no se ha perdido cuando perseguia al enemigo
              if (_vReturnsPoints.size() == 0) {
              _timeElapsed_Watching = 0;
              _sentinel_dest = false;
              walk_in_route();
              } else {
                // Si se ha perdido returna por los puntos de retorno
                if (walk_to(_vReturnsPoints[_vReturnsPoints.size() -1])) {
                              cout << "QUITANDO PUNTO RETORNO" << endl;
                  _vReturnsPoints.pop_back();
                }
              }
                  }

                break;

            // ################## ESTADO ALERT ##################
            case ALERT:

                // Partiendo de la premisa de que para estar en estado ALERT
                // el enemigo ha tenido que ver al heroe.
                //    - Pues si ahora ya no lo vemos :
                //        # Si el tiempo actual menos el tiempo que cogimos al verlo es mayor
                //          a 2 segundos, entonces pasamos al estado CHASING y almacenamos
                //          el tiempo cuando iniciamos la persecución
                //        # Sino, si el tiempo de vision es inferior a 2 segundos pasaremos
                //          al estado WATCHING
                //    - Si por el contrario, seguimos viendo habiendo cambiado de estado
                //      entonces, nos pasamos al estado shooting y registramos el momento temporal
                //      para el tema de que despues de verlo y pasados 2 segundos dispare.
                if ( !haveYouSeenAnybody() )
                  {
                    // Si hemos estado al menos 2 segundos delante del enemigo pasaremos
                    // al estado persecucion (CHASING)
                    if ( _timeElapsed_Global - _timeFirstVision > 2 )
                      {
                        _timeStartChasing = _timeElapsed_Global;
                        setCurrentState ( CHASING );
                      }
                    // Sino es asi volveremos al estado WATCHING
                    else
                      {
                        setCurrentState ( WATCHING );
                      }
                  }
                else
                  {
                    play_sound_alert();
                    setCurrentState ( SHOOTING );
                    _timeElapsed_Shooting = _timeElapsed_Global;
                  }

                break;

            // ################## ESTADO SHOOTING ##################
            case SHOOTING:

                _timeElapsed_Watching = 0;
                _sentinel_dest = false;

                stop_move();

                if ( haveYouSeenAnybody() || forcedIN )
                  {
                    forcedIN = false;

                    _positionLastViewed = _refHero->getPosition();

                    reorient_enemy_to_hero();

                    if ( _timeElapsed_Global - _timeElapsed_Shooting > 2 )
                    {
                      double distance = 0.0;

                      _timeElapsed_Shooting = _timeElapsed_Global;

                      distance = get_distance_with_hero();

                      shoot(distance);
                    }
                  }
                else
                  {
                    _timeStartChasing = _timeElapsed_Global;
                    setCurrentState ( CHASING );
                  }

                break;

            // ################## ESTADO CHASING ##################
            case CHASING:

                // Si entramos en estado CHASING
                //    - Si no ve al heroe desde el ultimo avistamiento pasaremos al estado WATCHING.
                //    - Si por el contrario, seguimos viendo al enemigo, pasaremos al
                //      estado ALERT
                //    - Por ultimo, si no lo vemos y el tiempo entre que ha desaparecido el heroe
                //      de su vision; el enemigo se movera hasta el punto ultimo visto del heroe y llegado
                //      a éste hara una visión alrededor para buscar al heroe.
                if ( !haveYouSeenAnybody() &&
                    _aroundNumber > 4 )
                  {
                    setCurrentState ( WATCHING );
                    _aroundNumber = 0;
                  }
                else if ( haveYouSeenAnybody() )
                  {
                    setCurrentState ( ALERT );
                    _aroundNumber = 0;
                  }
                else
                  {
                    // Si ya estamos en el punto donde vimos al heroe por
                    // ultima vez, entonces dejamos de movernos
                    if ( !_sentinel_dest )
                      {
                        _sentinel_dest = walk_to ( _positionLastViewed, true );
                        // Cada medio segundo añadimos puntos de retorno por si se pierde
                        if ( _timeElapsed_Global - _timeSeach > 0.5 )
                          {
                            _timeSeach = _timeElapsed_Global;
                            _vReturnsPoints.push_back(_node->getPosition());
                            cout << "INSERTADO PUNTO RETORNO" << endl;
                          }

                        _stopAround = true;
                      }
                    else
                      {
                        _timeElapsed_Watching += timeSinceLastFrame;

                        // Hacemos que pare y guiere cada segundo
                        if ( _timeElapsed_Watching >= 1.0 )
                          {
                            _timeElapsed_Watching = 0;
                            _stopAround = !_stopAround;
                            if (!_stopAround) _aroundNumber++;
                          }

                        if (_stopAround)
                          {
                            stop_move();
                          }
                        else
                          {
                            watch_around(timeSinceLastFrame);
                          }
                      }
                  }

                break;

          }
        // ######################################################

        // Cada TIMER_MAX_BLOCKED segundos, comprobaremos si el enemigo
        // se ha quedado bloqueado en algun sitio del escenario

        if ( _timeElapsed_Global - _timeBlocked > TIMER_MAX_BLOCKED )
          {
            _timeBlocked = _timeElapsed_Global;

            if ( ( _currentPosition == _node->getPosition() ) &&
                ( _currentState == CHASING ) )
                //( _currentState == CHASING && !_sentinel_dest) )
              {
                _currentState = WATCHING;
              }

            _currentPosition = _node->getPosition();
          }

    }
  }

void Enemy::play_sound_alert()
{
  if ( _currentSoundAlert == 1 )
    {
      _soundAlert1FX->play();
      _currentSoundAlert = 2;
    }
  else
    {
      _soundAlert2FX->play();
      _currentSoundAlert = 1;
    }

  }

const Ogre::Real& Enemy::get_distance_with_hero()
  {
    Ogre::Vector3 p = _refHero->getSceneNode()->getPosition();
    Ogre::Vector3 o = _node->getPosition();

    Ogre::Real distance = o.distance ( p );

    return distance;
  }

bool Enemy::walk_to ( const Ogre::Vector3& p, bool running )
  {
    bool res = false;
    Ogre::Vector3 o = _rigidBody->getSceneNode()->getPosition();

    Ogre::Vector3 v = p - o; // 1er vector

    Ogre::Vector3 orientacion = _rigidBody->getCenterOfMassOrientation() * Ogre::Vector3::UNIT_Z; // 2do vector

    Ogre::Radian angle = orientacion.angleBetween ( v );

    Ogre::Real distance = o.distance ( p );

    if ( distance < _route.getProximity() )
      res = true;

      if ( orientacion.getRotationTo(v).getYaw().valueDegrees() > 0 )
      {
        if ( orientacion.getRotationTo(v).getYaw().valueDegrees() > 170 )
          {
            turn_left(); turn_left();
          }
        if ( angle.valueDegrees() > 10 )
          {
            turn_left();
          }
      }
      else
      {
        if ( orientacion.getRotationTo(v).getYaw().valueDegrees() < -170 )
          {
            turn_right(); turn_right();
          }
        if ( angle.valueDegrees() > 10 )
          {
            turn_right();
          }
      }

    if ( running )
      walk ( false, VELOCIDAD_RUNNING );
    else
      walk ( false );

    return res;
  }

void Enemy::watch_around(double timeSinceLastFrame)
{
	changeAnimation(MOVE_ANIMATION);
    _rigidBody->enableActiveState();
	_node->yaw ( Ogre::Radian(Ogre::Math::DegreesToRadians(135)) * timeSinceLastFrame);
    btQuaternion quaternion = OgreBulletCollisions::OgreBtConverter::to(_node->getOrientation());
    _rigidBody->getBulletRigidBody()->getWorldTransform().setRotation(quaternion);

}

void Enemy::walk_in_route()
{
  assert ( _route.getNumPoints() );

  Ogre::Vector3 v = _route.getPoint ( _current_point );

  // Si el método walk_to() nos devuelve true, quiere decir que ha llegado al destino
  // y por lo tanto la variable _current_point de la ruta del enemigo debe de pasar al siguiente
  // punto
  if ( walk_to ( v ) )
  {
    cout << _name << ": punto destino = " << v << " - puntos = " << _route.getNumPoints() << " : current_point = " << _current_point << endl;
    if ( _way ) // Sentido hacia adelante
      _current_point++;
    else  // Sentido inverso
      _current_point--;
  }

  // Si esto es así, significa que hemos pasado del último punto
  if ( _current_point == _route.getNumPoints() )
    {
      cout << _name << ": _route.getRouteClosed() " << _route.getRouteClosed() << endl;
      if ( _route.getRouteClosed() )
        _current_point = 0;
      else
      {
        _way = !_way;
        _current_point-=2;
      }
      cout << _name << ": _current_point = " << _current_point << endl;
    }
  else if ( !_way && _current_point == -1 )
    {
      _way = !_way;
      _current_point = 1;
      cout << _name << ": _current_point = " << _current_point << endl;
    }
}

void Enemy::showDummy(bool show) {
	Character::showDummy(show);
	_lifeNode->setVisible(!show);
}

void Enemy::setCurrentState( const eSTATES_ENEMY& newValue )
{
  string msg = "";

  _currentState = newValue;

  switch ( _currentState )
    {
      case WATCHING:
        msg = "WATCHING"; break;
      case ALERT:
        msg = "ALERT"; break;
      case SHOOTING:
        msg = "SHOOTING"; break;
      case CHASING:
        msg = "CHASING"; break;
    }

  cout << "The new state is '" << msg << "'" << endl;
}

void Enemy::reorient_enemy_to_hero()
  {
    Ogre::Vector3 p = _refHero->getSceneNode()->getPosition();

    Ogre::Vector3 o = _rigidBody->getSceneNode()->getPosition();

    Ogre::Vector3 v = p - o; // 1er vector

    Ogre::Vector3 orientacion = _rigidBody->getCenterOfMassOrientation() * Ogre::Vector3::UNIT_Z; // 2do vector

    Ogre::Radian angle = orientacion.angleBetween ( v );

    if ( orientacion.getRotationTo(v).getYaw().valueDegrees() > 0 )
      {
        if ( angle.valueDegrees() > 5 )
          {
            turn_left();
          }
      }
    else
      {
        if ( angle.valueDegrees() > 5 )
          {
            turn_right();
          }
      }
}
