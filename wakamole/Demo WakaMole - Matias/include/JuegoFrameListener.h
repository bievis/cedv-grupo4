#include <Ogre.h>
#include <OIS/OIS.h>
#include <vector>
#include "Personaje.h"
#include "Constantes.h"

using namespace std;
using namespace Ogre;

#define ESCENARIO 1 << 0  // Mascara para el escenario
#define PERSONAJES 1 << 1  // Mascara para los personajes
#define TIEMPO_LANZAR_PERSONAJE 1 // Tiempo que cada cuanto se saca un personaje
#define TIEMPO_FIN 60 // Tiempo que dura la partida
#define ACELERAR 1 // Lo que incrementa la velocidad del personaje
#define TIEMPO_ACELERAR 10 // Tiempo cada cuanto se acelera el movimiento del personaje

class JuegoFrameListener : public FrameListener {
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
  std::vector<Personaje> _personajes;
  float _tiempoTranscurrido;
  int _puntos;
  int _acelerar; // Almacena lo que se van acelerando los personajes
  unsigned int _ultimoAcelerar; // Ultima vez que acelero

  Ray setRayQuery(int posx, int posy, uint32 mask);
public:
  JuegoFrameListener(RenderWindow* win, Camera* cam, 
		  OverlayManager* om, SceneManager* sm,
      std::vector<Personaje> &personajes);
  ~JuegoFrameListener();
  bool frameStarted(const FrameEvent& evt);
  Personaje* moverPersonajeAleatorio(unsigned int tiempo);
  void moverPersonajes(Ogre::Real deltaT, unsigned int tiempo);
};
