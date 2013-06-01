#ifndef _RANSOM_APP_HPP
#define _RANSOM_APP_HPP

#include "AdvancedOgreFramework.hpp"
#include "AppStateManager.hpp"

class RansomApp
  {

public:
    RansomApp();
    ~RansomApp();

    void start();

private:
    AppStateManager* m_pAppStateManager;

  };

#endif
