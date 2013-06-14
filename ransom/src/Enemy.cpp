#include "Enemy.h"

Enemy::Enemy( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    const Ogre::Vector3& v_pos,
                    const GameConfig& config,
                    unsigned int id_route ) : Character ( sceneMgr,
                                                        world,
                                                        name,
                                                        v_pos,
                                                        true )
  {
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

    // Maquina de Estados del Enemigo
    if ( _sm.load_from_file ( "config/stateMachineConfig.xml" ) )
      _sm.print_info();

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
	#ifndef _WIN32
		_rtt = source.getTexturePtr();
	#endif
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

void Enemy::update(double timeSinceLastFrame) {
    Character::update(timeSinceLastFrame);
    updateLifeBar();

    //Cambios de estado del enemigo
    if ( haveYouSeenAnybody() )
      {
        if ( _sm.getCurrentState() == "Watching" )
          _sm.setCurrentState ( "Alert" );
      }
    else
      {
        if ( _sm.getCurrentState() == "Alert" )
          _sm.setCurrentState ( "Watching" );
      }
}

bool Enemy::walk_to ( const Ogre::Vector3& p )
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

    walk ( false );

    return res;
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
