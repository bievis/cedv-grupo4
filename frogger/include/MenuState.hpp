#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "AppState.hpp"
#include "TrackManager.h"
#include "SoundFXManager.h"

class MenuState : public AppState
  {
public:
    MenuState();

    DECLARE_APPSTATE_CLASS(MenuState)

    void enter();
    void createScene();
    void exit();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

    void buttonHit(OgreBites::Button* button);

    void update(double timeSinceLastFrame);
private:
    TrackPtr _mainTrack;
    SoundFXPtr _menuFX;
    bool m_bQuit;
    Ogre::OverlayManager* m_pOverlayMgr;
};

#endif
