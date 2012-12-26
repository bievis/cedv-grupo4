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
  
  RenderWindow* window = _root->initialise(true,"Whack-a-Mole");
  _sceneManager = _root->createSceneManager(ST_GENERIC);
  
  Camera* cam = _sceneManager->createCamera("MainCamera");
  //  cam->setPosition(Vector3(5,10.5,12));
  cam->setPosition(Vector3(15,24.5,45));
  //cam->lookAt(Vector3(1.4,4.3,3));
  cam->lookAt(Vector3(15,4.3,3));
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

  SceneNode *node = 0;
  string sVal = "";
  const char *cNombre = "Esfera";
  char cad[50];

  //Partiendo de que esto es una ñapa ...
  //Para los objetos del 0 al 3 los muevo a primera linea de pantalla y separados
  //cada uno por 10 unids. además los pongo inicialmente en posiciones de Y distintas
  //para que en la animacion nada mas empezar se muevan a distintos niveles, por ultimo el 15
  //es porque esten mas cerca de pantalla
  //Para los objetos del 4 al 6 los muevo a segunda linea pos Z = 0, pero por lo demas son iguales
  //a la primera linea, salvando que son uno menos y los desplazo 5 unids a la derecha para centrarlos
  for ( unsigned int i = 0; i < _NUM_ELEMS_; i++ )
    {
      sprintf ( cad, "%s%i", cNombre, i );
      sVal = cad;
      node = _sceneManager->getSceneNode ( sVal.c_str() );
      if ( i < 4 )
	{
         
          node->translate ( i*10, i%5, 15 );
	}
      else
	{
          node->translate ( ((i-4)*10) + 5, i%5, 0 );
	}
      vNodes.push_back ( node );
    }
  
  _framelistener = new MyFrameListener ( window, cam, &vNodes, _overlayManager, _sceneManager );
  _root->addFrameListener ( _framelistener );
  
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

  Entity* ent = 0;
  SceneNode* node = 0;
  const char * cNombre = "Esfera";
  string sVal = "";
  char cad[50];

  for (unsigned int i = 0; i < _NUM_ELEMS_; i++ )
    {
      sprintf ( cad, "%s%d", cNombre, i );
      sVal = cad;
      cout << "Creando " << cad << " ... " << flush;
      ent = _sceneManager->createEntity ( sVal.c_str(), "Esfera.mesh" );
      node = _sceneManager->createSceneNode ( sVal.c_str() );
      node->attachObject(ent);
      _sceneManager->getRootSceneNode()->addChild(node);
      cout << "OK" << endl;
    }

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

  SceneNode* nodeGround = _sceneManager->createSceneNode ( "ground" );
  Entity* entGround = _sceneManager->createEntity ( "planeEnt", "plane1" );
  entGround->setMaterialName ( "Ground" );
  nodeGround->attachObject ( entGround );
  _sceneManager->getRootSceneNode()->addChild ( nodeGround );

}

void MyApp::createOverlay() {
  _overlayManager = OverlayManager::getSingletonPtr();
  Overlay *overlay = _overlayManager->getByName("Info");
  overlay->show();
}
