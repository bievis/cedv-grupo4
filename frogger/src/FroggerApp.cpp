#include "FroggerApp.hpp"

#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"

FroggerApp::FroggerApp()
  {
    m_pAppStateManager = 0;
  }

FroggerApp::~FroggerApp()
  {
    delete m_pAppStateManager;
    delete OgreFramework::getSingletonPtr();
  }

void FroggerApp::start()
  {
    new OgreFramework();

    if ( !OgreFramework::getSingletonPtr()->initOgre ( "Frogger", 0, 0 ) )
      return;

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Frogger initialized!");

    m_pAppStateManager = new AppStateManager();

    MenuState::create(m_pAppStateManager, "MenuState");
    GameState::create(m_pAppStateManager, "GameState");
    PauseState::create(m_pAppStateManager, "PauseState");

    m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
  }
