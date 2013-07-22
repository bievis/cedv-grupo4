#include "Juego.h" 

Juego::Juego(Ogre::Root* root, Ogre::RenderWindow* win, 
				 Ogre::Camera* cam,
         SceneManager* sm) {
  _root = root;
  _win = win;
  _cam = cam;
  _sceneManager = sm;
  _overlayManager = OverlayManager::getSingletonPtr();
}

Juego::~Juego() {
  _root = NULL;
  _win = NULL;
  _cam = NULL;
  _sceneManager = NULL;
  _overlayManager = NULL;
  _personajes.clear();
}

int Juego::start() {
  createScene();
  createElementosMovibles();
  mostrarOverlay(true);
  
  _framelistener = new JuegoFrameListener(_win, _cam, _overlayManager, _sceneManager, _personajes);
  _root->addFrameListener(_framelistener);
  _root->startRendering();
  _root->removeFrameListener(_framelistener);
  delete _framelistener;
  
  mostrarOverlay(false);

  return ESTADO_SALIR_JUEGO;
}

void Juego::createScene() {
  Entity* entEscenario = _sceneManager->createEntity("Escenario", "Escenario.mesh");
  entEscenario->setQueryFlags(ESCENARIO);
  
  Entity* entEspumadera = _sceneManager->createEntity("Espumadera", "Espumadera.mesh");
  entEspumadera->setQueryFlags(ESCENARIO);

  Entity* entEncimera = _sceneManager->createEntity("Encimera", "Encimera.mesh");
  entEncimera->setQueryFlags(ESCENARIO);

  // Creamos el escenario como geometria estatica
  StaticGeometry* escenario = _sceneManager->createStaticGeometry("EscenarioEstatico");
  escenario->addEntity(entEscenario, Vector3(0,0,0));
  escenario->addEntity(entEspumadera, Vector3(3.20714, 0.65222, 0.02925));
  escenario->addEntity(entEncimera, Vector3(2.98123, -0.16964, -0.55548));
  escenario->build();  // Operacion para construir la geometria

  _sceneManager->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);	
  _sceneManager->setAmbientLight(ColourValue(0.2, 0.2, 0.2));

  Light* luz = _sceneManager->createLight("Luz");
  luz->setType(Light::LT_POINT);
  luz->setPosition(6, 4, 0);
  luz->setSpecularColour(1, 1, 1); 
  luz->setDiffuseColour(1, 1, 1);
}

void Juego::createElementosMovibles() {
  float posZ = 2.3;
  _personajes.clear();
  
  for (unsigned int i = 1; i <= 4; i += 1)
  {
    // Primera fila
    createPersonaje(i, 4.12675, posZ);
    
    // Segunda fila
    createPersonaje(4 + i, 2.23941, posZ);

    posZ = posZ - 1.5;
  }
}

void Juego::createPersonaje(unsigned int numero, float posX, float posZ) {
  std::stringstream saux;
  Entity* entPersonaje;
  Entity* entPersonajeMuerto;
  SceneNode* nodePersonaje;  

  saux << "Cucaracha" << numero;
  nodePersonaje = _sceneManager->createSceneNode(saux.str());
  entPersonaje = _sceneManager->createEntity(saux.str(), "Cucaracha.mesh");
  entPersonaje->setQueryFlags(PERSONAJES);
  saux << "Muerta";
  entPersonajeMuerto = _sceneManager->createEntity(saux.str(), "CucarachaMuerta.mesh");
  entPersonajeMuerto->setQueryFlags(PERSONAJES);
  entPersonajeMuerto->setVisible(false);
  nodePersonaje->attachObject(entPersonaje);
  nodePersonaje->attachObject(entPersonajeMuerto);
  nodePersonaje->setPosition(posX, -1.8, posZ);
  _sceneManager->getRootSceneNode()->addChild(nodePersonaje);
  _personajes.push_back(Personaje(nodePersonaje->getName(), nodePersonaje));
  saux.str(string());
}

// Muestra los overlais de esta ventana
void Juego::mostrarOverlay(bool mostrar) {
  Overlay *overlay = _overlayManager->getByName("PantallaJuego");
  if (mostrar) overlay->show();
  else overlay->hide();

  overlay = _overlayManager->getByName("Cursor");
  if (mostrar) overlay->show();
  else overlay->hide();
}
