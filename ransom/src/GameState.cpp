#include "GameState.hpp"

#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Shapes/OgreBulletCollisionsCompoundShape.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "Debug/OgreBulletCollisionsDebugDrawer.h"

#include "Constraints/OgreBulletDynamicsRaycastVehicle.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include <cassert>

using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

using namespace Ogre;

GameState::GameState()
  {
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
    m_pSceneMgr         = NULL;
    m_hero              = NULL;
    m_pOverlayMgr       = NULL;
    m_enemies.clear();
	m_hostages.clear();
    defaultPlaneBodyFloor = NULL;
    ShapeFloor          = NULL;
    _world              = NULL;
	_camerasController = NULL;
  }

void GameState::enter()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Entering GameState...");

    m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

    // OIS::ParamList param;
    // size_t windowHandle;
    // std::ostringstream wHandleStr;

    // _tiempo = 0;
    // _mejorTiempo = 0;
    // _empieza_a_contar = true;
    // _estaEnMeta         = false;
    // _controlMeta = 0;

    // _gameTrack = TrackManager::getSingleton().load("musicGame.mp3");
    _sonidoShootFX = SoundFXManager::getSingleton().load("shoot.wav");

    // // Reproducción del track principal...
    // _gameTrack->play();

    m_pSceneMgr = OgreFramework::getSingletonPtr()->getRootPtr()->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight ( Ogre::ColourValue ( 0.9f, 0.9f, 0.9f ) );

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
    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "Loading_Game", true );
    OgreFramework::getSingletonPtr()->getRootPtr()->renderOneFrame();

    XMLCharger::getSingleton().LoadGameConfig ( FILE_ROUTE_XML, _gc );
    // TODO XMLCharger::getSingleton().LoadMap ( MAP_ROUTE_XML, _gc );

    _gc.print();

    // Creacion de los elementos iniciales del mundo
    CreateInitialWorld();

//    buildGUI();


    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "Loading_Game", false );

  }

void GameState::CreateInitialWorld()
  {
    // Activamos las sombras
    m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    m_pSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));

    m_pSceneMgr->setShadowTextureCount(2);
    m_pSceneMgr->setShadowTextureSize(512);

    // Crear el mapa
    CreateMap("Mapa1");
//    CreatePlane();

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
        route = _gc.getEnemyRoute ( i+1 );

        assert ( route.getNumPoints() != 0 );

        v = route.getPoint ( 0 );

        name_enemy = "Enemy" + StringConverter::toString(i);

        enemy = new Enemy ( m_pSceneMgr, _world, name_enemy, v, _gc, i+1, m_hero );
        m_enemies.push_back ( enemy );
        _vCharacteres.push_back(enemy);
      }

    for ( unsigned int i = 0; i < _gc.getNumHostages(); i++ )
      {
        string name_hostage = "Hostage" + StringConverter::toString(i);

        Hostage* hostage = new Hostage ( m_pSceneMgr, _world, name_hostage, _gc.getPositionHostage(i) );
        m_hostages.push_back ( hostage );
        _vCharacteres.push_back ( hostage );
      }

	// Creamos el controlador de las camaras para que sigan al heroe
	_camerasController = new CamerasController(m_pSceneMgr, m_pCamera, _cameraMiniMap, m_hero);

    // Creamos el Mapa
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
            "RttT_Map", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D, 256, 256, 0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET );

    _rtex = _rtt->getBuffer()->getRenderTarget();
	  // Vinculamos la camara con la textura
    _rtex->addViewport ( _cameraMiniMap );
    _rtex->getViewport(0)->setClearEveryFrame ( true );
    _rtex->getViewport(0)->setBackgroundColour ( Ogre::ColourValue::Black );
    _rtex->getViewport(0)->setOverlaysEnabled ( false );
    _rtex->setAutoUpdated(true);
    // Creamos el material al que le vamos a asginar la textura
    MaterialPtr mPtr = MaterialManager::getSingleton().create ( "RttMat_Map", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
    Technique* matTechnique = mPtr->createTechnique();
    matTechnique->createPass();
    mPtr->getTechnique(0)->getPass(0)->setLightingEnabled(true);
    mPtr->getTechnique(0)->getPass(0)->setDiffuse(0.9,0.9,0.9,1);
    mPtr->getTechnique(0)->getPass(0)->setSelfIllumination(0.4,0.4,0.4);
    // Le asignamos la textura que hemos creado
    mPtr->getTechnique(0)->getPass(0)->createTextureUnitState("RttT_Map");
    // Dibujamos el rectagulo donde vamos a insertar la camara
    Ogre::Rectangle2D* _rect = new Ogre::Rectangle2D ( true );
    _rect->setCorners ( 0.5, 0.98, 0.98, 0.5 );
    _rect->setMaterial ( "RttMat_Map" );
	// Use infinite AAB to always stay visible
	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	_rect->setBoundingBox(aabInf);
    // Render the background before everything else
    _rect->setRenderQueueGroup ( Ogre::RENDER_QUEUE_BACKGROUND );

    // Attach background to the scene
    Ogre::SceneNode* node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode ( "rectanglePOV_Map" );
    node->attachObject ( _rect );

	  // Le vinculamos el listener a la textura
  	_textureListener = new MiniMapTextureListener (m_pSceneMgr ,_vCharacteres, _rect );
    _rtex->addListener ( _textureListener );
  }


bool GameState::pause()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Pausing GameState...");

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
    // Mostrar_Velocidad ( 0 );

//    Ogre::OverlayElement *elem = NULL;

    // elem = m_pOverlayMgr->getOverlayElement("Panel_Tiempo_Game");
    // elem->show();

    // elem = m_pOverlayMgr->getOverlayElement("Panel_MejorTiempo_Game");
    // elem->show();

//    elem = m_pOverlayMgr->getOverlayElement ( "cursor" );
//    elem->hide();

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->hideCursor();

    // _gameTrack->play();

  }

void GameState::exit()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Leaving GameState...");

    // // Parar del track principal...
    // _gameTrack->play();
    // _gameTrack->stop();

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

    if ( defaultPlaneBodyFloor )
    {
      cout << "delete defaultPlaneBodyFloor" << endl;
      delete defaultPlaneBodyFloor;
    }

    if ( ShapeFloor )
    {
      cout << "delete ShapeFloor" << endl;
      delete ShapeFloor;
    }


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

    if ( _world )
    {
      cout << "delete world" << endl;
      delete _world;
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
      _sonidoShootFX->play();
    else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_K ) )
      {
        if ( m_enemies.size() > 0 && m_enemies[0] )
          m_enemies[0]->play_sound_death();
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
    // if ( _empieza_a_contar )
    //   _tiempo += timeSinceLastFrame;
	m_pCamera->setAspectRatio(Ogre::Real(OgreFramework::getSingletonPtr()->getViewportPtr()->getActualWidth()) /
                            Ogre::Real(OgreFramework::getSingletonPtr()->getViewportPtr()->getActualHeight()));

    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->frameRenderingQueued ( m_FrameEvent );

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
    m_hero->update(timeSinceLastFrame);
    if ( m_enemies.size() > 0 )
    {
        unsigned int i = 1;
        for ( std::deque<Enemy *>::iterator itEnemy = m_enemies.begin(); m_enemies.end() != itEnemy; itEnemy++, i++ )
        {
            (*itEnemy)->update(timeSinceLastFrame);
        }
    }
	if ( m_hostages.size() > 0 )
    {
        unsigned int i = 1;
        for ( std::vector<Hostage *>::iterator itHostage = m_hostages.begin(); m_hostages.end() != itHostage; itHostage++, i++ )
        {
            (*itHostage)->update(timeSinceLastFrame);
        }
    }

	// Miramos si el heroe a encontrado a algun rehen
	Hostage* hostageRescue = detectCollisionHeroWithHostages(_world, m_hero, m_hostages);
	if (hostageRescue != NULL) {
		hostageRescue->liberate();
	}

	// Actualizamos la camara
	if (_camerasController != NULL)
		_camerasController->update(timeSinceLastFrame);

//    if ( bMove && m_hero )
//      {
//        m_hero->walk();
//      }

    // 	    //Si no se tienen pulsadas las teclas DER o IZQ ponemos las ruedas rectas
    // 	    if ( !OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_LEFT ) &&
    // 		 !OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_RIGHT ) )
    // 		endereza = true;

    // 	    //Se le aplica el empujón al coche
    // 	    _vCoches[0]->accelerate ( endereza );
    // 	  }
    // 	else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_DOWN ) )
    // 	  {
    // 	    //Si no se tienen pulsadas las teclas DER o IZQ ponemos las ruedas rectas
    // 	    if ( !OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_LEFT ) &&
    // 		 !OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_RIGHT ) )
    // 		endereza = true;

    // 	    //Se le aplica el empujón al coche
    // 	    _vCoches[0]->decelerate ( endereza );
    // 	  }

    // 	if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_LEFT ) )
    // 	  {
    // 	    _vCoches[0]->turn_left();
    // 	  }
    // 	else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_RIGHT ) )
    // 	  {
    // 	    _vCoches[0]->turn_right();
    // 	  }

    // 	_velocidad = Math::Abs(_vCoches[0]->getVehiclePtr()->getBulletVehicle()->getCurrentSpeedKmHour());

    // 	Mostrar_Velocidad ( _velocidad );

    //   }

    // if ( _vCoches[0]->isMeta(_world) )
    //   {
    //     if (!_estaEnMeta) {
    //       // Cuando pisa la meta, para controlar que ha entrado en la linea y no esta encima
    //       if (_controlMeta == 1 && _estaEnPreMeta) {
    //         // Ha finalizado la vuelta
    //         _sonidoMetaFX->play();
    //         if ( _tiempo > 1  )
    //         {
    //           if ( _mejorTiempo > _tiempo || _mejorTiempo < 1 )
    //           {
    //             _mejorTiempo = _tiempo;
    //           }
    // 	          Records::getSingleton().add ( _tiempo );
    // 		      Records::getSingleton().compacta ( 10 );
    // 		      Records::getSingleton().write();

    //           _controlMeta = 0;
    //           _tiempo = 0;
    //           _empieza_a_contar = true;
    //         }
    //       }
    //     }
    //     _estaEnMeta = true;

    //     if (_estaEnPreMeta) {
    //       _controlMeta++; // Viene de premeta el coche
    //     }
    //   } else {
    //     if (_vCoches[0]->isPreMeta(_world) && _estaEnMeta) {
    //       _controlMeta--; // Viene de premeta el coche
    //     }
    //     // Cuando sale de la linea de meta
    //     _estaEnMeta = false;
    //     if (!_vCoches[0]->isCircuito(_world)) {
    //       // Si se ha salido lo volvemos a poner en la meta
    //       reiniciarCoche();
    //     }
    //   }
    // _estaEnPreMeta = _vCoches[0]->isPreMeta(_world);

//    if ( m_enemies.size() > 0 )
//      {
//        unsigned int i = 1;
//
//        //Comprobamos según el estado en el que está el enemigo la(s) acción(es) que vamos a realizar
//        for ( std::deque<Enemy *>::iterator itEnemy = m_enemies.begin(); m_enemies.end() != itEnemy; itEnemy++, i++ )
// 	        {
//            // NOTA!! : En el metodo Update() del enemigo se realizan los cambios de estado de éste
// 	          State current_state = ((StateMachine&) (*itEnemy)->getStateMachine()).getCurrentStateObject();
//
//            for ( std::map<string, Action>::iterator it_a = current_state.getActions()->begin(); it_a != current_state.getActions()->end(); advance(it_a,1) )
//              {
//                if ( it_a->second.getName() == "walk_in_route" )
//                  {
//                    (*itEnemy)->walk_in_route();
//                  }
//                else if ( it_a->second.getName() == "stop_move" )
//                  {
//                    (*itEnemy)->stop_move();
//                  }
//                else if ( it_a->second.getName() == "shoot" )
//                  {
//                    //PENDIENTE
//                  }
//                else if ( it_a->second.getName() == "run_to" )
//                  {
//                    //PENDIENTE
//                  }
//                else if ( it_a->second.getName() == "watch_around" )
//                  {
//                    //PENDIENTE
//                  }
//              }
// 	        }
//      }

  }

void GameState::buildGUI()
  {
//    Ogre::OverlayElement *elem = NULL;

//    elem = m_pOverlayMgr->getOverlayElement("Panel_Tiempo_Game");
//    elem->show();
//
//    elem = m_pOverlayMgr->getOverlayElement("txtMejorTiempo");
//    elem->setCaption ( "Best Time:" );
//    elem = m_pOverlayMgr->getOverlayElement("Panel_MejorTiempo_Game");
//    elem->show();

//    elem = m_pOverlayMgr->getOverlayElement ( "cursor" );
//
//    if ( elem )
//      elem->hide();

//    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->hideCursor();
  }

// string GameState::getTime ( double tiempo )
//   {
//     unsigned int minutos = 0, segundos = 0;
//     char cad[6];
//     string ret = "";

//     minutos = (int)tiempo / 60;
//     segundos = (int)tiempo % 60;

//     sprintf ( cad, "%02d:%02d", minutos, segundos );

//     ret = cad;

//     return ret;
//   }

// void GameState::Mostrar_Velocidad ( float velocidad, bool ocultar )
//   {
//     Ogre::OverlayElement *elem, *textArea;
//     int tipo = 0;
//     float porcion = MAX_SPEED / 5.5;

//     elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad5_Game");
//     elem->hide();
//     elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad4_Game");
//     elem->hide();
//     elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad3_Game");
//     elem->hide();
//     elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad2_Game");
//     elem->hide();
//     elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad1_Game");
//     elem->hide();
//     elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad0_Game");
//     elem->hide();

//     if ( !ocultar )
//       {
// 	tipo = velocidad / porcion;

// 	switch ( tipo )
// 	  {
// 	  case 5:
// 	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad5_Game");
// 	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad5");
// 	    break;
// 	  case 4:
// 	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad4_Game");
// 	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad4");
// 	    break;
// 	  case 3:
// 	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad3_Game");
// 	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad3");
// 	    break;
// 	  case 2:
// 	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad2_Game");
// 	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad2");
// 	    break;
// 	  case 1:
// 	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad1_Game");
// 	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad1");
// 	    break;
// 	  default:
// 	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad0_Game");
// 	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad0");
// 	    break;
// 	  }

// 	char cad[30];
// 	sprintf ( cad, " %d", (int)_velocidad );

// 	textArea->setCaption ( cad );
// 	elem->show();
//       }
//   }

// void GameState::reiniciarCoche () {
//   _tiempo = 0;
//   _controlMeta = 0;
//   _empieza_a_contar = true;
//   _vCoches[0]->reset();
// }

void GameState::CreateMap(string map)
{
    Entity *entity = m_pSceneMgr->createEntity(NAME_MAP, map + ".mesh");
    SceneNode *node = m_pSceneMgr->createSceneNode(NAME_MAP);
    node->attachObject(entity);

    m_pSceneMgr->getRootSceneNode()->addChild(node);
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new
    OgreBulletCollisions::StaticMeshToShapeConverter(entity);

    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimesh =
    trimeshConverter->createTrimesh();

    OgreBulletDynamics::RigidBody *rigidTrack = new
    OgreBulletDynamics::RigidBody(map, _world);
	rigidTrack->setStaticShape(node, trackTrimesh, 0.8, 0.95, Vector3::ZERO,
               Quaternion::IDENTITY);
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
		obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
		obB = const_cast<btCollisionObject*>(contactManifold->getBody1());

		OgreBulletCollisions::Object* obOB_A = world->findObject(obA);
		OgreBulletCollisions::Object* obOB_B = world->findObject(obB);

		for (int j=0;j<numHostages && !isCollition;j++) {
			hostage = hostages[j];
			if (hostage->getState() == CAPTURE) {
				isCollition = (obOB_A == hero->getRigidBody() || obOB_B == hero->getRigidBody())
							&& (obOB_A == hostage->getRigidBody() || obOB_B == hostage->getRigidBody());
				if (isCollition) hostageCollisition = hostage;
			}
		}
	}
	return hostageCollisition;
}

void GameState::CreatePlane()
  {
    assert ( _gc.getPlaneHeight() > 0 );
    assert ( _gc.getPlaneWidth() > 0 );

    // Define a floor plane mesh
 		Entity *ent;
    Plane p;
    p.normal = Vector3(0,1,0); p.d = 0;
    MeshManager::getSingleton().createPlane ( "FloorPlane",
                                              ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                              p, _gc.getPlaneHeight(), _gc.getPlaneWidth(),
                                              1, 1, true, 1, 1, 1,
                                              Vector3::UNIT_Z );
    // Create an entity (the floor)
    ent = m_pSceneMgr->createEntity("floor", "FloorPlane");
    ent->setMaterialName("MaterialMapa1");

//    Ogre::SceneNode *node = m_pSceneMgr->createSceneNode ( "FloorPlane" );
//    node->attachObject ( ent );
//    node->setPosition ( Ogre::Vector3::ZERO );
//    m_pSceneMgr->getRootSceneNode()->addChild ( node );

 		m_pSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject ( ent );

 		// add collision detection to it
 		//OgreBulletCollisions::CollisionShape *Shape;
 		ShapeFloor = new OgreBulletCollisions::StaticPlaneCollisionShape ( Ogre::Vector3 ( 0, 1, 0 ), 0 ); // (normal vector, distance)
 		// a body is needed for the shape
 		//OgreBulletDynamics::RigidBody *defaultPlaneBodyFloor = new OgreBulletDynamics::RigidBody("BasePlane",
    defaultPlaneBodyFloor = new OgreBulletDynamics::RigidBody ( "BasePlane", _world );
 		defaultPlaneBodyFloor->setStaticShape ( ShapeFloor, 0.1, 0.8 );// (shape, restitution, friction)
 		// push the created objects to the deques
// 		mShapes.push_back(Shape);
// 		mBodies.push_back(defaultPlaneBody);
    Ogre::Vector3 pos;

    for ( unsigned int i = 0; i < _gc.getNumColumns(); i++ )
      {
        pos = _gc.getPositionColumn(i);

        put_column ( pos, i );
      }
  }

void GameState::put_column ( const Ogre::Vector3& pos, unsigned int index )
  {
    string name = "Columna" + StringConverter::toString(index);

    Utilities::getSingleton().put_element_in_scene ( m_pSceneMgr, _world, "Plano_Alzado", name, pos );
//    string name = "Columna" + StringConverter::toString(index);
//
//    Entity *entity = m_pSceneMgr->createEntity ( name, "Plano_Alzado.mesh" );
//
//    entity->setVisible ( true );
//
//    SceneNode *node = m_pSceneMgr->createSceneNode ( name );
//    node->attachObject ( entity );
//
//    node->setPosition ( pos );
//
//    m_pSceneMgr->getRootSceneNode()->addChild ( node );
//    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new
//                      OgreBulletCollisions::StaticMeshToShapeConverter ( entity );
//
//    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimesh =
//                      trimeshConverter->createTrimesh();
//
//    OgreBulletDynamics::RigidBody *rigidTrack = new
//                      OgreBulletDynamics::RigidBody ( name, _world );
//    rigidTrack->setShape ( node, trackTrimesh, 0.8, 0.95, 0, Vector3::ZERO,
//                      Quaternion::IDENTITY );
  }












