#include <Ogre.h>
#include <OIS/OIS.h>

using namespace std;
using namespace Ogre;

#define ESCENARIO 1 << 0  // Mascara para el escenario
#define MENU 1 << 1  // Mascara para los opciones de menu
#define TIEMPO_LANZAR_PERSONAJE 1 // Tiempo que cada cuanto se saca un personaje

class MenuFrameListener : public FrameListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  Camera* _camera;
  RenderWindow* _win;
  SceneManager* _sceneManager;
  OIS::Mouse* _mouse;
  OverlayManager* _overlayManager;
  RaySceneQuery *_raySceneQuery;
  SceneNode *_selectedNode;

  Ray setRayQuery(int posx, int posy, uint32 mask);
public:
  MenuFrameListener(Ogre::RenderWindow* win, 
				 Ogre::Camera* cam,  
				 Ogre::OverlayManager *om,
         SceneManager* sm);
  ~MenuFrameListener();
  bool frameStarted(const FrameEvent& evt); 
};
