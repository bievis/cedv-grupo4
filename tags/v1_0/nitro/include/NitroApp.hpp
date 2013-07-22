#ifndef _NITRO_APP_HPP
#define _NITRO_APP_HPP

#include "AdvancedOgreFramework.hpp"
#include "AppStateManager.hpp"

class NitroApp
  {

public:
    NitroApp();
    ~NitroApp();

    void start();

private:
    AppStateManager* m_pAppStateManager;

  };

#endif
