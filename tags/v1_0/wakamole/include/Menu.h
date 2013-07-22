#include <Ogre.h>
#include <vector>
#include "MenuFrameListener.h"

class Menu {
  
private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  Ogre::RenderWindow* _win;
  Ogre::Camera* _cam;
  MenuFrameListener* _framelistener;
  Ogre::OverlayManager* _overlayManager;

  void createScene();
  void mostrarOverlay(bool mostrar);
  void mostrarOverlayRecords(bool mostrar);
public:
  Menu(Ogre::Root* root, Ogre::RenderWindow* win, 
				 Ogre::Camera* cam,
         SceneManager* sm);
  ~Menu();  
  int start();
};
