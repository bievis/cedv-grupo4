#include "KickRoach.h" 
#include "Constantes.h"

KickRoach::KickRoach() {
  _sceneManager = NULL;
  _root = NULL;
}

KickRoach::~KickRoach() {
  delete _root;
  delete _juego;
  delete _menu;
}

int KickRoach::start() {
  _root = new Root();
  
  if(!_root->restoreConfig()) {
    _root->showConfigDialog();
    _root->saveConfig();
  }
  
  RenderWindow* window = _root->initialise(true,"KickRoach");
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

  int estado = ESTADO_INICIO; // Alamacena el estado actual del juego
  // Partes del juego
  _juego = new Juego(_root, window, cam, _sceneManager);
  _menu = new Menu(_root, window, cam, _sceneManager);
  
  while (estado != ESTADO_SALIR) {
    if (estado == ESTADO_PLAY) estado = _juego->start();
    else estado = _menu->start();
    limpiarEscena();
  }
  
  return 0;
}

// Limpia la escena
void KickRoach::limpiarEscena() {
  // Eliminamos todos los nodos
  _sceneManager->getRootSceneNode()->removeAndDestroyAllChildren ();
  // Eliminamos todos las entidades
  _sceneManager->destroyAllEntities();
  // Eliminamos todas las luces
  _sceneManager->destroyAllLights();
}

void KickRoach::loadResources() {
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
