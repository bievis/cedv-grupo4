#include "MenuFrameListener.h"

MenuFrameListener::MenuFrameListener(Ogre::RenderWindow* win, 
				 Ogre::Camera* cam,  
				 Ogre::OverlayManager *om,
         SceneManager* sm) {
  _sceneManager = sm;
  _selectedNode = NULL;
  _win = win;
  OIS::ParamList param;
  size_t windowHandle;  std::ostringstream wHandleStr;

  _camera = cam;  
  _overlayManager = om;
  
  win->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(std::make_pair("WINDOW", wHandleStr.str()));
  
  _inputManager = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*>
    (_inputManager->createInputObject(OIS::OISKeyboard, false));

  _mouse = static_cast<OIS::Mouse*>
    (_inputManager->createInputObject(OIS::OISMouse, false));
  _mouse->getMouseState().width = _win->getWidth();
  _mouse->getMouseState().height = _win->getHeight();

  _raySceneQuery = _sceneManager->createRayQuery(Ray());
}

MenuFrameListener::~MenuFrameListener() {
  _inputManager->destroyInputObject(_keyboard);
  _inputManager->destroyInputObject(_mouse);
  OIS::InputManager::destroyInputSystem(_inputManager);
  _sceneManager->destroyQuery(_raySceneQuery);
}

Ray MenuFrameListener::setRayQuery(int posx, int posy, uint32 mask) {
  Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  _raySceneQuery->setQueryMask(mask);
  return (rayMouse);
}

bool MenuFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
  Ogre::Real deltaT = evt.timeSinceLastFrame;

  _keyboard->capture();  _mouse->capture();   // Captura eventos
  
  if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;

  // Posicion del raton
  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.

  //////// Tirar rayo
  // Botones del raton pulsados? -------------------------------------
  bool mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
  if (mbleft) { // Si hemos pulsado el boton izquierdo del raton
    Ray r = setRayQuery(posx, posy, MENU);
    RaySceneQueryResult &result = _raySceneQuery->execute();
    RaySceneQueryResult::iterator it;
    it = result.begin();
    if (it != result.end()) {
      _selectedNode = it->movable->getParentSceneNode();
      if (_selectedNode != NULL) {
        if(_selectedNode->getName().compare(0, 9, "Cucaracha") == 0) {          
          // TODO Poner aqui cuando se seleccione un opción
        }
      }
    }
  }
  
  Ogre::OverlayElement *oe;
  // Posicionamo el cursor del overlay donde este el raton
  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);  oe->setTop(posy);

  return true;
}
