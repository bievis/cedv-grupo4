#include <Ogre.h>
#include "Juego.h"

class KickRoach {
  
private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  Juego* _juego;
public:
  KickRoach();
  ~KickRoach();  
  int start();
  void loadResources();
};
