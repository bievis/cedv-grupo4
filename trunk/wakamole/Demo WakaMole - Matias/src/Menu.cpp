#include "Menu.h" 

Menu::Menu(Ogre::Root* root, Ogre::RenderWindow* win, 
				 Ogre::Camera* cam,
         SceneManager* sm) {
  _root = root;
  _win = win;
  _cam = cam;
  _sceneManager = sm;
  _overlayManager = OverlayManager::getSingletonPtr();
}

Menu::~Menu() {
  _root = NULL;
  _win = NULL;
  _cam = NULL;
  _sceneManager = NULL;
  _overlayManager = NULL;
}

int Menu::start() {
  int estado;
  createScene();
  mostrarOverlay(true);
  
  _framelistener = new MenuFrameListener(_win, _cam, _overlayManager, _sceneManager);
  _root->addFrameListener(_framelistener);
  _root->startRendering();
  _root->removeFrameListener(_framelistener);
  estado = _framelistener->getEstadoSalida();
  delete _framelistener;

  mostrarOverlay(false);  
  
  return estado;
}

void Menu::createScene() {
  Entity* entEscenario = _sceneManager->createEntity("Escenario", "Escenario.mesh");
  entEscenario->setQueryFlags(ESCENARIO);   // Usamos flags propios!
  SceneNode* nodeEscenario = _sceneManager->createSceneNode("Escenario");
  nodeEscenario->attachObject(entEscenario);
  _sceneManager->getRootSceneNode()->addChild(nodeEscenario);

  Entity* entEncimera = _sceneManager->createEntity("Encimera", "Encimera.mesh");
  entEncimera->setQueryFlags(ESCENARIO);   // Usamos flags propios!
  SceneNode* nodeEncimera = _sceneManager->createSceneNode("Encimera");
  nodeEncimera->attachObject(entEncimera);
  nodeEncimera->setPosition(2.98123, -0.16964, -0.55548);
  _sceneManager->getRootSceneNode()->addChild(nodeEncimera);

  Entity* entTextKickRoach = _sceneManager->createEntity("TextKickRoach", "TextKickRoach.mesh");
  entTextKickRoach->setQueryFlags(ESCENARIO);   // Usamos flags propios!
  SceneNode* nodeTextKickRoach = _sceneManager->createSceneNode("TextKickRoach");
  nodeTextKickRoach->attachObject(entTextKickRoach);
  nodeTextKickRoach->setPosition(3.03908, 3.63143, -0.7);
  _sceneManager->getRootSceneNode()->addChild(nodeTextKickRoach);

  Entity* entTextPlay = _sceneManager->createEntity(TEXT_PLAY, "TextPlay.mesh");
  entTextPlay->setQueryFlags(MENU);   // Usamos flags propios!
  SceneNode* nodeTextPlay = _sceneManager->createSceneNode(TEXT_PLAY);
  nodeTextPlay->attachObject(entTextPlay);
  nodeTextPlay->setPosition(6.80000, 2.27710, 0);
  _sceneManager->getRootSceneNode()->addChild(nodeTextPlay);

  Entity* entTextRecords = _sceneManager->createEntity(TEXT_RECORDS, "TextRecords.mesh");
  entTextRecords->setQueryFlags(MENU);   // Usamos flags propios!
  SceneNode* nodeTextRecords = _sceneManager->createSceneNode(TEXT_RECORDS);
  nodeTextRecords->attachObject(entTextRecords);
  nodeTextRecords->setPosition(6.80000, 1.52596, 0);
  _sceneManager->getRootSceneNode()->addChild(nodeTextRecords);

  Entity* entTextSalir = _sceneManager->createEntity(TEXT_SALIR, "TextSalir.mesh");
  entTextSalir->setQueryFlags(MENU);   // Usamos flags propios!
  SceneNode* nodeTextSalir = _sceneManager->createSceneNode(TEXT_SALIR);
  nodeTextSalir->attachObject(entTextSalir);
  nodeTextSalir->setPosition(6.80000, 0.56771, 0);
  _sceneManager->getRootSceneNode()->addChild(nodeTextSalir);

  Entity* entTextCreditos = _sceneManager->createEntity(TEXT_CREDITOS, "TextCreditos.mesh");
  entTextCreditos->setQueryFlags(MENU);   // Usamos flags propios!
  SceneNode* nodeTextCreditos = _sceneManager->createSceneNode(TEXT_CREDITOS);
  nodeTextCreditos->attachObject(entTextCreditos);
  nodeTextCreditos->setPosition(9.3, 1.44051, -1.0);
  _sceneManager->getRootSceneNode()->addChild(nodeTextCreditos);

  _sceneManager->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);	
  _sceneManager->setAmbientLight(ColourValue(0.2, 0.2, 0.2));

  Light* luz = _sceneManager->createLight("Luz");
  luz->setType(Light::LT_POINT);
  luz->setPosition(8, 4, 0);
  luz->setSpecularColour(1, 1, 1); 
  luz->setDiffuseColour(1, 1, 1);
}

// Muestra los overlays de esta ventana
void Menu::mostrarOverlay(bool mostrar) {
  Overlay *overlay = _overlayManager->getByName("Cursor");
  if (mostrar) overlay->show();
  else overlay->hide();
}
