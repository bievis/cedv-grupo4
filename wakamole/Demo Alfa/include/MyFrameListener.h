#include <Ogre.h>
#include <OIS/OIS.h>
using namespace std;
using namespace Ogre;


class MyFrameListener : public FrameListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  Camera* _camera;
  SceneNode* _node;
  RenderWindow* _win;
  SceneManager* _sceneManager;
  OIS::Mouse* _mouse;
  OverlayManager* _overlayManager;
  RaySceneQuery *_raySceneQuery;
  SceneNode *_selectedNode;

  Ray setRayQuery(int posx, int posy, uint32 mask);

public:
  MyFrameListener(RenderWindow* win, Camera* cam, 
		  SceneNode* node, OverlayManager* om, SceneManager* sm);
  ~MyFrameListener();
  bool frameStarted(const FrameEvent& evt);  
};
