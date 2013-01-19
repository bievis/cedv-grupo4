#include "MenuFrameListener.h"

MenuFrameListener::MenuFrameListener(Ogre::RenderWindow* win, 
				 Ogre::Camera* cam,  
				 Ogre::OverlayManager *om,
         SceneManager* sm) {
  _estadoSalida = ESTADO_SALIR;
  _sceneManager = sm;
  _selectedNode = NULL;
  _win = win;
  OIS::ParamList param;
  size_t windowHandle;  std::ostringstream wHandleStr;
  _mostradoRecords = false;
  _mostradoCreditos = false;

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

  _records.read();
}

MenuFrameListener::~MenuFrameListener() {
  _inputManager->destroyInputObject(_keyboard);
  _inputManager->destroyInputObject(_mouse);
  OIS::InputManager::destroyInputSystem(_inputManager);
  _sceneManager->destroyQuery(_raySceneQuery);
  _selectedNode = NULL;
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
  
  if(_keyboard->isKeyDown(OIS::KC_ESCAPE) && _mostradoRecords) {
    mostrarOverlayRecords(false);
  }
  else if ( _keyboard->isKeyDown ( OIS::KC_ESCAPE ) && _mostradoCreditos ) {
    mostrarOverlayCreditos ( false );
  }

  // oe = _overlayManager->getOverlayElement("recordsTitulo");
  // oe->setCaption("Records");

  // oe = _overlayManager->getOverlayElement("recordsValores");
  // string msg = "34 - 14/01/2013 : 22:17\n32 - 14/01/2013 : 15:44\n26 - 17/03/2012 : 12:27";
  // oe->setCaption(msg);

  // Posicion del raton
  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.

  //////// Tirar rayo
  // Botones del raton pulsados? -------------------------------------
  bool mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
  Ray r = setRayQuery(posx, posy, MENU);
  RaySceneQueryResult &result = _raySceneQuery->execute();
  RaySceneQueryResult::iterator it;
  it = result.begin();
  _selectedNode = NULL;
  if (it != result.end()) {
    _selectedNode = it->movable->getParentSceneNode();
    if (_selectedNode != NULL) {
      seleccionarTexto(_selectedNode->getName());
      if (mbleft && !_mostradoRecords && !_mostradoCreditos) {
        if(_selectedNode->getName() == TEXT_PLAY) {
          _estadoSalida = ESTADO_PLAY;
          return false;
        } else if(_selectedNode->getName() == TEXT_RECORDS) {          
          mostrarOverlayRecords(true);
        } else if(_selectedNode->getName() == TEXT_CREDITOS) {          
          mostrarOverlayCreditos ( true );
        } else if(_selectedNode->getName() == TEXT_SALIR) {          
          return false;
        }
      }
    } else {
      seleccionarTexto("");
    }
  }

  if ( _mostradoRecords )
    muestra_records();

  Ogre::OverlayElement *oe;
  // Posicionamo el cursor del overlay donde este el raton
  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);  oe->setTop(posy);

  return true;
}

void MenuFrameListener::muestra_records()
{
  OverlayElement *oe = NULL;

  oe = _overlayManager->getOverlayElement("recordsTitulo");
  oe->setCaption("Records");

  oe = _overlayManager->getOverlayElement("recordsValores");

    // string msg = "1. 34 - 14/01/2013 : 22:17\n2. 32 - 14/01/2013 : 15:44\n3. 26 - 17/03/2012 : 12:27\n";
    // msg += "4. 24 - 14/01/2013 : 22:17\n5. 22 - 14/01/2013 : 15:44\n6. 20 - 17/03/2012 : 12:27\n";
    // msg += "7. 19 - 14/01/2013 : 22:17\n8. 17 - 14/01/2013 : 15:44\n9. 16 - 17/03/2012 : 12:27";

   string msg = "";

  if ( _records.getSize() > 0 )
    {
      char cad[100];

      for ( unsigned int i = 0; i < _records.getSize(); i++ )
	{
	  sprintf ( cad, "%d. %s\n", i+1, _records.getValue(i).c_str() );
	  msg += string ( cad );
	}
    }
  else
    msg = "   No hay registros";

  oe->setCaption(msg);
}

void MenuFrameListener::seleccionarTexto(string texto) {
  _sceneManager->getEntity(TEXT_PLAY)->setMaterialName(MATERIAL_TEXT_NORMAL);
  _sceneManager->getEntity(TEXT_RECORDS)->setMaterialName(MATERIAL_TEXT_NORMAL);
  _sceneManager->getEntity(TEXT_SALIR)->setMaterialName(MATERIAL_TEXT_NORMAL);
  _sceneManager->getEntity(TEXT_CREDITOS)->setMaterialName(MATERIAL_TEXT_NORMAL);
  if (texto != "") {
    _sceneManager->getEntity(texto)->setMaterialName(MATERIAL_TEXT_SELECCION);
  }
}

// Muestra o oculta los records
void MenuFrameListener::mostrarOverlayRecords(bool mostrar) {
  Overlay *over = NULL;

  _mostradoRecords = mostrar;
  
  over = _overlayManager->getByName ( "PantallaRecords" );

  if (mostrar) over->show();
  else over->hide();
}

// Muestra o oculta los records
void MenuFrameListener::mostrarOverlayCreditos ( bool mostrar ) {
  Overlay *over;

  _mostradoCreditos = mostrar;

  over = _overlayManager->getByName ( "PantallaCreditos" );

  if (mostrar) over->show();
  else over->hide();
}
