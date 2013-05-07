#include "GameState.hpp"
#include "GameManager.h"
#include <GameConfigException.hpp>

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

// static float gWheelRadius = 0.5f;
// static float gWheelWidth = 0.4f;
// static float gWheelFriction = 1e30f;
// static float gRollInfluence = 0.1f;
// static float gSuspensionRestLength = 0.6;
// static float gEngineForce = 1000.0;

using namespace Ogre;

#define MAX_SPEED 80
GameState::GameState()
  {
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
    _estaEnMeta         = false;
    //    _ptrGameConfig      = NULL;
  }

void GameState::enter()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Entering GameState...");

    OIS::ParamList param;
    size_t windowHandle;
    std::ostringstream wHandleStr;

    _tiempo = 0;
    _mejorTiempo = 0;
    _empieza_a_contar = false;
    // _level = 1;
    // _vidas = 3;
    // _estado = GAME;
    // _tiempoMuertoFin = 0.0;

    _gameTrack = TrackManager::getSingleton().load("Hank_Williams_Junior_-_Canyonero.mp3");
    // _gameoverTrack = TrackManager::getSingleton().load("game_over.mp3");
    // _winnerTrack = TrackManager::getSingleton().load("aplausos.mp3");
    //_moverPersonajeFX = SoundFXManager::getSingleton().load("movimiento.wav");
    _sonidoMetaFX = SoundFXManager::getSingleton().load("claxon.wav");

    // // Reproducción del track principal...
    _gameTrack->play();

    m_pSceneMgr = OgreFramework::getSingletonPtr()->getRootPtr()->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight ( Ogre::ColourValue ( 0.9f, 0.9f, 0.9f ) );

    // m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    // m_pCamera->setPosition(Vector3(0.0, 19, 0.1));
    // m_pCamera->lookAt(Vector3(0, 0, 0));
    // m_pCamera->setNearClipDistance(5);
    // m_pCamera->setFarClipDistance(10000);

    m_pCamera = m_pSceneMgr->createCamera ( "GameCamera" );
    //m_pCamera->setPosition(Ogre::Vector3 ( 0, 17 * 10, 1 ) );
    m_pCamera->setPosition(Ogre::Vector3 ( 40.0f, 80.0f, 0.0f ) );
    m_pCamera->lookAt(Ogre::Vector3 ( 0.0f, 13.0f, 0.0f ) );
    m_pCamera->setNearClipDistance(5);
    m_pCamera->setFarClipDistance(10000);

    // m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
    //     Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

    OgreFramework::getSingletonPtr()->getViewportPtr()->setCamera ( m_pCamera );

    m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

    // _ptrGameConfig = &(OgreFramework::getSingletonPtr()->_gameConfig);

    OgreFramework::getSingletonPtr()->getRenderWindowPtr()->getCustomAttribute ( "WINDOW", &windowHandle );

    wHandleStr << windowHandle;
    param.insert ( std::make_pair ( "WINDOW", wHandleStr.str() ) );

//    _inputManager = OIS::InputManager::createInputSystem ( param );
//    _keyboard = static_cast<OIS::Keyboard*>
//    (_inputManager->createInputObject(OIS::OISKeyboard, false));
//    _mouse = static_cast<OIS::Mouse*>
//    (_inputManager->createInputObject(OIS::OISMouse, false));
//    _mouse->getMouseState().width = win->getWidth();
//    _mouse->getMouseState().height = win->getHeight();

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

    // Creacion de la entidad y del SceneNode ------------------------
    //Plane plane1(Vector3(0,1,0), 0);    // Normal y distancia
    //MeshManager::getSingleton().createPlane("p1",
		//			    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
		//			    200, 200, 1, 1, true, 1, 20, 20, Vector3::UNIT_Z);
    //SceneNode* nodep = m_pSceneMgr->createSceneNode("ground");
    //Entity* groundEnt = m_pSceneMgr->createEntity("planeEnt", "p1");
    //groundEnt->setMaterialName("Ground");
    //nodep->attachObject(groundEnt);
    //m_pSceneMgr->getRootSceneNode()->addChild(nodep);

    // Creamos forma de colision para el plano -----------------------
    //OgreBulletCollisions::CollisionShape *Shape;
    //Shape = new OgreBulletCollisions::StaticPlaneCollisionShape
    //  (Vector3(0,1,0), 0);   // Vector normal y distancia
    //OgreBulletDynamics::RigidBody *rigidBodyPlane = new
    //OgreBulletDynamics::RigidBody("rigidBodyPlane", _world);

    // Creamos la forma estatica (forma, Restitucion, Friccion) ------
    //rigidBodyPlane->setStaticShape(Shape, 0.1, 0.8);

    // // Crear el fondo de tierra

    Entity *entitySuelo = m_pSceneMgr->createEntity("Suelo", "Suelo.mesh");
    SceneNode *nodeSuelo = m_pSceneMgr->createSceneNode("Suelo");
    nodeSuelo->attachObject(entitySuelo);

    m_pSceneMgr->getRootSceneNode()->addChild(nodeSuelo);
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverterSuelo = new
      OgreBulletCollisions::StaticMeshToShapeConverter(entitySuelo);

    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimeshSuelo =
      trimeshConverterSuelo->createTrimesh();

    OgreBulletDynamics::RigidBody *rigidTrackSuelo = new
      OgreBulletDynamics::RigidBody("Suelo", _world);
    rigidTrackSuelo->setShape(nodeSuelo, trackTrimeshSuelo, 0.8, 0.95, 0, Vector3::ZERO,
    		         Quaternion::IDENTITY);

    delete trimeshConverterSuelo;

    // Crear el circuito

    Entity *entityCircuito = m_pSceneMgr->createEntity("Circuito", "Circuito.mesh");
    SceneNode *nodeCircuito = m_pSceneMgr->createSceneNode("Circuito");
    nodeCircuito->attachObject(entityCircuito);

    m_pSceneMgr->getRootSceneNode()->addChild(nodeCircuito);
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverterCircuito = new
      OgreBulletCollisions::StaticMeshToShapeConverter(entityCircuito);

    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimeshCircuito =
      trimeshConverterCircuito->createTrimesh();

    OgreBulletDynamics::RigidBody *rigidTrackCircuito = new
      OgreBulletDynamics::RigidBody("Circuito", _world);
    rigidTrackCircuito->setShape(nodeCircuito, trackTrimeshCircuito, 0.8, 0.95, 0, Vector3::ZERO,
		         Quaternion::IDENTITY);

    delete trimeshConverterCircuito;

    // // Crear la linea de meta

    Entity *entityMeta = m_pSceneMgr->createEntity("Meta", "Meta.mesh");
    SceneNode *nodeMeta = m_pSceneMgr->createSceneNode("Meta");
    nodeMeta->attachObject(entityMeta);

    m_pSceneMgr->getRootSceneNode()->addChild(nodeMeta);
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverterMeta = new
      OgreBulletCollisions::StaticMeshToShapeConverter(entityMeta);

    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimeshMeta =
      trimeshConverterMeta->createTrimesh();

    OgreBulletDynamics::RigidBody *rigidTrackMeta = new
      OgreBulletDynamics::RigidBody("Meta", _world);
    rigidTrackMeta->setShape(nodeMeta, trackTrimeshMeta, 0.8, 0.95, 0, Vector3::ZERO,
    		         Quaternion::IDENTITY);

    delete trimeshConverterMeta;

    // Crear la valla externa del circuito

    Entity *entityBarreraExterior = m_pSceneMgr->createEntity("Valla_Externa", "Valla_Externa.mesh");
    SceneNode *nodeBarreraExterior = m_pSceneMgr->createSceneNode("Valla_Externa");
    nodeBarreraExterior->attachObject(entityBarreraExterior);

    m_pSceneMgr->getRootSceneNode()->addChild(nodeBarreraExterior);
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverterBarreraExterior = new
      OgreBulletCollisions::StaticMeshToShapeConverter(entityBarreraExterior);

    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimeshBarreraExterior =
      trimeshConverterBarreraExterior->createTrimesh();

    OgreBulletDynamics::RigidBody *rigidTrackBarreraExterior = new
      OgreBulletDynamics::RigidBody("Valla_Externa", _world);
    rigidTrackBarreraExterior->setShape(nodeBarreraExterior, trackTrimeshBarreraExterior, 0.8, 0.95, 0, Vector3::ZERO,
		         Quaternion::IDENTITY);

    delete trimeshConverterBarreraExterior;

    // Crear la valla interna del circuito

    Entity *entityBarreraInterior = m_pSceneMgr->createEntity("Valla_Interna", "Valla_Interna.mesh");
    SceneNode *nodeBarreraInterior = m_pSceneMgr->createSceneNode("Valla_Interna");
    nodeBarreraInterior->attachObject(entityBarreraInterior);

    m_pSceneMgr->getRootSceneNode()->addChild(nodeBarreraInterior);
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverterBarreraInterior = new
      OgreBulletCollisions::StaticMeshToShapeConverter(entityBarreraInterior);

    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimeshBarreraInterior =
      trimeshConverterBarreraInterior->createTrimesh();

    OgreBulletDynamics::RigidBody *rigidTrackBarreraInterior = new
      OgreBulletDynamics::RigidBody("Valla_Interna", _world);
    rigidTrackBarreraInterior->setShape(nodeBarreraInterior, trackTrimeshBarreraInterior, 0.8, 0.95, 0, Vector3::ZERO,
		         Quaternion::IDENTITY);

    delete trimeshConverterBarreraInterior;

    // Creamos el vehiculo =============================================

    char name[100];
    float pos_x = -1.0;
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

    // Historias pa no dormir
    //****************************************************
    // Supongo que los scenenodes que empiezan por Unnamed son el chasis y las ruedas de cada coche
    // y tambien supongo que el primero de los cinco es el chasis
    //****************************************************
  //   Node *node_;
  //   SceneNode::ChildNodeIterator it = m_pSceneMgr->getRootSceneNode()->getChildIterator();

  //   cout << "=====" << endl;
  //   cout << "Nodos" << endl;
  //   cout << "=====" << endl;

  //   unsigned int encontrado = 0;

  //   while (it.hasMoreElements())
  //     {
  // 	node_ = it.getNext();

  // 	cout << "name : " << node_->getName();

  //       if ( node_->getName().find("Unnamed") != std::string::npos )
  // 	  {
  // 	    if ( encontrado % 5 == 0 )
  // 	      {
  // 		cout << " *** Coche??" << endl;
  // 		_vSceneNode_Coches.push_back ( node_ );
  // 	      }
  // 	    else
  // 	      cout << endl;

  // 	    encontrado++;
  // 	  }
  // 	else
  // 	  cout << endl;
  //     }

  //   cout << "=====" << endl;
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

    // elem = m_pOverlayMgr->getOverlayElement("panelVidas2");
    // elem->hide();

    // elem = m_pOverlayMgr->getOverlayElement("panelVidas1");
    // elem->hide();

    // elem = m_pOverlayMgr->getOverlayElement("panelTiempo");
    // elem->hide();

    // elem = m_pOverlayMgr->getOverlayElement("panelNivel");
    // elem->hide();

    return true;
  }

void GameState::resume()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Resuming GameState...");

    // buildGUI();

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

//    _vSceneNode_Coches.clear();

    // Parar del track principal...
    _gameTrack->play();
    _gameTrack->stop();
    // _gameoverTrack->stop();
    // _winnerTrack->stop();

    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
      OgreFramework::getSingletonPtr()->getRootPtr()->destroySceneManager(m_pSceneMgr);

    // GameManager::getSingleton().limpiar ();

    // Ocultar overlays
    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    overlay->hide();

    // Ogre::OverlayElement *elem;
    // elem = m_pOverlayMgr->getOverlayElement("panelVidas2");
    // elem->hide();

    // elem = m_pOverlayMgr->getOverlayElement("panelVidas1");
    // elem->hide();

    // overlay = m_pOverlayMgr->getByName("Screens_Game");
    // overlay->hide();

    // elem = m_pOverlayMgr->getOverlayElement("panelWinner");
    // elem->hide();

    // elem = m_pOverlayMgr->getOverlayElement("panelNextLevel");
    // elem->hide();

    // elem = m_pOverlayMgr->getOverlayElement("panelGameOver");
    // elem->hide();
  }

void GameState::createScene()
  {
    // // Creamos los objetos de la escena
    // m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
    // m_pSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
    // m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));

    // m_pSceneMgr->setShadowTextureCount(2);
    // m_pSceneMgr->setShadowTextureSize(512);

    // Entity* entSuelo = m_pSceneMgr->createEntity("Suelo", "Suelo.mesh");

    // // Creamos el escenario como geometria estatica
    // StaticGeometry* escenario = m_pSceneMgr->createStaticGeometry("EscenarioEstatico");
    // escenario->addEntity(entSuelo, Vector3(0,0,0));
    // escenario->build();  // Operacion para construir la geometria

    // // Luz de la escena
    Light* luz = m_pSceneMgr->createLight("Luz");
    luz->setType(Light::LT_POINT);
    luz->setPosition(75,75,75);
    luz->setSpecularColour(1, 1, 1);
    luz->setDiffuseColour(1, 1, 1);

    // // Partes del escenario

    // try
    //   {
    //     LoadScenaryParts();
    //   }
    // catch ( GameConfigException& exc )
    //   {
    // 	cerr << "EXCEPTION:: " << exc.what() << endl;
    //   }

    // // Cargamos el personaje
    // SceneNode* nodePersonaje = GameManager::getSingleton().
    //       crearNodo(m_pSceneMgr, "Marciano", "Marciano.mesh", 0, 0, 6.5);
    // SceneNode* nodePersonajeMuerto = GameManager::getSingleton().
    //       crearNodo(m_pSceneMgr, "MarcianoMuerto", "MarcianoMuerto.mesh", 0, 0, 6.5);
    // SceneNode* nodePersonajeEstrellas = GameManager::getSingleton().
    //       crearNodo(m_pSceneMgr, "Estrellas", "Estrellas.mesh", 0, 0, 6.5);
    // Personaje* p = new Personaje("Marciano", nodePersonaje, nodePersonajeMuerto, nodePersonajeEstrellas);
    // GameManager::getSingleton().setPersonaje(p);

    //Cargamos overlay con la GUI ( Tiempo )
    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    overlay->show();

    // Ogre::OverlayElement *elem;
    // elem = m_pOverlayMgr->getOverlayElement("panelVidas2");
    // elem->hide();

    // elem = m_pOverlayMgr->getOverlayElement("panelVidas1");
    // elem->hide();

    // //Cargamos las pantallas de Winner, Next Level y Game Over
    // overlay = m_pOverlayMgr->getByName("Screens_Game");
    // overlay->show();

    // elem = m_pOverlayMgr->getOverlayElement("panelWinner");
    // elem->setDimensions(OgreFramework::getSingletonPtr()->_factorX, OgreFramework::getSingletonPtr()->_factorY);
    // elem->hide();

    // elem = m_pOverlayMgr->getOverlayElement("panelNextLevel");
    // elem->setDimensions (OgreFramework::getSingletonPtr()->_factorX, OgreFramework::getSingletonPtr()->_factorY);
    // elem->hide();

    // elem = m_pOverlayMgr->getOverlayElement("panelGameOver");
    // elem->setDimensions(OgreFramework::getSingletonPtr()->_factorX, OgreFramework::getSingletonPtr()->_factorY);
    // elem->hide();
  }

// void GameState::LoadScenaryParts()
//   {

//     SceneNode* nodoZonaCrater = GameManager::getSingleton().
//           crearNodo(m_pSceneMgr, "Abismo", "Abismo.mesh", 0.0, 0.00001, -4.00000);
//     ParteEscenario* zona_crater = new ParteEscenario ( nodoZonaCrater->getName(), nodoZonaCrater, AGUA );
//     GameManager::getSingleton().addParteEscenario ( zona_crater );

//     SceneNode* nodoZonaCarretera = GameManager::getSingleton().
//           crearNodo(m_pSceneMgr, "Carretera", "Carretera.mesh", 0.0, 0.00001, 4.00000);
//     ParteEscenario* zona_carretera = new ParteEscenario ( nodoZonaCarretera->getName(), nodoZonaCarretera, CARRETERA );
//     GameManager::getSingleton().addParteEscenario ( zona_carretera );

//     // Cargamos el primer nivel
//     cargarNivel();
// }

// void GameState::cargarNivel() {
//     // Limpiamos el nivel anterior
//     GameManager::getSingleton().limpiarPartes();

//     // Construimos los carriles
//     Level lvl;
//     Row row;

//     _ptrGameConfig->getLevel ( _level, lvl );

//     double zona_crater_Z[] = { -2.5, -4, -5.5 };
//     double zona_carretera_Z[] = {  2,  3.5,  5 };

//     ParteEscenario* parte = NULL;

//     std::vector<ParteEscenario*>::const_iterator
//       mit (GameManager::getSingleton().getPartesEscenario().begin()),
//       mend(GameManager::getSingleton().getPartesEscenario().end());
//     for(;mit!=mend;++mit) {
//       parte = (*mit);

//       if (parte->getTipo() == AGUA) {
//         // Zona crater
//         for ( unsigned int i = 0; i < lvl.getCrater().getRows(); i++ )
//         {
//           lvl.getCrater().getRow ( i, row );
//           parte->addCarril ( row.get_name().c_str(), row.get_speed(), row.get_distance(), (row.get_way()==RIGHT)?DIR_DER:DIR_IZQ, zona_crater_Z[i%3], m_pSceneMgr );
//           parte->addModeloElementoCarril ( row.get_name().c_str(), "Plataforma.mesh" );
//         }
//       } else if (parte->getTipo() == CARRETERA) {
//         // Zona carretera
//         for ( unsigned int i = 0; i < lvl.getRoad().getRows(); i++ )
//         {
//           lvl.getRoad().getRow ( i, row );
//           parte->addCarril ( row.get_name().c_str(), row.get_speed(), row.get_distance(), (row.get_way()==RIGHT)?DIR_DER:DIR_IZQ, zona_carretera_Z[i%3], m_pSceneMgr );
//           parte->addModeloElementoCarril ( row.get_name().c_str(), "Meteorito.mesh" );
//           parte->addModeloElementoCarril ( row.get_name().c_str(), "Nave1.mesh" );
//           parte->addModeloElementoCarril ( row.get_name().c_str(), "Nave2.mesh" );
//         }
//       }
//     }
//  }

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
  {
    // if (_estado == GAME_OVER || _estado == WINNER) {
    //   // popAllAndPushAppState(findByName("MenuState"));
    //   // return true;
    // } else if (_estado == NEXT_LEVEL) {
    //   // Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    //   // overlay->show();
    //   // Ogre::OverlayElement *elem = m_pOverlayMgr->getOverlayElement("panelNextLevel");
    //   // elem->hide();
    //   // _tiempo = 0;
    //   // _estado = GAME;
    //   // return true;
    // }
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
      _tiempo = 0;
      _empieza_a_contar = false;
      _vCoches[0]->reset();
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
    // _tiempoTotal += timeSinceLastFrame;

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

    // Ogre::OverlayElement *elem = NULL;
    // if (_estado == GAME) {
    //   Personaje *personaje = GameManager::getSingleton().getPersonaje();
    //   if (personaje->getNodo()->getPosition().z == LIMITE_ARRIBA) {
    //     // Si hemos llegado arriba hemos pasado el nivel
    //     if (OgreFramework::getSingletonPtr()->_gameConfig.getNumLevels() == _level) {
    //       // Si no hay mas niveles se ha ganado
    //       _estado = WINNER;
    //       Records::getSingleton().add ( _level, _tiempoTotal );
    // 	        Records::getSingleton().write();
    // 	        Records::getSingleton().compacta ( 10 );
    // 	        _tiempoTotal = 0;
    //     } else {
    //       // Si no pasamos al siguiente nivel
    //       _level++;
    //       cargarNivel();
    //       personaje->volverAInicio();
    //       _estado = NEXT_LEVEL;
    //     }
    //   } else {
    //     // Movemos la escena
    //     GameManager::getSingleton().mover(timeSinceLastFrame, _tiempo);

    //     // Vemos si esta Muerto
    //     if (personaje->getEstado() == MUERTO || personaje->getEstado() == MUERTO_AHOGADO) {
    //       // Controlamos el tiempo que esta quito cuando muere
    //       if (_tiempoMuertoFin == 0.0) { // Recien muerto
    // 	          _muertoFX->play();
    //         _vidas--;
    //         actualizarVidas();
    //         _tiempoMuertoFin = _tiempo + TIEMPO_MUERTO;
    //       } else  if (_tiempoMuertoFin <= _tiempo) {
    //         _tiempoMuertoFin = 0.0;
    //         personaje->volverAInicio();
    //       }
    //     }

    //     // Actualizamos el tiempo que se esta mostrando
    //     elem = m_pOverlayMgr->getOverlayElement("txtNivel");
    //     char cad[3];
    //     sprintf ( cad, "%d", _level );
    //     elem->setCaption ( string ("Level ") + string ( cad ) );

    //     elem = m_pOverlayMgr->getOverlayElement("txtTiempo");
    //     elem->setCaption ( getTime(_tiempo) );
    //   }
    // } else if (_estado == GAME_OVER) {
    //   // Cuando se hayan acabo todas las vidas
    //   // Mostramos el panel de GameOver y el sonido correspondiente
    //   elem = m_pOverlayMgr->getOverlayElement("panelGameOver");
    //   if ( !elem->isVisible() )
    // 	    {
    // 	      _gameoverTrack->play();
    // 	      Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    // 	      overlay->hide();
    // 	      elem->show();
    // 	      if ( _level > 1 ) // Si al menos hemos pasado el primer nivel grabaremos
    // 	        {
    // 	          Records::getSingleton().add ( _level - 1, _tiempoNextLevel );
    // 	          Records::getSingleton().write();
    // 	          Records::getSingleton().compacta ( 10 );
    // 	        }
    // 	    }
    // } else if (_estado == NEXT_LEVEL) {
    //   // Cuando se hemos pasado de nivel
    //   // Mostramos el panel de NextLevel
    //   _tiempoNextLevel = _tiempoTotal;
    //   Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    //   overlay->hide();
    //   elem = m_pOverlayMgr->getOverlayElement("panelNextLevel");
    //   elem->show();
    // } else if (_estado == WINNER) {
    //   // Cuando se hayan acabo todos los niveles
    //   // Mostramos el panel de Winner y el sonido correspondiente
    //   elem = m_pOverlayMgr->getOverlayElement("panelWinner");
    //   if ( !elem->isVisible() )
    //   {
    //     _winnerTrack->play();
    //     Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    //     overlay->hide();
    //     elem->show();
    //   }
    // }

    Ogre::Vector3 vt(0,0,0);
    Ogre::Real tSpeed = 20.0;
    Ogre::Real deltaT = timeSinceLastFrame;
    bool endereza = false;

    int fps = 1.0 / deltaT;
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

    // TODO
    //    cout << "Meta: " << _vCoches[0]->isMeta(m_pSceneMgr, _world) << endl;
    
    if ( _vCoches[0]->isMeta(m_pSceneMgr, _world) )
      {
        // Si es la primera vez, significa que hemos pisado la meta pero para empezar la vuelta, por ser ésta la primera
        // no se debe de contar
        if ( _empieza_a_contar )
	  {
            cout << "mejorTiempo = " << _mejorTiempo << endl;
            cout << "tiempo = " << _tiempo << endl;
	    if ( ( _tiempo > 1 ) && ( _mejorTiempo > _tiempo || _mejorTiempo < 1 ) )
	      _mejorTiempo = _tiempo;
	  }
        if (!_estaEnMeta) _sonidoMetaFX->play(); // Cuando entra en la meta
        _tiempo = 0;

        _empieza_a_contar = true; // Solo sirve para la primera vez
        _estaEnMeta = true;
      } else {
        _estaEnMeta = false;
      }

    int posx = OgreFramework::getSingletonPtr()->getMousePtr()->getMouseState().X.abs;   // Posicion del puntero
    int posy = OgreFramework::getSingletonPtr()->getMousePtr()->getMouseState().Y.abs;   //  en pixeles.

    m_pCamera->moveRelative ( vt * deltaT * tSpeed );
    if ( m_pCamera->getPosition().length() < 10.0 )
      {
     	m_pCamera->moveRelative(-vt * deltaT * tSpeed);
      }

//    _mouse->capture();

    // Si usamos la rueda, desplazamos en Z la camara ------------------
    vt += Ogre::Vector3(0,0,-0.5)*deltaT * OgreFramework::getSingletonPtr()->getMousePtr()->getMouseState().Z.rel;
    m_pCamera->moveRelative ( vt * deltaT * tSpeed );

    // // Botones del raton pulsados? -------------------------------------
    // mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
    // mbmiddle = OgreFramework::getSingletonPtr()->getMousePtr()->getMouseState().buttonDown(OIS::MB_Middle);
    // mbright = _mouse->getMouseState().buttonDown(OIS::MB_Right);

    if ( OgreFramework::getSingletonPtr()->getMousePtr()->getMouseState().buttonDown ( OIS::MB_Middle ) )
      { // Con boton medio pulsado, rotamos camara ---------

	float rotx = OgreFramework::getSingletonPtr()->getMousePtr()->getMouseState().X.rel * deltaT * -1;
	float roty = OgreFramework::getSingletonPtr()->getMousePtr()->getMouseState().Y.rel * deltaT * -1;
	m_pCamera->yaw ( Ogre::Radian ( rotx ) );
	m_pCamera->pitch ( Ogre::Radian ( roty ) );
	Ogre::Vector3 v = m_pCamera->getPosition();
	cout << "Pos camara : x = " << v.x << ", y = " << v.y << ", z = " << v.z << endl;
      }

    // Ogre::OverlayElement *oe = NULL;

    // oe = m_pOverlayMgr->getOverlayElement("cursor");
    // oe->setLeft(posx);
    // oe->setTop(posy);

    // oe = m_pOverlayMgr->getOverlayElement("fpsInfo");
    // oe->setCaption(Ogre::StringConverter::toString(fps));

  }

void GameState::buildGUI()
  {
    // actualizarVidas();

    // Ogre::Overlay *ov = NULL;
    Ogre::OverlayElement *elem = NULL;

    elem = m_pOverlayMgr->getOverlayElement("Panel_Tiempo_Game");
    elem->show();

    elem = m_pOverlayMgr->getOverlayElement("txtMejorTiempo");
    elem->setCaption ( "Best Time:" );
    elem = m_pOverlayMgr->getOverlayElement("Panel_MejorTiempo_Game");
    elem->show();

    // elem = m_pOverlayMgr->getOverlayElement("panelNivel");
    // elem->show();

    // oe = m_pOverlayMgr->getOverlayElement("cursor");
    // oe->setLeft(posx);
    // oe->setTop(posy);

    // ov = m_pOverlayMgr->getByName("Info");

    // if ( ov )
    //   ov->show();

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

// void GameState::actualizarVidas()
//   {
//     // Ogre::OverlayElement *elem;

//     // elem = m_pOverlayMgr->getOverlayElement("panelVidas1");
//     // elem->hide();
//     // elem = m_pOverlayMgr->getOverlayElement("panelVidas2");
//     // elem->hide();
//     // elem = m_pOverlayMgr->getOverlayElement("panelVidas3");
//     // elem->hide();

//     // switch ( _vidas )
//     //   {
//     //   case 0:
//     // 	_estado = GAME_OVER;
//     //   case 2:
//     // 	elem = m_pOverlayMgr->getOverlayElement("panelVidas2"); break;
//     //   case 3:
//     // 	elem = m_pOverlayMgr->getOverlayElement("panelVidas3"); break;
//     //   default:
//     // 	elem = m_pOverlayMgr->getOverlayElement("panelVidas1"); break;
//     //   }
//     // elem->show();
//   }
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