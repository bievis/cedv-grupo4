//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.hpp"

#include "DotSceneLoader.hpp"
#include "Personaje.h"

#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

// Segundos en los que se queda el personaje muerto
#define TIEMPO_MUERTO 3.0

//|||||||||||||||||||||||||||||||||||||||||||||||

enum QueryFlags
{
	OGRE_HEAD_MASK	= 1<<0,
  CUBE_MASK		= 1<<1
};

//|||||||||||||||||||||||||||||||||||||||||||||||

class GameState : public AppState
{
public:
	GameState();

	DECLARE_APPSTATE_CLASS(GameState)

	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();
  
  void buildGUI();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void onLeftPressed(const OIS::MouseEvent &evt);

	void update(double timeSinceLastFrame);

  void LoadScenaryParts();

private:
	bool						m_bQuit;
	bool						m_bLMouseDown, m_bRMouseDown;
  Ogre::OverlayManager* m_pOverlayMgr;
  double _tiempo;
  unsigned int _level;
  char _vidas;
  GameConfig* _ptrGameConfig;
  double _tiempoMuertoFin;

        string getTime();
  
  void actualizarVidas();
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||