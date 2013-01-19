#include <Ogre.h>
#include <OIS/OIS.h>
#include "Constantes.h"
#include "Records.h"

using namespace std;
using namespace Ogre;

#define ESCENARIO 1 << 0  // Mascara para el escenario
#define MENU 1 << 1  // Mascara para los opciones de menu
#define TIEMPO_LANZAR_PERSONAJE 1 // Tiempo que cada cuanto se saca un personaje
#define TEXT_PLAY "TextPlay"
#define TEXT_RECORDS "TextRecords"
#define TEXT_SALIR "TextSalir"
#define TEXT_CREDITOS "TextCreditos"
#define MATERIAL_TEXT_NORMAL "MaterialTextoMenu"
#define MATERIAL_TEXT_SELECCION "MaterialTextoEncima"

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
  int _estadoSalida; // Estado de cuando se sale del FrameListener del Menu 
  bool _mostradoRecords; // Nos dice si se estan mostrando los records o no ahora
  bool _mostradoCreditos; // Nos dice si se estan mostrando los creditos o no ahora
  Records _records;

  Ray setRayQuery(int posx, int posy, uint32 mask);
  void seleccionarTexto(string texto);
  void mostrarOverlayRecords(bool mostrar);
  void mostrarOverlayCreditos ( bool mostrar );
  void muestra_records();

public:
  MenuFrameListener(Ogre::RenderWindow* win, 
				 Ogre::Camera* cam,  
				 Ogre::OverlayManager *om,
         SceneManager* sm);
  ~MenuFrameListener();
  bool frameStarted(const FrameEvent& evt);
  inline int getEstadoSalida() { return _estadoSalida; };
};
