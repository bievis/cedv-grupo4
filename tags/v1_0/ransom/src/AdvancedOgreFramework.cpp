#include "AdvancedOgreFramework.hpp"

using namespace Ogre;

template<> OgreFramework* Ogre::Singleton<OgreFramework>::msSingleton = 0;

#define FILE_XML "./config/config.xml"

OgreFramework::OgreFramework()
  {
    m_pRoot		= 0;
    m_pRenderWnd	= 0;
    m_pViewport		= 0;
    m_pLog		= 0;
    m_pTimer		= 0;

    m_pInputMgr		= 0;
    m_pKeyboard		= 0;
    m_pMouse		= 0;
    m_pTrayMgr          = 0;
  }

OgreFramework::~OgreFramework()
  {
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");

    if ( m_pTrayMgr )   delete m_pTrayMgr;
    if ( m_pInputMgr )	OIS::InputManager::destroyInputSystem ( m_pInputMgr );
    if ( m_pRoot )	delete m_pRoot;
  }

bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
  {
    //Ogre::LogManager* logMgr = new Ogre::LogManager();
    new Ogre::LogManager();
    //UNUSED_VARIABLE(logMgr);

    //Ejemplos para uso del log: en el cual tenemos 3 niveles : LML_TRIVIAL, LML_NORMAL, LML_CRITICAL
    // string msg = "pruebas";
    // logMgr->logMessage ( msg, LML_NORMAL );

    m_pLog = Ogre::LogManager::getSingleton().createLog ( "ransom.log", true, true, false );
    m_pLog->setDebugOutputEnabled ( true );

    m_pRoot = new Ogre::Root();

    if(!m_pRoot->restoreConfig()) {
      m_pRoot->showConfigDialog();
      m_pRoot->saveConfig();
    }
    m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

    m_pViewport = m_pRenderWnd->addViewport(0);
    m_pViewport->setBackgroundColour(ColourValue(0.f, 0.f, 0.f, 1.0f));

    m_pViewport->setCamera(0);

    unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

    paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

    m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

    m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
    m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();

    if(pKeyListener == 0)
      m_pKeyboard->setEventCallback(this);
    else
      m_pKeyboard->setEventCallback(pKeyListener);

    if(pMouseListener == 0)
      m_pMouse->setEventCallback(this);
    else
      m_pMouse->setEventCallback(pMouseListener);

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
      {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
	  {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
	  }
      }

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    //OgreBites::InputContext inputContext;
    //inputContext.mMouse = m_pMouse;
    //inputContext.mKeyboard = m_pKeyboard;
    m_pTrayMgr = new OgreBites::SdkTrayManager("AOFTrayMgr", m_pRenderWnd, m_pMouse, 0);

    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();

    m_pRenderWnd->setActive(true);

    // Inicio de los managers de sonido
    initSDL ();
    new TrackManager;
    new SoundFXManager;

    // Calculamos los fatores de escalar de los overlays
    unsigned int width; unsigned int height; unsigned int colourDepth; int left; int top;
    OgreFramework::getSingletonPtr()->m_pRenderWnd->getMetrics(width, height, colourDepth, left, top);

    _factorX = width;
    _factorY = height;

    return true;
  }

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
  {
    if ( m_pKeyboard->isKeyDown(OIS::KC_SYSRQ) )
      {
        m_pRenderWnd->writeContentsToTimestampedFile ( "Ransom_Screenshot_", ".jpg" );
        return true;
      }

    if(m_pKeyboard->isKeyDown(OIS::KC_O))
      {
        if(m_pTrayMgr->isLogoVisible())
	  {
            m_pTrayMgr->hideFrameStats();
            m_pTrayMgr->hideLogo();
	  }
        else
	  {
            m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
            m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	  }
      }

    return true;
  }

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
  {
    return true;
  }

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
  {
    return true;
  }

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
  {
    return true;
  }

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
  {
    return true;
  }

void OgreFramework::updateOgre(double timeSinceLastFrame)
  {
  }

bool OgreFramework::initSDL ()
  {
    // Inicializando SDL...
    OgreFramework::getSingletonPtr()->m_pLog->logMessage ( "Initializing SDL ..." );

    if ( SDL_Init ( SDL_INIT_AUDIO ) < 0 )
      return false;

    // Llamar a  SDL_Quit al terminar.
    atexit ( SDL_Quit );

    // Inicializando SDL mixer...
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS, 4096) < 0)
      return false;

    // Llamar a Mix_CloseAudio al terminar.
    atexit ( Mix_CloseAudio );

    OgreFramework::getSingletonPtr()->m_pLog->logMessage ( "SDL Initialization successful." );

    return true;
  }
