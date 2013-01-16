#include "MyFrameListener.h"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, 
				 Ogre::Camera* cam,  
				 Ogre::OverlayManager *om,
         SceneManager* sm,
         std::vector<Personaje> &personajes) {
  _puntos = 0;
  _tiempoTranscurrido = 0;
  _personajes = personajes;
  _sceneManager = sm;
  _selectedNode = NULL;
  _win = win;
  OIS::ParamList param;
  size_t windowHandle;  std::ostringstream wHandleStr;

  _camera = cam; 
  _nodeCamera = _sceneManager->getRootSceneNode()->createChildSceneNode("NodoCamara");
  _nodeCamera->attachObject(_camera); // Atachamos la camara con el nodo de la camara
  
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

MyFrameListener::~MyFrameListener() {
  _inputManager->destroyInputObject(_keyboard);
  _inputManager->destroyInputObject(_mouse);
  OIS::InputManager::destroyInputSystem(_inputManager);
  _sceneManager->destroyQuery(_raySceneQuery);
}

Ray MyFrameListener::setRayQuery(int posx, int posy, uint32 mask) {
  Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  _raySceneQuery->setQueryMask(mask);
  return (rayMouse);
}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
  Ogre::Real deltaT = evt.timeSinceLastFrame;
  unsigned int tiempoRedondeado = Math ::Ceil(_tiempoTranscurrido);
  _tiempoTranscurrido = _tiempoTranscurrido + deltaT;
  //int fps = 1.0 / deltaT;

  _keyboard->capture();  _mouse->capture();   // Captura eventos
  
  if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;

  // Posicion del raton
  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.

  //////// Tirar rayo
  // Botones del raton pulsados? -------------------------------------
  bool mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
  if (mbleft) { // Si hemos pulsado el boton izquierdo del raton
    Ray r = setRayQuery(posx, posy, PERSONAJES);
    RaySceneQueryResult &result = _raySceneQuery->execute();
    RaySceneQueryResult::iterator it;
    it = result.begin();
    if (it != result.end()) {
      _selectedNode = it->movable->getParentSceneNode();
      if (_selectedNode != NULL) {
        if(_selectedNode->getName().compare(0, 9, "Cucaracha") == 0) {          
          std::vector<Personaje>::iterator it1;
          bool encontrado = false;
          Personaje *pers = NULL;
          for (it1 = _personajes.begin(); it1 != _personajes.end() && !encontrado; it1++) {
            pers = &(*it1);
            if (pers->getNombre() == _selectedNode->getName()) encontrado = true;
          }
          if (encontrado) {
            if (pers->getEstado() == MOVIMIENTO) { // Ya estaba moviendose lo matamos
              _puntos++;
              pers->setEstado(MUERTO);
            }
          }
        }
      }
    }
  }

  // Sacamos un personaje aleatoriamente
  Personaje* personaje = moverPersonajeAleatorio(tiempoRedondeado);
  if (tiempoRedondeado >= 5) {
    rotarCamara(deltaT);
  }
  
  // Mover los que haya que mover
  std::vector<Personaje>::iterator it2;
  for (it2 = _personajes.begin(); it2 != _personajes.end(); it2++) {
    if ((*it2).getEstado() == MOVIMIENTO || (*it2).getEstado() == MUERTO) {
      (*it2).mover(deltaT);
    }
  }
  //////////////////////////////////////
  
  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("puntuacion");
  oe->setCaption(Ogre::StringConverter::toString(_puntos));
  oe = _overlayManager->getOverlayElement("tiempo");
  oe->setCaption(Ogre::StringConverter::toString(tiempoRedondeado));

  // Posicionamo el cursor del overlay donde este el raton
  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);  oe->setTop(posy);


  return true;
}

Personaje* MyFrameListener::moverPersonajeAleatorio(unsigned int tiempo) {
  static int ultimo = -1;
  unsigned int aleatorio = 0;
  Personaje* personaje = NULL;
  bool encontrado = false;
  
  if (tiempo > 0 && ultimo != tiempo && tiempo % TIEMPO_LANZAR_PERSONAJE == 0) {
    while (!encontrado) {
      aleatorio = Math ::Ceil(Math::RangeRandom (-1, 7));
      personaje = &(_personajes[aleatorio]);
      if (personaje->getEstado() == PARADADO) encontrado = true;
    }

    personaje->setEstado(MOVIMIENTO);
    ultimo = tiempo;
  }
  
  return personaje;
}

void MyFrameListener::rotarCamara (const Ogre::Real deltaT) {
  static Ogre::Real _posFinal = 180;
  cout << Ogre::Degree(_nodeCamera->getOrientation ().getYaw()) << endl;
  int grados = Math::Ceil(Ogre::Degree(_nodeCamera->getOrientation().getYaw()).valueDegrees());
  if(grados < _posFinal
     && grados >= 0)
    _nodeCamera->yaw(Ogre::Degree(180 * deltaT), Node::TS_WORLD);
}
