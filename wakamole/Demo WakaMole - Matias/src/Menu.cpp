#include "Menu.h" 

Menu::Menu(Ogre::Root* root, Ogre::RenderWindow* win, 
				 Ogre::Camera* cam,
         SceneManager* sm) {
  _root = root;
  _win = win;
  _cam = cam;
  _sceneManager = sm;
}

Menu::~Menu() {
  _root = NULL;
  _win = NULL;
  _cam = NULL;
  _sceneManager = NULL;
  _personajes.clear();
}

int Menu::start() {
  createScene();
  createElementosMovibles();
  createOverlay();
  
  _framelistener = new MenuFrameListener(_win, _cam, _overlayManager, _sceneManager);
  _root->addFrameListener(_framelistener);
  _root->startRendering();
  
  return 0;
}

void Menu::createScene() {
  Entity* entEscenario = _sceneManager->createEntity("Escenario", "Escenario.mesh");
  entEscenario->setQueryFlags(ESCENARIO);   // Usamos flags propios!
  SceneNode* nodeEscenario = _sceneManager->createSceneNode("Escenario");
  nodeEscenario->attachObject(entEscenario);
  _sceneManager->getRootSceneNode()->addChild(nodeEscenario);
  
  Entity* entEspumadera = _sceneManager->createEntity("Espumadera", "Espumadera.mesh");
  entEspumadera->setQueryFlags(ESCENARIO);   // Usamos flags propios!
  SceneNode* nodeEspumadera = _sceneManager->createSceneNode("Espumadera");
  nodeEspumadera->attachObject(entEspumadera);
  nodeEspumadera->setPosition(3.20714, 0.65222, 0.02925);
  _sceneManager->getRootSceneNode()->addChild(nodeEspumadera);

  Entity* entEncimera = _sceneManager->createEntity("Encimera", "Encimera.mesh");
  entEncimera->setQueryFlags(ESCENARIO);   // Usamos flags propios!
  SceneNode* nodeEncimera = _sceneManager->createSceneNode("Encimera");
  nodeEncimera->attachObject(entEncimera);
  nodeEncimera->setPosition(2.98123, -0.16964, -0.55548);
  _sceneManager->getRootSceneNode()->addChild(nodeEncimera);

  _sceneManager->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);	
  _sceneManager->setAmbientLight(ColourValue(0.2, 0.2, 0.2));

  Light* luz = _sceneManager->createLight("Luz");
  luz->setType(Light::LT_POINT);
  luz->setPosition(6, 4, 0);
  luz->setSpecularColour(1, 1, 1); 
  luz->setDiffuseColour(1, 1, 1);
}

void Menu::createElementosMovibles() {
  // TODO Cramos los elementos movible
}

void Menu::createOverlay() {
  _overlayManager = OverlayManager::getSingletonPtr();
  Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();
}
