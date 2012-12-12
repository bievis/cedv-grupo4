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
#include "MyFrameListener.h"

MyFrameListener::MyFrameListener(Ogre::RenderWindow* win, 
				 Ogre::Camera* cam, 
				 Ogre::SceneNode *node, 
				 Ogre::OverlayManager *om,
         SceneManager* sm) {
  _sceneManager = sm;
  _selectedNode = NULL;
  _win = win;
  OIS::ParamList param;
  size_t windowHandle;  std::ostringstream wHandleStr;

  _camera = cam;  _node = node; _overlayManager = om;
  
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
  //_raySceneQuery->setQueryMask(mask);
  return (rayMouse);
}

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
  Ogre::Real r = 0;
  static Ogre::Real posInicial = _node->getPosition().y;
  static Ogre::Real posFinal = posInicial + 5;
  static bool subir = true;
  static Ogre::Real VELOCIDAD = 2;
  Ogre::Real velocidad = VELOCIDAD;
  Ogre::Real deltaT = evt.timeSinceLastFrame;
  int fps = 1.0 / deltaT;

  _keyboard->capture();  _mouse->capture();   // Captura eventos
  
  if(_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;

  // Posicion del raton
  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.

  if(_keyboard->isKeyDown(OIS::KC_R)) r+=180;
  _node->yaw(Ogre::Degree(r * deltaT));
  
  // Subir y bajar objeto
  if(_node->getPosition().y >= posFinal) subir = false;
  else if(_node->getPosition().y <= posInicial) subir = true;
  if(subir) velocidad = VELOCIDAD;
  else velocidad = VELOCIDAD * -1;
  _node->translate(0, velocidad * deltaT, 0);

  //////// Tirar rayo
  // Botones del raton pulsados? -------------------------------------
  bool mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
  //bool mbmiddle = _mouse->getMouseState().buttonDown(OIS::MB_Middle);
  //bool mbright = _mouse->getMouseState().buttonDown(OIS::MB_Right);
  if (mbleft) { // Si hemos pulsado el boton izquierdo del raton
    Ray r = setRayQuery(posx, posy, 0);
    RaySceneQueryResult &result = _raySceneQuery->execute();
    RaySceneQueryResult::iterator it;
    it = result.begin();
    if (it != result.end()) {
      if (_selectedNode != NULL) {
        if(_selectedNode->getName() == "Esfera") _selectedNode->showBoundingBox(true);
        else _sceneManager->getSceneNode("Esfera")->showBoundingBox(false);
      }
      _selectedNode = it->movable->getParentSceneNode();
    }
  }
  //////////////////////////////////////
  
  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("fpsInfo");
  oe->setCaption(Ogre::StringConverter::toString(fps));

  // Posicionamo el cursor del overlay donde este el raton
  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);  oe->setTop(posy);


  return true;
}
