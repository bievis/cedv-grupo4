#include <Ogre.h>
#include "Juego.h"
#include "Menu.h"

class KickRoach {
  
private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  Juego* _juego;
  Menu* _menu;
public:
  KickRoach();
  ~KickRoach();  
  int start();
  void loadResources();
  void limpiarEscena();
};
