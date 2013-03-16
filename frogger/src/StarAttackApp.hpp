#ifndef _STAR_ATTACK_APP_HPP
#define _STAR_ATTACk_APP_HPP

#include "AdvancedOgreFramework.hpp"
#include "AppStateManager.hpp"

class StarAttackApp
  {
public:
    StarAttackApp();
    ~StarAttackApp();

    void start();

private:
    AppStateManager* m_pAppStateManager;
};

#endif
