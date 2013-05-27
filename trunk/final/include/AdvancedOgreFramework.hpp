#ifndef OGRE_FRAMEWORK_HPP
#define OGRE_FRAMEWORK_HPP

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "TrackManager.h"
#include "SoundFXManager.h"

class OgreFramework : public Ogre::Singleton<OgreFramework>, OIS::KeyListener, OIS::MouseListener
  {

public:
    OgreFramework();
    ~OgreFramework();

    bool initOgre ( Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0 );
    void updateOgre ( double timeSinceLastFrame );

    bool keyPressed ( const OIS::KeyEvent &keyEventRef );
    bool keyReleased ( const OIS::KeyEvent &keyEventRef );

    bool mouseMoved ( const OIS::MouseEvent &evt );
    bool mousePressed ( const OIS::MouseEvent &evt, OIS::MouseButtonID id );
    bool mouseReleased ( const OIS::MouseEvent &evt, OIS::MouseButtonID id );


    //Para los cambios de NORMAL a FULLSCREEN y viceversa, es decir, mantener su aspect ratio
    double                      _factorX;
    double                      _factorY;

    inline Ogre::Log*                   getLogMgrPtr() { return m_pLog; };
    inline OIS::InputManager*           getInputMgrPtr() { return m_pInputMgr; };
    inline OIS::Keyboard*	        getKeyboardPtr() { return m_pKeyboard; };
    inline OIS::Mouse*		        getMousePtr() { return m_pMouse; };
    inline Ogre::Root*		        getRootPtr() { return m_pRoot; };
    inline Ogre::RenderWindow*	        getRenderWindowPtr() { return m_pRenderWnd; };
    inline Ogre::Viewport*	        getViewportPtr() { return m_pViewport; };
    inline Ogre::Timer*		        getTimerPtr() { return m_pTimer; };
    inline OgreBites::SdkTrayManager*	getSDKTrayMgrPtr() { return m_pTrayMgr; };

private:
    OgreFramework(const OgreFramework&);
    OgreFramework& operator= (const OgreFramework&);

    Ogre::Log*			m_pLog;
    OIS::InputManager*		m_pInputMgr;
    OIS::Keyboard*		m_pKeyboard;
    OIS::Mouse*			m_pMouse;
    Ogre::Root*			m_pRoot;
    Ogre::RenderWindow*		m_pRenderWnd;
    Ogre::Viewport*		m_pViewport;
    Ogre::Timer*		m_pTimer;
    OgreBites::SdkTrayManager*	m_pTrayMgr;

    //Carga de la configuración dinámica del juego (niveles)

    bool initSDL ();
};

#endif
