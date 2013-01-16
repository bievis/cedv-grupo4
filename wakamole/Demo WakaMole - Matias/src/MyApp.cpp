#include "MyApp.h" 

MyApp::MyApp() {
  _sceneManager = NULL;
  _framelistener = NULL;
}

MyApp::~MyApp() {
  delete _root;
  delete _framelistener;
}

int MyApp::start() {
  _root = new Root();
  
  if(!_root->restoreConfig()) {
    _root->showConfigDialog();
    _root->saveConfig();
  }
  
  RenderWindow* window = _root->initialise(true,"WakaMole");
  _sceneManager = _root->createSceneManager(ST_GENERIC);
  
  Camera* cam = _sceneManager->createCamera("Camara");
  cam->setPosition(Vector3(14,4.5,0));
  cam->lookAt(Vector3(0,0,0));
  cam->setFOVy(Degree(36));
  cam->setNearClipDistance(5);
  cam->setFarClipDistance(10000);

  
  Viewport* viewport = window->addViewport(cam);
  viewport->setBackgroundColour(ColourValue(0.0,0.0,0.0));
  double width = viewport->getActualWidth();
  double height = viewport->getActualHeight();
  cam->setAspectRatio(width / height);
  
  loadResources();
  createScene();
  createElementosMovibles();
  createOverlay();
  
  _framelistener = new MyFrameListener(window, cam, _overlayManager, _sceneManager, _personajes);
  _root->addFrameListener(_framelistener);
  
  _root->startRendering();
  return 0;
}

void MyApp::loadResources() {
  ConfigFile cf;
  cf.load("resources.cfg");
  
  ConfigFile::SectionIterator sI = cf.getSectionIterator();
  String sectionstr, typestr, datastr;
  while (sI.hasMoreElements()) {
    sectionstr = sI.peekNextKey();
    ConfigFile::SettingsMultiMap *settings = sI.getNext();
    ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i) {
      typestr = i->first;    datastr = i->second;
      ResourceGroupManager::getSingleton().addResourceLocation
            (datastr, typestr, sectionstr);	
    }
  }
  ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void MyApp::createScene() {
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

void MyApp::createElementosMovibles() {
  float posZ = 2.3;

  for (unsigned int i = 1; i <= 4; i += 1)
  {
    // Primera fila
    createPersonaje(i, 4.12675, posZ);
    
    // Segunda fila
    createPersonaje(4 + i, 2.23941, posZ);

    posZ = posZ - 1.5;
  }
}

void MyApp::createPersonaje(unsigned int numero, float posX, float posZ) {
  std::stringstream saux;
  Entity* entPersonaje;
  SceneNode* nodePersonaje;  

  saux << "Cucaracha" << numero;
  entPersonaje = _sceneManager->createEntity(saux.str(), "Cucaracha.mesh");
  entPersonaje->setQueryFlags(PERSONAJES);   // Usamos flags propios!
  nodePersonaje = _sceneManager->createSceneNode(saux.str());
  nodePersonaje->attachObject(entPersonaje);
  nodePersonaje->setPosition(posX, -1.8, posZ);
  _sceneManager->getRootSceneNode()->addChild(nodePersonaje);
  _personajes.push_back(Personaje(nodePersonaje->getName(), nodePersonaje));
  saux.str(string());
}

void MyApp::createOverlay() {
  _overlayManager = OverlayManager::getSingletonPtr();
  Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();
}
