#include "Enemy.h"

Enemy::Enemy( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    const Ogre::Vector3& v_pos,
                    const GameConfig& config,
                    unsigned int id_route,
                    Hero* ptrHero ) : Character ( sceneMgr,
                                                        world,
                                                        name,
                                                        v_pos,
                                                        ENEMY, STOP_ANIMATION )
  {
    // Inicialización del generador de numeros aleatorios
    srand(time(NULL));

    _timeElapsed_Global = 0;
    _timeElapsed_Watching = 0;
    _timeFirstVision = 0;
    _timeStartChasing = 0;
    _timeElapsed_Shooting = 2;
    _timeBlocked = 0;

    _currentPosition = Ogre::Vector3::ZERO;

    _sentinel_dest = false;

    _refHero = ptrHero;

    //Material del enemigo
    _entity->setMaterialName ( "MaterialRojo" );

    _entityDummy->setMaterialName ( "MaterialRojo" );

    // Textura para mostrar la visualizacion de lo que ve el enemigo
    _rtt = Ogre::TextureManager::getSingleton().createManual (
            "RttT_" + name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
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

    try {

      _route = config.getEnemyRoute ( id_route );

    }
    catch ( GameConfigException& exc )
    {
      cerr << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ": EXCEPTION : " << exc.what() << endl;
    }

    print();

    cout << " punto de salida = " << _route.getPoint(0) << endl;

    _currentState = WATCHING;

    _soundAlert1FX = SoundFXManager::getSingleton().load("alert1.wav");
    _soundAlert2FX = SoundFXManager::getSingleton().load("alert2.wav");

    _soundDeath1FX = SoundFXManager::getSingleton().load("death1.wav");
    _soundDeath2FX = SoundFXManager::getSingleton().load("death2.wav");
    _soundDeath3FX = SoundFXManager::getSingleton().load("death3.wav");

    _sonidoShootFX = SoundFXManager::getSingleton().load("shoot.wav");

    _currentSoundAlert = 1;
    _currentSoundDeath = 1;

//    try {
//      // Maquina de Estados del Enemigo
//      if ( _sm.load_from_file ( "config/stateMachineConfig.xml" ) )
//        {
//          assert ( _sm.validateXML() );
//          _sm.print_info();
//        }
//    }
//    catch ( StateMachineException& exc )
//    {
//      cout << exc.what() << endl;
//    }

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

    // Destruimos el billboardset
    _lifeNode->detachObject(_bbSetLife);
    _sceneMgr->destroyBillboardSet(_bbSetLife);
    _lifeNode->getParent()->removeChild(_lifeNode);
    _sceneMgr->destroySceneNode(_lifeNode);
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
    Ogre::Real ratio = _health / MAX_HEALTH;

    if (ratio < 0.0f)
        ratio = 0.0f;

    _lifeBar->setTexcoordRect((1.0 - ratio) / SIZE_LIFE_BAR,
                              0.0f,
                              0.50f + (1.0 - ratio) / SIZE_LIFE_BAR,
                              1.0);
}

void Enemy::update ( double timeSinceLastFrame )
  {
    _timeElapsed_Global += timeSinceLastFrame;

    Character::update(timeSinceLastFrame);
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
                _timeElapsed_Watching = 0;
                _sentinel_dest = false;
                walk_in_route();
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

            if ( haveYouSeenAnybody() )
              {
                _positionLastViewed = _refHero->getPosition();

                reorient_enemy_to_hero();

                if ( _timeElapsed_Global - _timeElapsed_Shooting > 2 )
                {
                  double distance = 0.0;
                  double rate = 0.0;

                  _timeElapsed_Shooting = _timeElapsed_Global;

                  distance = get_distance_with_hero();

                  _sonidoShootFX->play();

                  if ( validate_success_rate ( distance, &rate ) )
                  {
                    cout << "TOCADO!!! (" << rate << "%) distance = " << distance << endl;
                    _refHero->setHealth ( _refHero->getHealth() - 25 );
                  }
                  else
                    cout << "AGUA!!! (" << rate << "%) distance = " << distance << endl;
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
            //    - Si no ve al heroe y ademas han pasado unos 10 segundos desde el
            //      ultimo avistamiento pasaremos al estado WATCHING.
            //      NOTA: Lo de poner 10 segundos es porque el mirar a las 4 esquinas
            //      dura aproximadamente éste tiempo
            //    - Si por el contrario, seguimos viendo al enemigo, pasaremos al
            //      estado ALERT
            //    - Por ultimo, si no lo vemos y el tiempo entre que hemos desaparecido
            //      de su vision y cuando pasamos a éste estado es inferior a 10
            //      el enemigo se movera hasta el punto ultimo visto del heroe y llegado
            //      a éste hara una visión alrededor para buscar al heroe
            if ( !haveYouSeenAnybody() &&
                ( _timeElapsed_Global - _timeStartChasing ) > 10 )
              {
                setCurrentState ( WATCHING );
              }
            else if ( haveYouSeenAnybody() )
              {
                setCurrentState ( ALERT );
              }
            else
              {
                // Si ya estamos en el punto donde vimos al heroe por
                // ultima vez, entonces dejamos de movernos
                if ( !_sentinel_dest )
                  {
                    _sentinel_dest = walk_to ( _positionLastViewed, true );
                  }

                if ( _sentinel_dest )
                  {
                    _timeElapsed_Watching += timeSinceLastFrame;

                    if ( _timeElapsed_Watching >= 2 )
                      {
                        _timeElapsed_Watching = 0;

                        watch_around();
                      }
                    else
                      {
                        stop_move();
                      }
                  }
              }

            break;

      }
    // ######################################################

    // Cada 11 segundos, comprobaremos si el enemigo se ha quedado bloqueado
    // en algun sitio del escenario

    if ( _timeElapsed_Global - _timeBlocked > 11 )
      {
        _timeBlocked = _timeElapsed_Global;

        if ( ( _currentPosition == _node->getPosition() ) &&
            ( ( _currentState == WATCHING ) ||
             ( _currentState == CHASING ) ) )
          {
            turn_right(); turn_right(); turn_right();
          }

        _currentPosition = _node->getPosition();
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

void Enemy::play_sound_death()
{
  if ( _currentSoundDeath == 1 )
    {
      _soundDeath1FX->play();
      _currentSoundDeath++;
    }
  else if ( _currentSoundDeath == 2 )
    {
      _soundDeath2FX->play();
      _currentSoundDeath++;
    }
  else
    {
      _soundDeath3FX->play();
      _currentSoundDeath = 1;
    }

}

bool Enemy::validate_success_rate ( double distance, double* rate )
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

const Ogre::Real& Enemy::get_distance_with_hero()
  {
    Ogre::Vector3 p = _refHero->getSceneNode()->getPosition();
    Ogre::Vector3 o = _rigidBody->getSceneNode()->getPosition();

    Ogre::Real distance = o.distance ( p );

    return distance;
  }

bool Enemy::walk_to ( const Ogre::Vector3& p, bool running )
  {
//    cout << _name << ": destino " << p << endl;
    bool res = false;
    Ogre::Vector3 o = _rigidBody->getSceneNode()->getPosition();

    Ogre::Vector3 v = p - o; // 1er vector

    Ogre::Vector3 orientacion = _rigidBody->getCenterOfMassOrientation() * Ogre::Vector3::UNIT_Z; // 2do vector

    Ogre::Radian angle = orientacion.angleBetween ( v );

    Ogre::Real distance = o.distance ( p );

//    cout << " prueba = " << orientacion.getRotationTo(v).getYaw().valueDegrees() << endl;
//    cout << " angle = " << angle.valueAngleUnits() << endl;
//    cout << _name << ": distance = " << distance << ": proximity = " << _route.getProximity() << endl;

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

void Enemy::watch_around()
{

    _rigidBody->enableActiveState();
    _node->yaw ( Ogre::Radian( Ogre::Math::HALF_PI ) );
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
