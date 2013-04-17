//|||||||||||||||||||||||||||||||||||||||||||||||

#include "GameState.hpp"
#include "GameManager.h"
#include <GameConfigException.hpp>

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

GameState::GameState()
{
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
    _ptrGameConfig      = NULL;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameState...");

    _tiempo = 0;
    _level = 1;
    _vidas = 3;
    _estado = GAME;
    _tiempoMuertoFin = 0.0;

    _gameTrack = TrackManager::getSingleton().load("tema_juego.mp3");
    _gameoverTrack = TrackManager::getSingleton().load("game_over.mp3");
    _winnerTrack = TrackManager::getSingleton().load("aplausos.mp3");
    _moverPersonajeFX = SoundFXManager::getSingleton().load("movimiento.wav");
    _muertoFX = SoundFXManager::getSingleton().load("muerto.wav");

    // Reproducción del track principal...
    _gameTrack->play();

    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pCamera->setPosition(Vector3(0.0, 19, 0.1));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(5);
    m_pCamera->setFarClipDistance(10000);

    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

    m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

    _ptrGameConfig = &(OgreFramework::getSingletonPtr()->_gameConfig);

    buildGUI();

    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing GameState...");

    Ogre::OverlayElement *elem;
 
    elem = m_pOverlayMgr->getOverlayElement("panelVidas3");
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelVidas2");
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelVidas1");
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelTiempo");
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelNivel");
    elem->hide();

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming GameState...");

    buildGUI();

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_bQuit = false;

    _gameTrack->play();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState...");

    // Parar del track principal...
    _gameTrack->stop();
    _gameoverTrack->stop();
    _winnerTrack->stop();

    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);

    GameManager::getSingleton().limpiar ();
    
    // Ocultar overlays
    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    overlay->hide();

    Ogre::OverlayElement *elem;
    elem = m_pOverlayMgr->getOverlayElement("panelVidas2");
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelVidas1");
    elem->hide();
    
    overlay = m_pOverlayMgr->getByName("Screens_Game");
    overlay->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelWinner");
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelNextLevel");
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelGameOver");
    elem->hide();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::createScene()
{
    // Creamos los objetos de la escena
    m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);	
    m_pSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));

    m_pSceneMgr->setShadowTextureCount(2);
    m_pSceneMgr->setShadowTextureSize(512);

    Entity* entSuelo = m_pSceneMgr->createEntity("Suelo", "Suelo.mesh");
    
    // Creamos el escenario como geometria estatica
    StaticGeometry* escenario = m_pSceneMgr->createStaticGeometry("EscenarioEstatico");
    escenario->addEntity(entSuelo, Vector3(0,0,0));
    escenario->build();  // Operacion para construir la geometria

    // Luz de la escena
    Light* luz = m_pSceneMgr->createLight("Luz");
    luz->setType(Light::LT_POINT);
    luz->setPosition(75,75,75);
    luz->setSpecularColour(1, 1, 1); 
    luz->setDiffuseColour(1, 1, 1);

    // Partes del escenario

    try 
      {
        LoadScenaryParts();
      }
    catch ( GameConfigException& exc )
      {
    	cerr << "EXCEPTION:: " << exc.what() << endl;
      }

    // Cargamos el personaje
    SceneNode* nodePersonaje = GameManager::getSingleton().
          crearNodo(m_pSceneMgr, "Marciano", "Marciano.mesh", 0, 0, 6.5);
    SceneNode* nodePersonajeMuerto = GameManager::getSingleton().
          crearNodo(m_pSceneMgr, "MarcianoMuerto", "MarcianoMuerto.mesh", 0, 0, 6.5);
    SceneNode* nodePersonajeEstrellas = GameManager::getSingleton().
          crearNodo(m_pSceneMgr, "Estrellas", "Estrellas.mesh", 0, 0, 6.5);
    Personaje* p = new Personaje("Marciano", nodePersonaje, nodePersonajeMuerto, nodePersonajeEstrellas);
    GameManager::getSingleton().setPersonaje(p);

    //Cargamos overlay con la GUI ( Vidas[Los 3 tipos] + Tiempo + Nivel )
    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    overlay->show();

    Ogre::OverlayElement *elem;
    elem = m_pOverlayMgr->getOverlayElement("panelVidas2");
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelVidas1");
    elem->hide();

    //Cargamos las pantallas de Winner, Next Level y Game Over
    overlay = m_pOverlayMgr->getByName("Screens_Game");
    overlay->show();

    elem = m_pOverlayMgr->getOverlayElement("panelWinner");
    elem->setDimensions(OgreFramework::getSingletonPtr()->_factorX, OgreFramework::getSingletonPtr()->_factorY);
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelNextLevel");
    elem->setDimensions (OgreFramework::getSingletonPtr()->_factorX, OgreFramework::getSingletonPtr()->_factorY);
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelGameOver");
    elem->setDimensions(OgreFramework::getSingletonPtr()->_factorX, OgreFramework::getSingletonPtr()->_factorY);
    elem->hide();
}

void GameState::LoadScenaryParts()
  {

    SceneNode* nodoZonaCrater = GameManager::getSingleton().
          crearNodo(m_pSceneMgr, "Abismo", "Abismo.mesh", 0.0, 0.00001, -4.00000);
    ParteEscenario* zona_crater = new ParteEscenario ( nodoZonaCrater->getName(), nodoZonaCrater, AGUA );
    GameManager::getSingleton().addParteEscenario ( zona_crater );

    SceneNode* nodoZonaCarretera = GameManager::getSingleton().
          crearNodo(m_pSceneMgr, "Carretera", "Carretera.mesh", 0.0, 0.00001, 4.00000);
    ParteEscenario* zona_carretera = new ParteEscenario ( nodoZonaCarretera->getName(), nodoZonaCarretera, CARRETERA );
    GameManager::getSingleton().addParteEscenario ( zona_carretera );

    // Cargamos el primer nivel
    cargarNivel();
}

///////////////////////////////////7

void GameState::cargarNivel() {
    // Limpiamos el nivel anterior
    GameManager::getSingleton().limpiarPartes();

    // Construimos los carriles
    Level lvl;
    Row row;

    _ptrGameConfig->getLevel ( _level, lvl );

    double zona_crater_Z[] = { -2.5, -4, -5.5 };
    double zona_carretera_Z[] = {  2,  3.5,  5 };

    ParteEscenario* parte = NULL;
    
    std::vector<ParteEscenario*>::const_iterator
      mit (GameManager::getSingleton().getPartesEscenario().begin()),
      mend(GameManager::getSingleton().getPartesEscenario().end());
    for(;mit!=mend;++mit) {
      parte = (*mit);
      
      if (parte->getTipo() == AGUA) {
        // Zona crater
        for ( unsigned int i = 0; i < lvl.getCrater().getRows(); i++ )
        {
          lvl.getCrater().getRow ( i, row );
          parte->addCarril ( row.get_name().c_str(), row.get_speed(), row.get_distance(), (row.get_way()==RIGHT)?DIR_DER:DIR_IZQ, zona_crater_Z[i%3], m_pSceneMgr );
          parte->addModeloElementoCarril ( row.get_name().c_str(), "Plataforma.mesh" );
        }
      } else if (parte->getTipo() == CARRETERA) {
        // Zona carretera
        for ( unsigned int i = 0; i < lvl.getRoad().getRows(); i++ )
        {
          lvl.getRoad().getRow ( i, row );
          parte->addCarril ( row.get_name().c_str(), row.get_speed(), row.get_distance(), (row.get_way()==RIGHT)?DIR_DER:DIR_IZQ, zona_carretera_Z[i%3], m_pSceneMgr );
          parte->addModeloElementoCarril ( row.get_name().c_str(), "Meteorito.mesh" );
          parte->addModeloElementoCarril ( row.get_name().c_str(), "Nave1.mesh" );
          parte->addModeloElementoCarril ( row.get_name().c_str(), "Nave2.mesh" );
        }
      }
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{    
    if (_estado == GAME_OVER || _estado == WINNER) {
      popAllAndPushAppState(findByName("MenuState"));
      return true;
    } else if (_estado == NEXT_LEVEL) {
      Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
      overlay->show();
      Ogre::OverlayElement *elem = m_pOverlayMgr->getOverlayElement("panelNextLevel");
      elem->hide();
      _tiempo = 0;
      _estado = GAME;
      return true;
    }
    else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
      pushAppState(findByName("PauseState"));
      return true;
    } else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_UP))
    {
      _moverPersonajeFX->play();
      GameManager::getSingleton().getPersonaje()->setMovimiento(DELANTE);
    } else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_DOWN))
    {
      _moverPersonajeFX->play();
      GameManager::getSingleton().getPersonaje()->setMovimiento(ATRAS);
    } else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LEFT))
    {
      _moverPersonajeFX->play();
      GameManager::getSingleton().getPersonaje()->setMovimiento(IZQUIERDA);
    } else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
    {
      _moverPersonajeFX->play();
      GameManager::getSingleton().getPersonaje()->setMovimiento(DERECHA);
    }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;

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

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;

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

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::onLeftPressed(const OIS::MouseEvent &evt)
{

    
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::update(double timeSinceLastFrame)
{
    _tiempo += timeSinceLastFrame;
    _tiempoTotal += timeSinceLastFrame;
    
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
    Ogre::OverlayElement *elem = NULL;
    if (_estado == GAME) {
      Personaje *personaje = GameManager::getSingleton().getPersonaje();
      if (personaje->getNodo()->getPosition().z == LIMITE_ARRIBA) {
        // Si hemos llegado arriba hemos pasado el nivel
        if (OgreFramework::getSingletonPtr()->_gameConfig.getNumLevels() == _level) {
          // Si no hay mas niveles se ha ganado
          _estado = WINNER;
          Records::getSingleton().add ( _level, _tiempoTotal );
	        Records::getSingleton().write();
	        Records::getSingleton().compacta ( 10 );
	        _tiempoTotal = 0;
        } else {
          // Si no pasamos al siguiente nivel
          _level++;
          cargarNivel();
          personaje->volverAInicio();
          _estado = NEXT_LEVEL;
        }
      } else {
        // Movemos la escena
        GameManager::getSingleton().mover(timeSinceLastFrame, _tiempo);    
      
        // Vemos si esta Muerto        
        if (personaje->getEstado() == MUERTO || personaje->getEstado() == MUERTO_AHOGADO) {
          // Controlamos el tiempo que esta quito cuando muere
          if (_tiempoMuertoFin == 0.0) { // Recien muerto
	          _muertoFX->play();
            _vidas--;
            actualizarVidas();
            _tiempoMuertoFin = _tiempo + TIEMPO_MUERTO;
          } else  if (_tiempoMuertoFin <= _tiempo) {
            _tiempoMuertoFin = 0.0;
            personaje->volverAInicio();
          } 
        }

        // Actualizamos el tiempo que se esta mostrando
        elem = m_pOverlayMgr->getOverlayElement("txtNivel");
        char cad[3];
        sprintf ( cad, "%d", _level );
        elem->setCaption ( string ("Level ") + string ( cad ) );

        elem = m_pOverlayMgr->getOverlayElement("txtTiempo");
        elem->setCaption ( getTime() );
      }
    } else if (_estado == GAME_OVER) {
      // Cuando se hayan acabo todas las vidas
      // Mostramos el panel de GameOver y el sonido correspondiente
      elem = m_pOverlayMgr->getOverlayElement("panelGameOver");
      if ( !elem->isVisible() )
	    {
	      _gameoverTrack->play();
	      Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
	      overlay->hide();
	      elem->show();
	      if ( _level > 1 ) // Si al menos hemos pasado el primer nivel grabaremos
	        {
	          Records::getSingleton().add ( _level - 1, _tiempoNextLevel );
	          Records::getSingleton().write();
	          Records::getSingleton().compacta ( 10 );
	        }
	    }
    } else if (_estado == NEXT_LEVEL) {
      // Cuando se hemos pasado de nivel
      // Mostramos el panel de NextLevel
      _tiempoNextLevel = _tiempoTotal;
      Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
      overlay->hide();
      elem = m_pOverlayMgr->getOverlayElement("panelNextLevel");
      elem->show();
    } else if (_estado == WINNER) {
      // Cuando se hayan acabo todos los niveles
      // Mostramos el panel de Winner y el sonido correspondiente
      elem = m_pOverlayMgr->getOverlayElement("panelWinner");
      if ( !elem->isVisible() )
      {
        _winnerTrack->play();
        Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
        overlay->hide();
        elem->show();
      }
    }    
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::buildGUI()
{
    actualizarVidas();

    Ogre::OverlayElement *elem;

    elem = m_pOverlayMgr->getOverlayElement("panelTiempo");
    elem->show();

    elem = m_pOverlayMgr->getOverlayElement("panelNivel");
    elem->show();

    OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
}

string GameState::getTime()
{
  unsigned int minutos = 0, segundos = 0;
  char cad[6];
  string ret = "";

  minutos = (int)_tiempo / 60;
  segundos = (int)_tiempo % 60;

  sprintf ( cad, "%02d:%02d", minutos, segundos );

  ret = cad;

  return ret;
}

void GameState::actualizarVidas() {
  Ogre::OverlayElement *elem;
  
  elem = m_pOverlayMgr->getOverlayElement("panelVidas1");
  elem->hide();
  elem = m_pOverlayMgr->getOverlayElement("panelVidas2");
  elem->hide();
  elem = m_pOverlayMgr->getOverlayElement("panelVidas3");  
  elem->hide();
  
  switch ( _vidas )
  {
    case 0:
      _estado = GAME_OVER;
    case 2:
      elem = m_pOverlayMgr->getOverlayElement("panelVidas2"); break;
    case 3:
      elem = m_pOverlayMgr->getOverlayElement("panelVidas3"); break;
    default:
      elem = m_pOverlayMgr->getOverlayElement("panelVidas1"); break;
  }
  elem->show();
}