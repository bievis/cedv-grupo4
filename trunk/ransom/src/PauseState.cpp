#include "PauseState.hpp"

using namespace Ogre;

PauseState::PauseState()
  {
    m_bQuit             = false;
    m_bQuestionActive   = false;
    m_FrameEvent        = Ogre::FrameEvent();
  }

void PauseState::enter()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Entering PauseState...");

    // Carga del sonido.
    _menuFX = SoundFXManager::getSingleton().load("boton.wav");

    m_pSceneMgr = OgreFramework::getSingletonPtr()->getRootPtr()->createSceneManager(ST_GENERIC, "PauseSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    m_pCamera = m_pSceneMgr->createCamera("PauseCam");
    m_pCamera->setPosition(Vector3(0, 25, -50));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(1);

    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->getViewportPtr()->getActualWidth()) /
			      Real(OgreFramework::getSingletonPtr()->getViewportPtr()->getActualHeight()));

    OgreFramework::getSingletonPtr()->getViewportPtr()->setCamera(m_pCamera);

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->showCursor();
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->createButton(OgreBites::TL_CENTER, "BackToGameBtn", "Resume Game", 350);
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->createButton(OgreBites::TL_CENTER, "BackToMenuBtn", "Return to Main Menu", 350);
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit Game", 350);

    m_bQuit = false;
  }

void PauseState::exit()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Leaving PauseState...");

    // Parar del track principal...
//    this->_mainTrack->stop();

    m_pSceneMgr->destroyCamera(m_pCamera);
    if ( m_pSceneMgr )
      OgreFramework::getSingletonPtr()->getRootPtr()->destroySceneManager ( m_pSceneMgr );

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->clearAllTrays();
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->setListener(0);
  }

bool PauseState::keyPressed(const OIS::KeyEvent &keyEventRef)
  {
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_ESCAPE ) && !m_bQuestionActive )
      {
        m_bQuit = true;
        return true;
      }

    OgreFramework::getSingletonPtr()->keyPressed ( keyEventRef );

    return true;
  }

bool PauseState::keyReleased(const OIS::KeyEvent &keyEventRef)
  {
    OgreFramework::getSingletonPtr()->keyReleased ( keyEventRef );

    return true;
  }

bool PauseState::mouseMoved(const OIS::MouseEvent &evt)
  {
    if ( OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->injectMouseMove ( evt ) )
      return true;

    return true;
  }

bool PauseState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
  {
    if ( OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->injectMouseDown ( evt, id ) )
      return true;

    return true;
  }

bool PauseState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
  {
    if ( OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->injectMouseUp ( evt, id ) )
      return true;

    return true;
  }

void PauseState::update(double timeSinceLastFrame)
  {
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->frameRenderingQueued ( m_FrameEvent );

    if ( m_bQuit == true )
      {
        popAppState();
        return;
      }
  }

void PauseState::buttonHit(OgreBites::Button *button)
  {
    _menuFX->play();
    if ( button->getName() == "ExitBtn" )
      {
        OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->showYesNoDialog ( "Really?", "Are you sure to abort the game?" );
        m_bQuestionActive = true;
      }
    else if ( button->getName() == "BackToGameBtn" )
      m_bQuit = true;
    else if ( button->getName() == "BackToMenuBtn" )
      popAllAndPushAppState(findByName("MenuState"));
  }

void PauseState::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
  {
    if ( yesHit == true )
      shutdown();
    else
      OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->closeDialog();

    m_bQuestionActive = false;
  }
