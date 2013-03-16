#include "StarAttackApp.hpp"

#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "Records.h"

StarAttackApp::StarAttackApp()
  {
    m_pAppStateManager = 0;
    Records::getSingleton().read();
  }

StarAttackApp::~StarAttackApp()
  {
    delete m_pAppStateManager;
    delete OgreFramework::getSingletonPtr();
  }

void StarAttackApp::start()
  {
    new OgreFramework();

    if ( !OgreFramework::getSingletonPtr()->initOgre ( "Star Attack", 0, 0 ) )
      return;


    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Star Attack initialized!");

    m_pAppStateManager = new AppStateManager();

    MenuState::create(m_pAppStateManager, "MenuState");
    GameState::create(m_pAppStateManager, "GameState");
    PauseState::create(m_pAppStateManager, "PauseState");

    m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
  }
