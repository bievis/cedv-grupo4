#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

#include "AppState.hpp"
#include "SoundFXManager.h"

/// \brief This class manage the Pause State
class PauseState : public AppState
{
public:
    /// \brief default constructor
    PauseState();

    DECLARE_APPSTATE_CLASS(PauseState)

    /// \brief this method was performed at enter of this app state
    void enter();
    /// \brief this method was performed at exit of this app state
    void exit();
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
    /// \brief method to show the dialog with yes/no response
    void yesNoDialogClosed ( const Ogre::DisplayString& question, bool yesHit );
    /// \brief this method will be run every frame
    void update ( double timeSinceLastFrame );

private:
    /// \brief this attribute will be true when we found to exit the state
    bool                        m_bQuit;
    /// \brief this attribute will be true if the dialog yes/no was showed
    bool                        m_bQuestionActive;
    /// \brief sound fx played when the button was pushed
    SoundFXPtr                  _menuFX;
};

#endif
