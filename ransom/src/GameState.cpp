#include "GameState.hpp"

#include <cassert>

using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

using namespace Ogre;

GameState::GameState()
  {
    clear();
  }

void GameState::clear() {
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
    m_pSceneMgr         = NULL;
    m_hero              = NULL;
    m_pOverlayMgr       = NULL;
    m_enemies.clear();
    m_hostages.clear();
	_vCharacteres.clear();
    _world              = NULL;
    _camerasController  = NULL;
    _vFader.clear();
    _tiempo             = 0;
    _hostages           = 1;
	_rtex = NULL;
	_textureListener = NULL;
	_cameraMiniMap = NULL;
	_staticGeometry = NULL;
  }

void GameState::enter()
  {
	clear();

    _faderGameOver = NULL;
    _faderFinish = NULL;

    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Entering GameState...");

    m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

    // Overlays con fundidos de negro al color del overlay
    Fader* fader = NULL;

    fader = new Fader ( "GUI_Game", "Game/Recuadro", NULL );
    _vFader.push_back ( fader );

    fader = new Fader ( "GUI_Game", "Game/Face", NULL );
    _vFader.push_back ( fader );

    fader = new Fader ( "GUI_Game", "Game/Life4", NULL );
    _vFader.push_back ( fader );

    fader = new Fader ( "GUI_Game", "Game/Time", NULL );
    _vFader.push_back ( fader );

    fader = new Fader ( "GUI_Game", "Game/Hostages", NULL );
    _vFader.push_back ( fader );

    fader = new Fader ( "GUI_Game", "Game/Recuadro_MiniMapa", NULL );
    _vFader.push_back ( fader );

    _soundGameOver = SoundFXManager::getSingleton().load("gameover.wav");
    _soundFinish = SoundFXManager::getSingleton().load("finish.wav");

    // OIS::ParamList param;
    // size_t windowHandle;
    // std::ostringstream wHandleStr;

    // _tiempo = 0;
    // _mejorTiempo = 0;
    // _empieza_a_contar = true;
    // _estaEnMeta         = false;
    // _controlMeta = 0;

    _gameTrack = TrackManager::getSingleton().load("ingame.mp3");

    // // Reproducción del track principal...
    _gameTrack->play();

    m_pSceneMgr = OgreFramework::getSingletonPtr()->getRootPtr()->createSceneManager(ST_GENERIC, "GameSceneMgr");
//    m_pSceneMgr->setAmbientLight ( Ogre::ColourValue ( 0.9f, 0.9f, 0.9f ) );

    CreateCameras();

    // // Creacion del modulo de debug visual de Bullet ------------------
    _debugDrawer = new OgreBulletCollisions::DebugDrawer();
    _debugDrawer->setDrawWireframe(true);
    SceneNode *node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode ( "debugNode", Vector3::ZERO );
    node->attachObject(static_cast <SimpleRenderable *>(_debugDrawer));

    // Creacion del mundo (definicion de los limites y la gravedad) ---
    AxisAlignedBox worldBounds = AxisAlignedBox (
    Vector3 (-10000, -10000, -10000),
    Vector3 (10000,  10000,  10000));
    Vector3 gravity = Vector3(0, -9.8, 0);

    _world = new OgreBulletDynamics::DynamicsWorld ( m_pSceneMgr,
     	   worldBounds, gravity);
    _world->setDebugDrawer (_debugDrawer);

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->hideCursor();

    // Mostramos el loading a pantalla completa
    Ogre::OverlayElement *elem;
    elem = m_pOverlayMgr->getOverlayElement("Panel_Loading_Game");
    elem->setDimensions(OgreFramework::getSingletonPtr()->_factorX, OgreFramework::getSingletonPtr()->_factorY);

    elem = m_pOverlayMgr->getOverlayElement("Panel_Game_Over");
    elem->setDimensions(OgreFramework::getSingletonPtr()->_factorX, OgreFramework::getSingletonPtr()->_factorY);

    elem = m_pOverlayMgr->getOverlayElement("Panel_Finish");
    elem->setDimensions(OgreFramework::getSingletonPtr()->_factorX, OgreFramework::getSingletonPtr()->_factorY);

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_GameOver", false );
    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_Finish", false );

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "Loading_Game", true );

    OgreFramework::getSingletonPtr()->getRootPtr()->renderOneFrame();

	// Cargamos el fichero de configuracion
    XMLCharger::getSingleton().LoadGameConfig ( FILE_ROUTE_XML, _gc );

    _gc.print();

    // Creacion de los elementos iniciales del mundo
    CreateInitialWorld();

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "Loading_Game", false );

    for ( unsigned int i = 0; i < _vFader.size(); i++ )
      if (_vFader[i]) _vFader[i]->startFadeOut(4.0);

  }

void GameState::CreateInitialWorld()
  {
    // Activamos las sombras
    m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    m_pSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );

    m_pSceneMgr->setShadowTextureCount(2);
    m_pSceneMgr->setShadowTextureSize(512);

	// Generamos la configuracion de del Mapa aleatorio
	_gc.createMapRandom();
    // Crear el mapa
    CreateMap();

    // Creamos al Heroe
    Ogre::Vector3 v_pos;
    v_pos = _gc.getInitialPosHero();
    m_hero = new Hero ( m_pSceneMgr, _world, "Hero", v_pos );
    _vCharacteres.push_back(m_hero);

    // Creamos los enemigos
    Enemy *enemy = NULL;
    string name_enemy = "";
    EnemyRoute route;
    Ogre::Vector3 v;

    for ( unsigned int i = 0; i < _gc.getNumEnemies(); i++ )
      {
        route = _gc.getEnemyRoute ( i );

        assert ( route.getNumPoints() != 0 );

        v = route.getPoint ( 0 );

        name_enemy = "Enemy" + StringConverter::toString(i);

        enemy = new Enemy ( m_pSceneMgr, _world, name_enemy, v, route, m_hero );
        m_enemies.push_back ( enemy );
        _vCharacteres.push_back(enemy);
      }

    _hostages = _gc.getNumHostages();

    for ( unsigned int i = 0; i < _gc.getNumHostages(); i++ )
      {
        string name_hostage = "Hostage" + StringConverter::toString(i);

        Hostage* hostage = new Hostage ( m_pSceneMgr, _world, name_hostage, _gc.getPositionHostage(i) );
        m_hostages.push_back ( hostage );
        _vCharacteres.push_back ( hostage );
      }

	// Creamos el controlador de las camaras para que sigan al heroe
	_camerasController = new CamerasController(m_pSceneMgr, m_pCamera, _cameraMiniMap, m_hero);

    // Creamos el MiniMapa
    //*************************************************************************
    CreateMiniMap();

	// Creamos las luz del escenario
    Light* luz = m_pSceneMgr->createLight("Luz");
    luz->setType(Light::LT_POINT);
    luz->setPosition(50,100,50);
    luz->setSpecularColour(1, 1, 1);
    luz->setDiffuseColour(1, 1, 1);
}

void GameState::CreateCameras() {
	m_pCamera = m_pSceneMgr->createCamera ( "GameCamera" );
    m_pCamera->setNearClipDistance(1);
    OgreFramework::getSingletonPtr()->getViewportPtr()->setCamera ( m_pCamera );
	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->getViewportPtr()->getActualWidth()) /
                            Ogre::Real(OgreFramework::getSingletonPtr()->getViewportPtr()->getActualHeight()));

	// Creamos la camara del mini mapa
	_cameraMiniMap = m_pSceneMgr->createCamera ( "CameraMiniMap" );
}

void GameState::CreateMiniMap() {
    // Creamos la textura donde vamos a meter el mapa que va visualizarse a partir de la camara
    Ogre::TexturePtr _rtt = Ogre::TextureManager::getSingleton().createManual (
            NAME_TEXTUTE_MINIMAP, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D, 256, 256, 0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET );

    _rtex = _rtt->getBuffer()->getRenderTarget();
	  // Vinculamos la camara con la textura
    _rtex->addViewport ( _cameraMiniMap );
    _rtex->getViewport(0)->setClearEveryFrame ( true );
    _rtex->getViewport(0)->setBackgroundColour ( Ogre::ColourValue::Black );
    _rtex->getViewport(0)->setOverlaysEnabled ( false );
    _rtex->setAutoUpdated(true);
    // Creamos el material al que le vamos a asginar la textura
    MaterialPtr mPtr = MaterialManager::getSingleton().create ( NAME_MATERIAL_MINIMAP, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
    Technique* matTechnique = mPtr->createTechnique();
    matTechnique->createPass();
    mPtr->getTechnique(0)->getPass(0)->setLightingEnabled(true);
    mPtr->getTechnique(0)->getPass(0)->setDiffuse(0.9,0.9,0.9,1);
    mPtr->getTechnique(0)->getPass(0)->setSelfIllumination(0.4,0.4,0.4);
    // Le asignamos la textura que hemos creado
    mPtr->getTechnique(0)->getPass(0)->createTextureUnitState("RttT_Map");

	  // Le vinculamos el listener a la textura
  	_textureListener = new MiniMapTextureListener (m_pSceneMgr ,_vCharacteres );
    _rtex->addListener ( _textureListener );

     Ogre::OverlayElement *elem;

     elem = m_pOverlayMgr->getOverlayElement("Panel_Mini_Map_Game");
     elem->setMaterialName ( "RttMat_Map" );

  }


bool GameState::pause()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Pausing GameState...");

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_Game", false );

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_GameOver", false );

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_Finish", false );

    // Mostrar_Velocidad ( 0, true );

    // Ogre::OverlayElement *elem;

    // elem = m_pOverlayMgr->getOverlayElement("Panel_Tiempo_Game");
    // elem->hide();

    // elem = m_pOverlayMgr->getOverlayElement("Panel_MejorTiempo_Game");
    // elem->hide();

    // _gameTrack->pause();

    return true;
  }

void GameState::resume()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Resuming GameState...");

    OgreFramework::getSingletonPtr()->getViewportPtr()->setCamera(m_pCamera);
    m_bQuit = false;

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_Game", true );

    // Mostrar_Velocidad ( 0 );

//    Ogre::OverlayElement *elem = NULL;

    // elem = m_pOverlayMgr->getOverlayElement("Panel_Tiempo_Game");
    // elem->show();

    // elem = m_pOverlayMgr->getOverlayElement("Panel_MejorTiempo_Game");
    // elem->show();

//    elem = m_pOverlayMgr->getOverlayElement ( "cursor" );
//    elem->hide();

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->hideCursor();

    _gameTrack->play();

  }

void GameState::exit()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Leaving GameState...");

    for ( unsigned int i = 0; i < _vFader.size(); i++ )
      if (_vFader[i]) delete _vFader[i];

    _vFader.clear();

    if ( _faderGameOver )
      delete _faderGameOver;

    if ( _faderFinish )
      delete _faderFinish;

    // // Parar del track principal...
//     _gameTrack->play();
     _gameTrack->stop();

    // Erase the enemies deque
    std::deque<Enemy *>::iterator itEnemy = m_enemies.begin();
 		while ( m_enemies.end() != itEnemy )
 		{
      if ( *itEnemy )
        delete *itEnemy;
 			++itEnemy;
 		}

 		m_enemies.clear();

 		if ( m_hero )
 		{
 		  cout << "delete hero" << endl;
      delete m_hero;
 		}

	std::vector<Hostage *>::iterator itHostage = m_hostages.begin();
 		while ( m_hostages.end() != itHostage )
 		{
      if ( *itHostage )
        delete *itHostage;
 			++itHostage;
 		}

 		m_hostages.clear();

	_vCharacteres.clear();

    if ( m_pCamera )
    {
      cout << "delete camera" << endl;
      m_pSceneMgr->destroyCamera ( m_pCamera );
    }

    if ( _textureListener )
      {
        _rtex->removeListener ( _textureListener );
        delete _textureListener;
      }

	  if ( _cameraMiniMap )
    {
      cout << "delete camera" << endl;
      m_pSceneMgr->destroyCamera ( _cameraMiniMap );
    }

	Ogre::TextureManager::getSingleton().remove(NAME_TEXTUTE_MINIMAP);

	Ogre::MaterialManager::getSingleton().remove(NAME_MATERIAL_MINIMAP);
    if ( _world )
    {
      cout << "delete world" << endl;
      delete _world;
    }

	if (_staticGeometry) {
		m_pSceneMgr->destroyStaticGeometry(_staticGeometry);
	}

    if ( _debugDrawer )
    {
      cout << "delete debugdrawer" << endl;
      delete _debugDrawer;
    }


    if ( m_pSceneMgr )
    {
      cout << "delete scene manager" << endl;
      OgreFramework::getSingletonPtr()->getRootPtr()->destroySceneManager ( m_pSceneMgr );
    }

    // Ocultar overlays
//    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
//    overlay->hide();

    _gc.clear();

  }

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
  {
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_ESCAPE ) )
    {
      pushAppState(findByName("PauseState"));
      return true;
    }
    else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_D ) )
		_world->setShowDebugShapes (true);
    else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_H ) )
      _world->setShowDebugShapes (false);
    else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_SPACE ) )
      {
        m_hero->shoot();
      }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
  }

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
  {
    OgreFramework::getSingletonPtr()->keyPressed ( keyEventRef );

    if (keyEventRef.key == OIS::KC_UP || keyEventRef.key == OIS::KC_DOWN || keyEventRef.key == OIS::KC_X ) {
        m_hero->stop_move();
    }

    return true;
  }

bool GameState::mouseMoved(const OIS::MouseEvent &evt)
  {
    if ( OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->injectMouseMove ( evt ) ) return true;

    return true;
  }

bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
  {
    if ( OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->injectMouseDown ( evt, id ) )
      return true;

    if(id == OIS::MB_Left)
      {
        onLeftPressed(evt);
        m_bLMouseDown = true;
      }
    else if(id == OIS::MB_Right)
      {
        m_bRMouseDown = true;
      }

    return true;
  }

bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
  {
    if ( OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->injectMouseUp ( evt, id ) ) return true;

    if(id == OIS::MB_Left)
      {
        m_bLMouseDown = false;
      }
    else if(id == OIS::MB_Right)
      {
        m_bRMouseDown = false;
      }

    return true;
  }

void GameState::onLeftPressed(const OIS::MouseEvent &evt)
  {
  }

void GameState::update(double timeSinceLastFrame)
  {
    _tiempo += timeSinceLastFrame;

    // if ( _empieza_a_contar )
    //   _tiempo += timeSinceLastFrame;
    m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->getViewportPtr()->getActualWidth()) /
                            Ogre::Real(OgreFramework::getSingletonPtr()->getViewportPtr()->getActualHeight()));

    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->frameRenderingQueued ( m_FrameEvent );

    // Fade in/out
    for ( unsigned int i = 0; i < _vFader.size(); i++ )
      if (_vFader[i]) _vFader[i]->fade ( timeSinceLastFrame );

    Ogre::OverlayElement *elem = NULL;
    elem = m_pOverlayMgr->getOverlayElement("txtTiempo");
    elem->setCaption ( getTime(_tiempo) );

    elem = m_pOverlayMgr->getOverlayElement("txtHostages");
    elem->setCaption ( Ogre::StringConverter::toString(_hostages) );

    updatePanelLife();

    if ( m_hero->getHealth() == 0 )
      {
        _gameTrack->stop();

        if ( !_faderGameOver )
          {
            if ( ( _gc.getNumHostages() - _hostages ) > 0 )
              {
                Records::getSingleton().add ( _gc.getNumHostages() - _hostages, _tiempo );
                Records::getSingleton().compacta ( 10 );
                Records::getSingleton().write();
              }

            Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_Game", false );

            _faderGameOver = new Fader ( "GUI_GameOver", "Game/GameOver", this );
            _faderGameOver->startFadeOut ( 2.0 );
          }

        if ( _faderGameOver )
          _faderGameOver->fade ( timeSinceLastFrame );
      }

    if ( _hostages == 0 )
      {
        _gameTrack->stop();

        if ( !_faderFinish )
          {
            Records::getSingleton().add ( _gc.getNumHostages(), _tiempo );
            Records::getSingleton().compacta ( 10 );
            Records::getSingleton().write();

            Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_Game", false );

            _faderFinish = new Fader ( "GUI_Finish", "Game/Finish", this );
            _faderFinish->startFadeOut ( 2.0 );
          }

        if ( _faderFinish )
          _faderFinish->fade ( timeSinceLastFrame );
      }

    // if(m_bQuit == true)
    //   {
    //     popAppState();
    //     return;
    //   }

    // Ogre::OverlayElement *elem = NULL;
    // elem = m_pOverlayMgr->getOverlayElement("txtTiempo");
    // elem->setCaption ( getTime(_tiempo) );

    // elem = m_pOverlayMgr->getOverlayElement("txtMejorTiempo");
    // elem->setCaption ( "Best Time:\n  " + getTime(_mejorTiempo) );
    // elem = m_pOverlayMgr->getOverlayElement("Panel_MejorTiempo_Game");
    // elem->show();

    // //    bool mbleft, mbmiddle, mbright; // Botones del raton pulsados

    _world->stepSimulation(timeSinceLastFrame); // Actualizar simulacion Bullet

//    bool bMove = false;
//    float valX = 0, valZ = 0;
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_RIGHT ) )
      {
        m_hero->turn_right();
      }
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_LEFT ) )
      {
        m_hero->turn_left();
      }
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_DOWN ) )
      {
        m_hero->walk ( true, 4.0 );
      }
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_UP ) )
      {
        m_hero->walk ( false, 4.0 );
      }

    // Actalizamos los personajes
    m_hero->update(timeSinceLastFrame, _vCharacteres);
    if ( m_enemies.size() > 0 )
    {
        unsigned int i = 1;
        for ( std::deque<Enemy *>::iterator itEnemy = m_enemies.begin(); m_enemies.end() != itEnemy; itEnemy++, i++ )
        {
			(*itEnemy)->update(timeSinceLastFrame, _vCharacteres);
        }
    }
	if ( m_hostages.size() > 0 )
    {
        unsigned int i = 1;
        for ( std::vector<Hostage *>::iterator itHostage = m_hostages.begin(); m_hostages.end() != itHostage; itHostage++, i++ )
        {
            (*itHostage)->update(timeSinceLastFrame, _vCharacteres);
        }
    }

	// Miramos si el heroe a encontrado a algun rehen
	Hostage* hostageRescue = detectCollisionHeroWithHostages(_world, m_hero, m_hostages);
	if (hostageRescue != NULL) {
		hostageRescue->liberate();
		_hostages--;
	}

	// Actualizamos la camara
	if (_camerasController != NULL)
		_camerasController->update(timeSinceLastFrame);

  }

void GameState::CreateMap()
{
	float widthScene = SIZE_PART * _gc.getColsMap();
	float heightScene = SIZE_PART * _gc.getRowsMap();
	float heightWall = 10.0f;

	_staticGeometry = m_pSceneMgr->createStaticGeometry("StaticMap");

	// Creamos las paredes
	Utilities::getSingleton().put_plane_in_scene(m_pSceneMgr, _world, _staticGeometry, "leftWall", "MaterialSuelo", heightScene, heightWall,
													Vector3::UNIT_Y, Vector3(1,0,0), Vector3(widthScene / -2.0f,0,0));
	Utilities::getSingleton().put_plane_in_scene(m_pSceneMgr, _world, _staticGeometry, "rightWall", "MaterialSuelo", heightScene, heightWall,
													Vector3::UNIT_Y, Vector3(-1,0,0), Vector3(widthScene / 2.0f,0,0));
	Utilities::getSingleton().put_plane_in_scene(m_pSceneMgr, _world, _staticGeometry, "upWall", "MaterialSuelo", widthScene, heightWall,
													Vector3::UNIT_Y, Vector3(0,0,1), Vector3(0,0,heightScene / -2.0f));
	Utilities::getSingleton().put_plane_in_scene(m_pSceneMgr, _world, _staticGeometry, "downWall", "MaterialSuelo", widthScene, heightWall,
													Vector3::UNIT_Y, Vector3(0,0,-1), Vector3(0,0,heightScene / 2.0f));

	Piece* pieza = NULL;
	string nomPieza = "";
	// Creamos el mapa que habia en la cofig y que hemos generado
	for ( unsigned int i = 0; i < _gc.getNumPiecesMap(); i++ )
	{
		nomPieza = "Pieza" + StringConverter::toString ( i+1 );

		pieza = _gc.getPieceMap(i).pPiece;

		Utilities::getSingleton().put_part_map_in_scene
					( m_pSceneMgr,
						_world,
						_staticGeometry,
						nomPieza,
						pieza->getNameMeshFile(),
						_gc.getPieceMap(i).pos );
	}

	_staticGeometry->build();
}


Hostage* GameState::detectCollisionHeroWithHostages(OgreBulletDynamics::DynamicsWorld* world,
												Hero* hero,
												std::vector<Hostage*> hostages) {
	bool isCollition = false;
	Hostage* hostageCollisition = NULL;
	Hostage* hostage = NULL;
	btCollisionWorld *bulletWorld = world->getBulletCollisionWorld();
	int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();
	int numHostages = hostages.size();
	btPersistentManifold* contactManifold = NULL;
	btCollisionObject* obA;
	btCollisionObject* obB;

	// Recorremos las colisiones que se esten produciendo
	for (int i=0;i<numManifolds && !isCollition;i++) {
		contactManifold = bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
		obA = (btCollisionObject*)(contactManifold->getBody0());
		obB = (btCollisionObject*)(contactManifold->getBody1());

		OgreBulletCollisions::Object* obOB_A = world->findObject(obA);
		OgreBulletCollisions::Object* obOB_B = world->findObject(obB);

		if (obOB_A == hero->getRigidBody() || obOB_B == hero->getRigidBody()) {
			for (int j=0;j<numHostages && !isCollition;j++) {
				hostage = hostages[j];
				if (hostage->getState() == CAPTURE) {
					isCollition = (obOB_A == hostage->getRigidBody() || obOB_B == hostage->getRigidBody());
					if (isCollition) hostageCollisition = hostage;
				}
			}
		}
	}
	return hostageCollisition;
}

string GameState::getTime ( double tiempo )
  {
    unsigned int minutos = 0, segundos = 0;
    char cad[6];
    string ret = "";

    minutos = (int)tiempo / 60;
    segundos = (int)tiempo % 60;

    sprintf ( cad, "%02d:%02d", minutos, segundos );

    ret = cad;

    return ret;
  }

void GameState::updatePanelLife()
  {
     Ogre::OverlayElement *elem = NULL;

     elem = m_pOverlayMgr->getOverlayElement("Panel_Life_Hero_Game");

     if ( m_hero->getHealth() > 75 )
       elem->setMaterialName ( "Game/Life4" );
     else if ( m_hero->getHealth() > 50 && m_hero->getHealth() <= 75 )
       elem->setMaterialName ( "Game/Life3" );
     else if ( m_hero->getHealth() > 25 && m_hero->getHealth() <= 50 )
       elem->setMaterialName ( "Game/Life2" );
     else
       elem->setMaterialName ( "Game/Life1" );
  }

void GameState::fadeOutCallback(void)
  {
    if ( _hostages == 0 )
      _soundFinish->play();
    else
      _soundGameOver->play();
  }
