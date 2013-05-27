#include "RansomApp.hpp"

#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
// #include "Records.h"

RansomApp::RansomApp()
  {
    m_pAppStateManager = 0;
//    Records::getSingleton().read();
  }

RansomApp::~RansomApp()
  {
    delete m_pAppStateManager;
    delete OgreFramework::getSingletonPtr();
  }

void RansomApp::start()
  {
    new OgreFramework();

    if ( !OgreFramework::getSingletonPtr()->initOgre ( "Ransom", 0, 0 ) )
      return;

    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Ransom initialized!");

    m_pAppStateManager = new AppStateManager();

    MenuState::create(m_pAppStateManager, "MenuState");
    GameState::create(m_pAppStateManager, "GameState");
    PauseState::create(m_pAppStateManager, "PauseState");

    m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
  }
