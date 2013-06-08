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

#define NUM_ENEMIES 2

GameState::GameState()
  {
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
    m_pSceneMgr         = NULL;
    m_hero              = NULL;
    m_pOverlayMgr       = NULL;
    m_enemies.clear();
    defaultPlaneBodyFloor = NULL;
    ShapeFloor          = NULL;
    _world              = NULL;
  }

void GameState::enter()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Entering GameState...");

    // OIS::ParamList param;
    // size_t windowHandle;
    // std::ostringstream wHandleStr;

    // _tiempo = 0;
    // _mejorTiempo = 0;
    // _empieza_a_contar = true;
    // _estaEnMeta         = false;
    // _controlMeta = 0;

    // _gameTrack = TrackManager::getSingleton().load("musicGame.mp3");
    // _sonidoMetaFX = SoundFXManager::getSingleton().load("claxon.wav");

    // // Reproducción del track principal...
    // _gameTrack->play();

    m_pSceneMgr = OgreFramework::getSingletonPtr()->getRootPtr()->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight ( Ogre::ColourValue ( 0.9f, 0.9f, 0.9f ) );

    m_pCamera = m_pSceneMgr->createCamera ( "GameCamera" );

//    m_pCamera->setPosition(Ogre::Vector3 ( 40.0f, 80.0f, 0.0f ) );
//    m_pCamera->lookAt(Ogre::Vector3 ( 0.0f, 13.0f, 0.0f ) );
//    m_pCamera->setNearClipDistance(5);
//    m_pCamera->setFarClipDistance(10000);

    // Position it at 500 in Z direction
    m_pCamera->setPosition(Vector3(0,18,70));
      // Look back along -Z
    m_pCamera->lookAt(Vector3(0,0,-300));
    m_pCamera->setNearClipDistance(5);

    OgreFramework::getSingletonPtr()->getViewportPtr()->setCamera ( m_pCamera );

    // m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

    // OgreFramework::getSingletonPtr()->getRenderWindowPtr()->getCustomAttribute ( "WINDOW", &windowHandle );

    // wHandleStr << windowHandle;
    // param.insert ( std::make_pair ( "WINDOW", wHandleStr.str() ) );

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

    // Creacion de los elementos iniciales del mundo
    CreateInitialWorld();

    buildGUI();

    createScene();

  }

void GameState::CreateInitialWorld()
  {
    // Activamos las sombras
    m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    m_pSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));

    m_pSceneMgr->setShadowTextureCount(2);
    m_pSceneMgr->setShadowTextureSize(512);

    // Crear el plano
    CreatePlane();
    // insertarElementoEscena(string("Suelo"));

    m_hero = new Hero ( m_pSceneMgr, _world, "Hero", 0, 0, 10 );
    m_hero->print();
    //ptrNode = Utilities::getSingleton().put_element_in_scene ( m_pSceneMgr, _world, "Cube" );

    Enemy *enemy = NULL;
    string name_enemy = "";

    for ( unsigned int i = 0, j = 8; i < NUM_ENEMIES; i++, j+=8 )
      {
        name_enemy = "Enemy" + StringConverter::toString(i);
        enemy = new Enemy ( m_pSceneMgr, _world, name_enemy, j, 0, 0 );
        m_enemies.push_back ( enemy );
      }

    //Plano donde se verá lo que está viendo el personaje principal
    //*************************************************************************
//    rtt = TextureManager::getSingleton().createManual(
//            "RttT_Hero", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
//            TEX_TYPE_2D, 64, 64, 0, PF_A8R8G8B8, TU_RENDERTARGET );
//
//    rtex = rtt->getBuffer()->getRenderTarget();
//
//    camPOV = m_pSceneMgr->createCamera ( "cameraPOV_Hero" );
//    camPOV->setPosition ( Ogre::Vector3 ( 0, 2, -4 ) );
//    camPOV->lookAt ( Ogre::Vector3 ( 0, 2, -14 ) );
//    camPOV->setNearClipDistance ( 5 );
//    camPOV->setFOVy ( Degree ( 38 ) );
//
//    rtex->addViewport ( camPOV );
//    rtex->getViewport(0)->setClearEveryFrame ( true );
//    rtex->getViewport(0)->setBackgroundColour ( ColourValue::Black );
//    rtex->getViewport(0)->setOverlaysEnabled ( false );
//    rtex->setAutoUpdated(true);

    MaterialPtr mPtr = MaterialManager::getSingleton().create ( "RttMat_Enemy0", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
    Technique* matTechnique = mPtr->createTechnique();
    matTechnique->createPass();
    mPtr->getTechnique(0)->getPass(0)->setLightingEnabled(true);
    mPtr->getTechnique(0)->getPass(0)->setDiffuse(0.9,0.9,0.9,1);
    mPtr->getTechnique(0)->getPass(0)->setSelfIllumination(0.4,0.4,0.4);

    mPtr->getTechnique(0)->getPass(0)->createTextureUnitState("RttT_Enemy0");

    Ogre::Rectangle2D* _rect = new Ogre::Rectangle2D ( true );
    _rect->setCorners ( -0.25, 1, 0.25, 0.5 ); //( -0.5, 0, 0.5, -1 );
    _rect->setMaterial ( "RttMat_Enemy0" );

    // Render the background before everything else
    _rect->setRenderQueueGroup ( Ogre::RENDER_QUEUE_BACKGROUND );

    // Attach background to the scene
    Ogre::SceneNode* node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode ( "rectanglePOV_Enemy0" );
    node->attachObject ( _rect );
    //*************************************************************************

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

    // if ( _vCoches.size() > 0 )
    //   {
    // 	for ( unsigned int i = 0; i < _vCoches.size(); i++ )
    // 	  {
    // 	    if ( _vCoches[i] )
    // 	      delete _vCoches[i];
    // 	  }

    // 	_vCoches.clear();
    //   }

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

void GameState::createScene()
  {
    // Luz de la escena
    Light* luz = m_pSceneMgr->createLight("Luz");
    luz->setType(Light::LT_POINT);
    luz->setPosition(75,75,75);
    luz->setSpecularColour(1, 1, 1);
    luz->setDiffuseColour(1, 1, 1);

    //Cargamos overlay con la GUI ( Tiempo )
//    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
//    overlay->show();
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
    // else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_R ) ) {
    //   // Volver a poner el coche en el inicio
    //   reiniciarCoche();
    // }
//    else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_W ) )
//    {
//      m_hero->check_vision();
//      //check_vision();
//    }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
  }

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
  {
    OgreFramework::getSingletonPtr()->keyPressed ( keyEventRef );

    if (keyEventRef.key == OIS::KC_UP || keyEventRef.key == OIS::KC_DOWN || keyEventRef.key == OIS::KC_X ) {
        m_hero->stop_move(); //getRigidBody()->setLinearVelocity(Ogre::Vector3(0.0, 0.0, 0.0));
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

    // Ogre::Vector3 vt(0,0,0);
    Ogre::Real deltaT = timeSinceLastFrame;
    // bool endereza = false;

    // //    bool mbleft, mbmiddle, mbright; // Botones del raton pulsados

    _world->stepSimulation(deltaT); // Actualizar simulacion Bullet

    // if ( _vCoches.size() > 0 )
    //   {
    // 	_vCoches[0]->getVehiclePtr()->applyEngineForce ( 0, 0 );
    // 	_vCoches[0]->getVehiclePtr()->applyEngineForce ( 0, 1 );

    bool bMove = false;
    float valX = 0, valZ = 0;

    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_RIGHT ) )
      {
//        bMove = true;
//        valX = 0.05;
        m_hero->turn ( (-1) * Ogre::Math::HALF_PI / 32 );
      }
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_LEFT ) )
      {
//        bMove = true;
//        valX = -0.05;
        m_hero->turn ( Ogre::Math::HALF_PI / 32 );
      }
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_DOWN ) )
      {
//        bMove = true;
//        valZ = 0.05;
        m_hero->walk ( true );
      }
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_UP ) )
      {
//        bMove = true;
//        valZ = -0.05;
        m_hero->walk();
      }
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_X ) )
      {
        m_hero->walk_to(m_enemies[0]->getSceneNode()->getPosition());
      }

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

    if ( m_enemies.size() > 0 )
      {
        unsigned int i = 1;
        for ( std::deque<Enemy *>::iterator itEnemy = m_enemies.begin(); m_enemies.end() != itEnemy; itEnemy++, i++ )
 	        {
            if ( (*itEnemy)->haveYouSeenAnybody() )
              cout << "Enemy" + StringConverter::toString(i) + "::te veo!!" << endl;
 	        }
      }

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

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->hideCursor();
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

void GameState::CreatePlane()
  {
    // Define a floor plane mesh
    Plane p;
    p.normal = Vector3(0,1,0); p.d = 0;
    MeshManager::getSingleton().createPlane ( "FloorPlane",
                                                        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                        p, 200000, 200000, 20, 20, true, 1, 9000, 9000,
                                                        Vector3::UNIT_Z );
    // Create an entity (the floor)
    entFloor = m_pSceneMgr->createEntity ( "floor", "FloorPlane" );
 		entFloor->setMaterialName("Pruebas/BumpyMetal");

 		m_pSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject ( entFloor );

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
  }
