#ifndef __MENU_STATE_HPP__
#define __MENU_STATE_HPP__

#include "AppState.hpp"
#include "TrackManager.h"
#include "SoundFXManager.h"
//#include "Records.h"
#include "Utilities.h"

class MenuState : public AppState
  {

    enum eScreens {
      CREDITS_SCREEN,
      HIGHSCORES_SCREEN
    };

public:
    MenuState();

    DECLARE_APPSTATE_CLASS(MenuState)

    void enter();
    void createMenuScene();
    void createButtons();
    void showButtons();
    void hideButtons();
    void exit();

    bool keyPressed ( const OIS::KeyEvent &keyEventRef );
    bool keyReleased ( const OIS::KeyEvent &keyEventRef );

    bool mouseMoved ( const OIS::MouseEvent &evt );
    bool mousePressed ( const OIS::MouseEvent &evt, OIS::MouseButtonID id );
    bool mouseReleased ( const OIS::MouseEvent &evt, OIS::MouseButtonID id );

    void buttonHit ( OgreBites::Button* button );

    void update ( double timeSinceLastFrame );

    void show_screen ( eScreens scr, bool visible );
    void inputbox ( bool visible );

    // void muestra_highscores();
    // void refresca_highscores();

private:
    bool                        m_bQuit;
    Ogre::OverlayManager*       m_pOverlayMgr;

    TrackPtr                    _menuTrack;
    SoundFXPtr                  _menuFX;

    bool                        _mostradoCreditos; // Nos dice si se estan mostrando los creditos o no ahora
    bool                        _mostradoHighScores; // Nos dice si se estan mostrando los highscores o no ahora
    bool                        _show_inputbox;

    // string                      _msg_highscore;  //Este string contiene los registros de los records a la hora de invocar al refresca_highscores()

  };

#endif