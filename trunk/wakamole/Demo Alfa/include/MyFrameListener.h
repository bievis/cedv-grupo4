#include <Ogre.h>
#include <OIS/OIS.h>
#include <vector>

using namespace std;
using namespace Ogre;

class MyFrameListener : public FrameListener {
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

  std::vector<SceneNode*> *_ptrNodes;
  std::vector<Ogre::Real> _vPosInicial;
  std::vector<Ogre::Real> _vPosFinal;
  std::vector<bool> _vSubir;
  const Ogre::Real _VELOCIDAD_STD = 6;

  Ray setRayQuery(int posx, int posy, uint32 mask);

public:
  MyFrameListener(RenderWindow* win, Camera* cam, 
		 std::vector<Ogre::SceneNode *> *ptrNodes, 
                 OverlayManager* om, SceneManager* sm);
  ~MyFrameListener();
  bool frameStarted(const FrameEvent& evt);  
};
