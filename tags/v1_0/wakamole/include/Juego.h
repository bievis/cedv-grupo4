#include <Ogre.h>
#include <vector>
#include "JuegoFrameListener.h"
#include "Personaje.h"

class Juego {
  
private:
  Ogre::SceneManager* _sceneManager;
  Ogre::Root* _root;
  Ogre::RenderWindow* _win;
  Ogre::Camera* _cam;
  JuegoFrameListener* _framelistener;
  Ogre::OverlayManager* _overlayManager;
  std::vector<Personaje> _personajes;

  void createScene();
  void mostrarOverlay(bool mostrar);
  void createElementosMovibles();
  void createPersonaje(unsigned int numero, float posX, float posZ);
public:
  Juego(Ogre::Root* root, Ogre::RenderWindow* win, 
				 Ogre::Camera* cam,
         SceneManager* sm);
  ~Juego();  
  int start();
};
