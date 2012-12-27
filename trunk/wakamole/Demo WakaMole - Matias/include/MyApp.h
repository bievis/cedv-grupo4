#include <Ogre.h>
#include <vector>
#include "MyFrameListener.h"
#include "Personaje.h"

class MyApp {
  
private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  MyFrameListener* _framelistener;
  Ogre::OverlayManager* _overlayManager;
  std::vector<Personaje> _personajes;
public:
  MyApp();
  ~MyApp();  
  int start();
  void loadResources();
  void createScene();
  void createOverlay();
  void createElementosMovibles();
  void createPersonaje(unsigned int numero, float posX, float posZ);
};
