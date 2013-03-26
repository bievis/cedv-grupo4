#include "NitroApp.hpp"

#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "Records.h"

NitroApp::NitroApp()
  {
    m_pAppStateManager = 0;
    Records::getSingleton().read();
  }

NitroApp::~NitroApp()
  {
    delete m_pAppStateManager;
    delete OgreFramework::getSingletonPtr();
  }

void NitroApp::start()
  {
    new OgreFramework();

    if ( !OgreFramework::getSingletonPtr()->initOgre ( "Nitro", 0, 0 ) )
      return;

    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Nitro initialized!");

    m_pAppStateManager = new AppStateManager();

    MenuState::create(m_pAppStateManager, "MenuState");
    GameState::create(m_pAppStateManager, "GameState");
    PauseState::create(m_pAppStateManager, "PauseState");

    m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
  }
