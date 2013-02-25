#ifndef _FROGGER_APP_HPP
#define _FROGGER_APP_HPP

#include "AdvancedOgreFramework.hpp"
#include "AppStateManager.hpp"

class FroggerApp
  {
public:
    FroggerApp();
    ~FroggerApp();

    void start();

private:
    AppStateManager* m_pAppStateManager;
};

#endif
