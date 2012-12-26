#include "MyFrameListener.h"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, 
				 Ogre::Camera* cam, 
				 std::vector<Ogre::SceneNode *> *nodes,
				 Ogre::OverlayManager *om,
                                 SceneManager* sm) {
  _sceneManager = sm;
  _selectedNode = NULL;
  _win = win;
  OIS::ParamList param;
  size_t windowHandle;  std::ostringstream wHandleStr;

  _camera = cam;
  _overlayManager = om;
  _ptrNodes = nodes;
  
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
  
  _vPosInicial.resize((*_ptrNodes).size(),0);
  _vPosFinal.resize((*_ptrNodes).size(),5);
  _vSubir.resize((*_ptrNodes).size(),false);

}

MyFrameListener::~MyFrameListener() {
  _inputManager->destroyInputObject(_keyboard);
  _inputManager->destroyInputObject(_mouse);
  OIS::InputManager::destroyInputSystem(_inputManager);
  _sceneManager->destroyQuery(_raySceneQuery);

  _vPosInicial.clear();
  _vPosFinal.clear();
  _vSubir.clear();
}

Ray MyFrameListener::setRayQuery(int posx, int posy, uint32 mask) {
  Ray rayMouse = _camera->getCameraToViewportRay
    (posx/float(_win->getWidth()), posy/float(_win->getHeight()));
  _raySceneQuery->setRay(rayMouse);
  _raySceneQuery->setSortByDistance(true);
  //_raySceneQuery->setQueryMask(mask);
  return (rayMouse);
}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
  //  Ogre::Real r = 0;
  //  static Ogre::Real posInicial = 0;
  //  static Ogre::Real posFinal = 0;
  //  static bool subir = true;
  Ogre::Real velocidad = _VELOCIDAD_STD;
  Ogre::Real deltaT = evt.timeSinceLastFrame;
  int fps = 1.0 / deltaT;

  _keyboard->capture();  _mouse->capture();   // Captura eventos
  
  if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;

  // Posicion del raton
  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.

  //  if(_keyboard->isKeyDown(OIS::KC_R)) r+=180;
  //  _node->yaw(Ogre::Degree(r * deltaT));

  //Movimiento de los objetos
  for ( unsigned int i = 0; i < (*_ptrNodes).size(); i++ )
    {
      //      cout << "Objeto : " << (*_ptrNodes)[i]->getName() << flush;

      //      cout << " : (*_ptrNodes)[i]->getPosition().y [" << (*_ptrNodes)[i]->getPosition().y 
      //	   << "] >= _vPosFinal[i] [" << _vPosFinal[i] << "]" << endl;
    
      if ( (*_ptrNodes)[i]->getPosition().y >= _vPosFinal[i] ) _vSubir[i] = false;
      else if ( (*_ptrNodes)[i]->getPosition().y <= _vPosInicial[i] ) _vSubir[i] = true;
      
      if ( _vSubir[i] ) velocidad = _VELOCIDAD_STD;
      else velocidad = _VELOCIDAD_STD * -1;

      (*_ptrNodes)[i]->translate ( 0, velocidad * deltaT, 0 );
    }

  //////// Tirar rayo
  // Botones del raton pulsados? -------------------------------------
  bool mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
  //bool mbmiddle = _mouse->getMouseState().buttonDown(OIS::MB_Middle);
  //bool mbright = _mouse->getMouseState().buttonDown(OIS::MB_Right);
  if ( mbleft ) { // Si hemos pulsado el boton izquierdo del raton
    Ray r = setRayQuery(posx, posy, 0);
    RaySceneQueryResult &result = _raySceneQuery->execute();
    RaySceneQueryResult::iterator it;
    string strFind = "Esfera";
    bool bEncontrado = false;

    it = result.begin();
    if (it != result.end()) {
      if (_selectedNode != NULL) {
        for ( unsigned int i = 0; i < (*_ptrNodes).size(); i++ )
	  {
            cout << "selected " << _selectedNode->getName() << " == " << 
              "nodo " << i << " " <<  (*_ptrNodes)[i]->getName() << endl;
            if ( _selectedNode->getName() == (*_ptrNodes)[i]->getName() )
	      {
                bEncontrado = true; 
                break;
              }
	  }

        if ( bEncontrado )
	  {
            _selectedNode->showBoundingBox(true);
	  }
        else 
          {
            string sVal = "";
            const char *cNombre = "Esfera";
            char cad[50];
            for ( unsigned int i = 0; i < (*_ptrNodes).size(); i++ )
	      {
                sprintf ( cad, "%s%d", cNombre, i );
                sVal = cad;
                _sceneManager->getSceneNode ( sVal.c_str() )->showBoundingBox ( false );
	      }
	  }
      }
      _selectedNode = it->movable->getParentSceneNode();
    }
  }
  //////////////////////////////////////
  
  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("fpsInfo");
  oe->setCaption(Ogre::StringConverter::toString(fps));

  // Posicionamos el cursor del overlay donde este el raton
  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);  oe->setTop(posy);

  return true;
}
