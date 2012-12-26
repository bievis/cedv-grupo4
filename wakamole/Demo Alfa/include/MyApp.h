#include <Ogre.h>
#include <vector>
#include "MyFrameListener.h"

using namespace std;

//Numero de objetos que se van a mover en pantalla
#define _NUM_ELEMS_ 7

class MyApp {
  
private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  MyFrameListener* _framelistener;
  Ogre::OverlayManager* _overlayManager;
  std::vector<SceneNode*> vNodes;
  
public:
  MyApp();
  ~MyApp();  
  int start();
  void loadResources();
  void createScene();
  void createOverlay();
};
