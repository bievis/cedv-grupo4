#ifndef __MENU_STATE_HPP__
#define __MENU_STATE_HPP__

#include "AppState.hpp"
#include "TrackManager.h"
#include "SoundFXManager.h"
#include "Records.h"
#include "Utilities.h"

/// \brief This class manage the Menu State
class MenuState : public AppState
  {
    /// \brief differents screen to view
    enum eScreens {
      CREDITS_SCREEN,
      HIGHSCORES_SCREEN
    };

public:
    /// \brief default constructor
    MenuState();

    DECLARE_APPSTATE_CLASS(MenuState)

    /// \brief this method was performed at enter of this app state
    void enter();
    /// \brief this method was performed at exit of this app state
    void exit();
    /// \brief method to create menu scene
    void createMenuScene();
    /// \brief method to create menu buttons
    void createButtons();
    /// \brief method to show buttons
    void showButtons();
    /// \brief method to hide buttons
    void hideButtons();
    /// \brief this method was performed when the key was pressed
    /// \param keyEventRef key event received
    bool keyPressed ( const OIS::KeyEvent &keyEventRef );
    /// \brief this method was performed when the key was released
    /// \param keyEventRef key event received
    bool keyReleased ( const OIS::KeyEvent &keyEventRef );
    /// \brief this method was performed when the mouse was moved
    /// \param arg mouse event received
    bool mouseMoved ( const OIS::MouseEvent &evt );
    /// \brief this method was performed when the mouse button was pressed
    /// \param arg mouse event received
    /// \param id mouse button identifier
    bool mousePressed ( const OIS::MouseEvent &evt, OIS::MouseButtonID id );
    /// \brief this method was performed when the mouse button was released
    /// \param arg mouse event received
    /// \param id mouse button identifier
    bool mouseReleased ( const OIS::MouseEvent &evt, OIS::MouseButtonID id );
    /// \brief method to manage the button pressed in the menu
    void buttonHit ( OgreBites::Button* button );
    /// \brief this method will be run every frame
    void update ( double timeSinceLastFrame );
    /// \brief method to show/hide the screen select in the parameter
    /// \param scr screen to show/hide
    /// \param visible true/false to show/hide the screen
    void show_screen ( eScreens scr, bool visible );
    /// \brief method to show highscores in the highscores screen
    void muestra_highscores();
    /// \brief method to update the highscores
    void refresca_highscores();

private:
    /// \brief this attribute will be true when we found to exit the state
    bool                        m_bQuit;
    /// \brief reference to overlay manager
    Ogre::OverlayManager*       m_pOverlayMgr;
    /// \brief soundtrack played during the menu screen
    TrackPtr                    _menuTrack;
    /// \brief sound fx played when the button was pushed
    SoundFXPtr                  _menuFX;
    /// \brief this attribute tell us if the credit screen was showed
    bool                        _mostradoCreditos;
    /// \brief this attribute tell us if the highscores screen was showed
    bool                        _mostradoHighScores;
    /// \brief message with all the highscores to show in the screen
    string                      _msg_highscore;

  };

#endif
