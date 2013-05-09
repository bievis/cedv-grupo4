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

using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

using namespace Ogre;

#define MAX_SPEED 70
GameState::GameState()
  {
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
  }

void GameState::enter()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Entering GameState...");

    OIS::ParamList param;
    size_t windowHandle;
    std::ostringstream wHandleStr;

    _tiempo = 0;
    _mejorTiempo = 0;
    _empieza_a_contar = true;
    _estaEnMeta         = false;
    _controlMeta = 0;

    _gameTrack = TrackManager::getSingleton().load("musicGame.mp3");
    _sonidoMetaFX = SoundFXManager::getSingleton().load("claxon.wav");

    // Reproducción del track principal...
    _gameTrack->play();

    m_pSceneMgr = OgreFramework::getSingletonPtr()->getRootPtr()->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight ( Ogre::ColourValue ( 0.9f, 0.9f, 0.9f ) );

    m_pCamera = m_pSceneMgr->createCamera ( "GameCamera" );
    m_pCamera->setPosition(Ogre::Vector3 ( 40.0f, 80.0f, 0.0f ) );
    m_pCamera->lookAt(Ogre::Vector3 ( 0.0f, 13.0f, 0.0f ) );
    m_pCamera->setNearClipDistance(5);
    m_pCamera->setFarClipDistance(10000);

    OgreFramework::getSingletonPtr()->getViewportPtr()->setCamera ( m_pCamera );

    m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

    OgreFramework::getSingletonPtr()->getRenderWindowPtr()->getCustomAttribute ( "WINDOW", &windowHandle );

    wHandleStr << windowHandle;
    param.insert ( std::make_pair ( "WINDOW", wHandleStr.str() ) );

    // Creacion del modulo de debug visual de Bullet ------------------
    _debugDrawer = new OgreBulletCollisions::DebugDrawer();
    _debugDrawer->setDrawWireframe(true);
    SceneNode *node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode ( "debugNode", Vector3::ZERO );
    node->attachObject(static_cast <SimpleRenderable *>(_debugDrawer));

    // Creacion del mundo (definicion de los limites y la gravedad) ---
    AxisAlignedBox worldBounds = AxisAlignedBox (
    Vector3 (-100, -100, -100),
    Vector3 (100,  100,  100));
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

    // Crear el fondo de tierra

    insertarElementoEscena(string("Suelo"));

    // Crear el circuito

    insertarElementoEscena(string("Circuito"));

    // Crear la linea de meta

    insertarElementoEscena(string("PreMeta"));
    insertarElementoEscena(string("Meta"));

    // Crear la valla externa del circuito

    insertarElementoEscena(string("Valla_Externa"));

    // Crear la valla interna del circuito

    insertarElementoEscena(string("Valla_Interna"));

    // Creamos el/los vehiculo/s =============================================

    char name[100];
    eColour_Chassis color = RED;
    float posX = 13.0f;
    float posY = 12.0f;
    float posZ = 0.0f;
    float base = 7.0f;

    for ( unsigned int i = 0; i < _NUM_COCHES_; i++ )
      {
	if ( i % 2 == 0 )
	  {
	    if ( i > 0 )
	      base -= 7.0f;
	    posX = 18.0f;
            posZ = base;
	  }
        else
	  {
            posX = 22.0f;
            posZ = base - 3.0f;
	  }

	memset ( name, 0, sizeof(char)*100 );
	sprintf ( name, "Coche%03u", i+1 );
	_vCoches.push_back ( new Coche ( name, posX, posY, posZ,  m_pSceneMgr, _world, color ) );
	_vCoches[i]->print_info();

	if ( i == 0 )
	  color = BLUE;
	else if ( i == 1 )
	  color = GREEN;
	else if ( i == 2 )
	  color = YELLOW;
      }

  }

void GameState::insertarElementoEscena (string nombreElemento) {
  Entity *entity = m_pSceneMgr->createEntity(nombreElemento, nombreElemento + string(".mesh"));
  SceneNode *node = m_pSceneMgr->createSceneNode(nombreElemento);
  node->attachObject(entity);

  m_pSceneMgr->getRootSceneNode()->addChild(node);
  OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new
    OgreBulletCollisions::StaticMeshToShapeConverter(entity);

  OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimesh =
    trimeshConverter->createTrimesh();

  OgreBulletDynamics::RigidBody *rigidTrack = new
    OgreBulletDynamics::RigidBody(nombreElemento, _world);
  rigidTrack->setShape(node, trackTrimesh, 0.8, 0.95, 0, Vector3::ZERO,
  		         Quaternion::IDENTITY);

  delete trimeshConverter;
}

bool GameState::pause()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Pausing GameState...");

    Mostrar_Velocidad ( 0, true );

    Ogre::OverlayElement *elem;

    elem = m_pOverlayMgr->getOverlayElement("Panel_Tiempo_Game");
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("Panel_MejorTiempo_Game");
    elem->hide();

    _gameTrack->pause();

    return true;
  }

void GameState::resume()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Resuming GameState...");

    OgreFramework::getSingletonPtr()->getViewportPtr()->setCamera(m_pCamera);
    m_bQuit = false;
    Mostrar_Velocidad ( 0 );

    Ogre::OverlayElement *elem;

    elem = m_pOverlayMgr->getOverlayElement("Panel_Tiempo_Game");
    elem->show();

    elem = m_pOverlayMgr->getOverlayElement("Panel_MejorTiempo_Game");
    elem->show();

    elem = m_pOverlayMgr->getOverlayElement ( "cursor" );

    if ( elem )
      elem->hide();

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->hideCursor();

    _gameTrack->play();
  }

void GameState::exit()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Leaving GameState...");

    if ( _vCoches.size() > 0 )
      {
	for ( unsigned int i = 0; i < _vCoches.size(); i++ )
	  {
	    if ( _vCoches[i] )
	      delete _vCoches[i];
	  }

	_vCoches.clear();
      }

    // Parar del track principal...
    _gameTrack->play();
    _gameTrack->stop();

    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
      OgreFramework::getSingletonPtr()->getRootPtr()->destroySceneManager(m_pSceneMgr);

    // Ocultar overlays
    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    overlay->hide();
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
    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    overlay->show();
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
    else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_R ) ) {
      // Volver a poner el coche en el inicio
      reiniciarCoche();
    }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
  }

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
  {
    OgreFramework::getSingletonPtr()->keyPressed ( keyEventRef );

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
    if ( _empieza_a_contar )
      _tiempo += timeSinceLastFrame;

    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->frameRenderingQueued ( m_FrameEvent );

    if(m_bQuit == true)
      {
        popAppState();
        return;
      }

    Ogre::OverlayElement *elem = NULL;
    elem = m_pOverlayMgr->getOverlayElement("txtTiempo");
    elem->setCaption ( getTime(_tiempo) );

    elem = m_pOverlayMgr->getOverlayElement("txtMejorTiempo");
    elem->setCaption ( "Best Time:\n  " + getTime(_mejorTiempo) );
    elem = m_pOverlayMgr->getOverlayElement("Panel_MejorTiempo_Game");
    elem->show();

    Ogre::Vector3 vt(0,0,0);
    Ogre::Real deltaT = timeSinceLastFrame;
    bool endereza = false;

    //    bool mbleft, mbmiddle, mbright; // Botones del raton pulsados

    _world->stepSimulation(deltaT); // Actualizar simulacion Bullet

    if ( _vCoches.size() > 0 )
      {
	_vCoches[0]->getVehiclePtr()->applyEngineForce ( 0, 0 );
	_vCoches[0]->getVehiclePtr()->applyEngineForce ( 0, 1 );

	if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_UP ) )
	  {
	    //Si no se tienen pulsadas las teclas DER o IZQ ponemos las ruedas rectas
	    if ( !OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_LEFT ) &&
		 !OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_RIGHT ) )
		endereza = true;

	    //Se le aplica el empujón al coche
	    _vCoches[0]->accelerate ( endereza );
    	  }
	else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_DOWN ) )
	  {
	    //Si no se tienen pulsadas las teclas DER o IZQ ponemos las ruedas rectas
	    if ( !OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_LEFT ) &&
		 !OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_RIGHT ) )
		endereza = true;

	    //Se le aplica el empujón al coche
	    _vCoches[0]->decelerate ( endereza );
	  }

	if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_LEFT ) )
	  {
	    _vCoches[0]->turn_left();
	  }
	else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_RIGHT ) )
	  {
	    _vCoches[0]->turn_right();
	  }

	_velocidad = Math::Abs(_vCoches[0]->getVehiclePtr()->getBulletVehicle()->getCurrentSpeedKmHour());

	Mostrar_Velocidad ( _velocidad );

      }

    if ( _vCoches[0]->isMeta(_world) )
      {
        if (!_estaEnMeta) {
          // Cuando pisa la meta, para controlar que ha entrado en la linea y no esta encima
          if (_controlMeta == 1 && _estaEnPreMeta) {
            // Ha finalizado la vuelta
            _sonidoMetaFX->play();
            if ( _tiempo > 1  )
            {
              if ( _mejorTiempo > _tiempo || _mejorTiempo < 1 )
              {
                _mejorTiempo = _tiempo;
              }
   	          Records::getSingleton().add ( _tiempo );
		      Records::getSingleton().compacta ( 10 );
		      Records::getSingleton().write();

              _controlMeta = 0;
              _tiempo = 0;
              _empieza_a_contar = true;
            }
          }
        }
        _estaEnMeta = true;

        if (_estaEnPreMeta) {
          _controlMeta++; // Viene de premeta el coche
        }
      } else {
        if (_vCoches[0]->isPreMeta(_world) && _estaEnMeta) {
          _controlMeta--; // Viene de premeta el coche
        }
        // Cuando sale de la linea de meta
        _estaEnMeta = false;
        if (!_vCoches[0]->isCircuito(_world)) {
          // Si se ha salido lo volvemos a poner en la meta
          reiniciarCoche();
        }
      }
    _estaEnPreMeta = _vCoches[0]->isPreMeta(_world);
  }

void GameState::buildGUI()
  {
    Ogre::OverlayElement *elem = NULL;

    elem = m_pOverlayMgr->getOverlayElement("Panel_Tiempo_Game");
    elem->show();

    elem = m_pOverlayMgr->getOverlayElement("txtMejorTiempo");
    elem->setCaption ( "Best Time:" );
    elem = m_pOverlayMgr->getOverlayElement("Panel_MejorTiempo_Game");
    elem->show();

    elem = m_pOverlayMgr->getOverlayElement ( "cursor" );

    if ( elem )
      elem->hide();

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->hideCursor();
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

void GameState::Mostrar_Velocidad ( float velocidad, bool ocultar )
  {
    Ogre::OverlayElement *elem, *textArea;
    int tipo = 0;
    float porcion = MAX_SPEED / 5.5;

    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad5_Game");
    elem->hide();
    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad4_Game");
    elem->hide();
    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad3_Game");
    elem->hide();
    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad2_Game");
    elem->hide();
    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad1_Game");
    elem->hide();
    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad0_Game");
    elem->hide();

    if ( !ocultar )
      {
	tipo = velocidad / porcion;

	switch ( tipo )
	  {
	  case 5:
	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad5_Game");
	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad5");
	    break;
	  case 4:
	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad4_Game");
	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad4");
	    break;
	  case 3:
	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad3_Game");
	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad3");
	    break;
	  case 2:
	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad2_Game");
	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad2");
	    break;
	  case 1:
	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad1_Game");
	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad1");
	    break;
	  default:
	    elem = m_pOverlayMgr->getOverlayElement("Panel_Velocidad0_Game");
	    textArea = m_pOverlayMgr->getOverlayElement("txtVelocidad0");
	    break;
	  }

	char cad[30];
	sprintf ( cad, " %d", (int)_velocidad );

	textArea->setCaption ( cad );
	elem->show();
      }
  }

void GameState::reiniciarCoche () {
  _tiempo = 0;
  _controlMeta = 0;
  _empieza_a_contar = true;
  _vCoches[0]->reset();
}
