/*********************************************************************
 * Módulo 2. Curso de Experto en Desarrollo de Videojuegos
 * Autor: Carlos González Morcillo     Carlos.Gonzalez@uclm.es
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

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
  
  RenderWindow* window = _root->initialise(true,"MyApp Example");
  _sceneManager = _root->createSceneManager(ST_GENERIC);
  
  Camera* cam = _sceneManager->createCamera("MainCamera");
  cam->setPosition(Vector3(5,10.5,12));
  cam->lookAt(Vector3(1.4,4.3,3));
  cam->setNearClipDistance(5);
  cam->setFarClipDistance(10000);
  cam->setFOVy(Degree(48));

  
  Viewport* viewport = window->addViewport(cam);
  viewport->setBackgroundColour(ColourValue(0.0,0.0,0.0));
  double width = viewport->getActualWidth();
  double height = viewport->getActualHeight();
  cam->setAspectRatio(width / height);
  
  loadResources();
  createScene();
  createOverlay();

  SceneNode *node = _sceneManager->getSceneNode("Esfera");
  
  _framelistener = new MyFrameListener(window, cam, node, _overlayManager, _sceneManager);
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
  Entity* ent2 = _sceneManager->createEntity("Esfera", "Esfera.mesh");
  SceneNode* node2 = _sceneManager->createSceneNode("Esfera");
  node2->attachObject(ent2);
  _sceneManager->getRootSceneNode()->addChild(node2);

  _sceneManager->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);	
  _sceneManager->setAmbientLight(ColourValue(0.2, 0.2, 0.2));

  Light* light = _sceneManager->createLight("Light1");
  light->setType(Light::LT_DIRECTIONAL);
  light->setDirection(Vector3(1,-1,0));

  Light* light2 = _sceneManager->createLight("Light2");
  light2->setType(Light::LT_POINT);
  light2->setPosition(8, 8, -2);
  light2->setSpecularColour(0.9, 0.9, 0.9); 
  light2->setDiffuseColour(0.9, 0.9, 0.9);

  Plane plane1(Vector3::UNIT_Y, 0);
  MeshManager::getSingleton().createPlane("plane1",
	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
	200,200,1,1,true,1,10,10,Vector3::UNIT_Z);

  SceneNode* node3 = _sceneManager->createSceneNode("ground");
  Entity* groundEnt = _sceneManager->createEntity("planeEnt", "plane1");
  groundEnt->setMaterialName("Ground");
  node3->attachObject(groundEnt);
  _sceneManager->getRootSceneNode()->addChild(node3);

}

void MyApp::createOverlay() {
  _overlayManager = OverlayManager::getSingletonPtr();
  Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();
}
