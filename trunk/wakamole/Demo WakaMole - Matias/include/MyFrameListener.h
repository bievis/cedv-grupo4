#include <Ogre.h>
#include <OIS/OIS.h>
#include <vector>
#include "Personaje.h"

using namespace std;
using namespace Ogre;

#define ESCENARIO 1 << 0  // Mascara para el escenario
#define PERSONAJES 1 << 1  // Mascara para los personajes
#define TIEMPO_LANZAR_PERSONAJE 1 // Tiempo que cada cuanto se saca un personaje

class MyFrameListener : public FrameListener {
private:
  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  Camera* _camera;
  SceneNode *_nodeCamera;
  RenderWindow* _win;
  SceneManager* _sceneManager;
  OIS::Mouse* _mouse;
  OverlayManager* _overlayManager;
  RaySceneQuery *_raySceneQuery;
  SceneNode *_selectedNode;
  std::vector<Personaje> _personajes;
  float _tiempoTranscurrido;
  int _puntos;

  Ray setRayQuery(int posx, int posy, uint32 mask);
  void rotarCamara (const Ogre::Real deltaT);
public:
  MyFrameListener(RenderWindow* win, Camera* cam, 
		  OverlayManager* om, SceneManager* sm,
      std::vector<Personaje> &personajes);
  ~MyFrameListener();
  bool frameStarted(const FrameEvent& evt);
  Personaje* moverPersonajeAleatorio(unsigned int tiempo);  
};
