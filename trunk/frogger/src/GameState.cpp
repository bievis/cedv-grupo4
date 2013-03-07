//|||||||||||||||||||||||||||||||||||||||||||||||

#include "GameState.hpp"
#include "GameManager.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

GameState::GameState()
{
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::enter()
{
    _tiempo = 0;
    _level = "1";
    _vidas = 3;

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameState...");

    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pCamera->setPosition(Vector3(0, 11.00000, 2.50000));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setFOVy(Degree(93.695));
    m_pCamera->setNearClipDistance(1);
    m_pCamera->setFarClipDistance(100000);

    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

    m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

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
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState...");

    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);

    GameManager::getSingleton().limpiar ();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::createScene()
{
    // Creamos los objetos de la escena
    m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);	
    m_pSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));

    m_pSceneMgr->setShadowTextureCount(2);
    m_pSceneMgr->setShadowTextureSize(512);

    Entity* entCesped = m_pSceneMgr->createEntity("Cesped", "Cesped.mesh");
    
    // Creamos el escenario como geometria estatica
    StaticGeometry* escenario = m_pSceneMgr->createStaticGeometry("EscenarioEstatico");
    escenario->addEntity(entCesped, Vector3(0,0,0));
    escenario->build();  // Operacion para construir la geometria

    // Luz de la escena
    Light* luz = m_pSceneMgr->createLight("Luz");
    luz->setType(Light::LT_POINT);
    luz->setPosition(75,75,75);
    luz->setSpecularColour(1, 1, 1); 
    luz->setDiffuseColour(1, 1, 1);

    // Partes del escenario
    SceneNode* nodeParte1 = GameManager::getSingleton().
          crearNodo(m_pSceneMgr, "Rio", "Rio.mesh", 0.0, 0.00100, -3.00000);
    ParteEscenario* parte1 = new ParteEscenario(nodeParte1->getName(), nodeParte1, AGUA);
    GameManager::getSingleton().addParteEscenario (parte1);

    SceneNode* nodeParte2 = GameManager::getSingleton().
          crearNodo(m_pSceneMgr, "Carretera", "Carretera.mesh", 0.0, 0.00100, 3.00000);
    ParteEscenario* parte2 = new ParteEscenario(nodeParte2->getName(), nodeParte2, CARRETERA);
    GameManager::getSingleton().addParteEscenario (parte2);
    // Construimos los carriles
    parte1->addCarril("Carril1", 2, 12, DIR_DER, -1.5, m_pSceneMgr);
    parte1->addModeloElementoCarril("Carril1", "Tronco.mesh");
    parte1->addCarril("Carril2", 1.5, 10, DIR_IZQ, -3, m_pSceneMgr);
    parte1->addModeloElementoCarril("Carril2", "Tronco.mesh");
    parte1->addCarril("Carril3", 1, 15, DIR_DER, -4.5, m_pSceneMgr);
    parte1->addModeloElementoCarril("Carril3", "Tronco.mesh");
    parte2->addCarril("Carril1", 3, 6, DIR_IZQ, 1.5, m_pSceneMgr);
    parte2->addModeloElementoCarril("Carril1", "Coche.mesh");
    parte2->addCarril("Carril2", 2, 10, DIR_DER, 3, m_pSceneMgr);
    parte2->addModeloElementoCarril("Carril2", "Coche.mesh");
    parte2->addCarril("Carril3", 1.5, 8, DIR_IZQ, 4.5, m_pSceneMgr);
    parte2->addModeloElementoCarril("Carril3", "Coche.mesh");

    // Cargamos el personaje
    SceneNode* nodePersonaje = GameManager::getSingleton().
          crearNodo(m_pSceneMgr, "Personaje", "Personaje.mesh", 0, 0, 6.0);
    Personaje* p = new Personaje("Personaje", nodePersonaje);
    GameManager::getSingleton().setPersonaje(p);

    //Cargamos overlay con la GUI
    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Game");
    overlay->show();

    Ogre::OverlayElement *elem;
    elem = m_pOverlayMgr->getOverlayElement("panelVidas2");
    elem->hide();

    elem = m_pOverlayMgr->getOverlayElement("panelVidas1");
    elem->hide();

    // Ogre::OverlayElement *elem;

    // elem = m_pOverlayMgr->getOverlayElement("txtNivel");
    // elem->setCaption ( string ("Level ") + _level );

    // elem = m_pOverlayMgr->getOverlayElement("txtTiempo");
    // elem->setCaption ( getTime() );

}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        pushAppState(findByName("PauseState"));
        return true;
    } else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_UP))
    {
      GameManager::getSingleton().getPersonaje()->setMovimiento(DELANTE);
    } else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_DOWN))
    {
      GameManager::getSingleton().getPersonaje()->setMovimiento(ATRAS);
    } else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LEFT))
    {
      GameManager::getSingleton().getPersonaje()->setMovimiento(IZQUIERDA);
    } else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
    {
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
    // static double tiempo = 0;
    _tiempo += timeSinceLastFrame;
    
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

    if(m_bQuit == true)
    {
        popAppState();
        return;
    }

    GameManager::getSingleton().mover(timeSinceLastFrame, _tiempo);

    Ogre::OverlayElement *elem;

    elem = m_pOverlayMgr->getOverlayElement("txtNivel");
    elem->setCaption ( string ("Level ") + _level );

    elem = m_pOverlayMgr->getOverlayElement("txtTiempo");
    elem->setCaption ( getTime() );
    
  // Ogre::OverlayElement *oe;
  // int fps = 1.0 / timeSinceLastFrame;
  // oe = m_pOverlayMgr->getOverlayElement("fpsInfo");
  // oe->setCaption(Ogre::StringConverter::toString(fps));
  // oe = m_pOverlayMgr->getOverlayElement("camPosInfo");
  // oe->setCaption(Ogre::StringConverter::toString(m_pCamera->getPosition()));
  // oe = m_pOverlayMgr->getOverlayElement("camRotInfo");
  // oe->setCaption(Ogre::StringConverter::toString(m_pCamera->getDirection()));
  // oe = m_pOverlayMgr->getOverlayElement("modRotInfo");
  // oe->setCaption(Ogre::String("RotZ: 10"));

}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::buildGUI()
{
    Ogre::OverlayElement *elem;

    switch ( _vidas )
      {
        case 2:
	  elem = m_pOverlayMgr->getOverlayElement("panelVidas2"); break;
        case 3:
          elem = m_pOverlayMgr->getOverlayElement("panelVidas3"); break;
        default:
          elem = m_pOverlayMgr->getOverlayElement("panelVidas1"); break;
      }

    elem->show();

    elem = m_pOverlayMgr->getOverlayElement("panelTiempo");
    elem->show();

    elem = m_pOverlayMgr->getOverlayElement("panelNivel");
    elem->show();

    OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
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

