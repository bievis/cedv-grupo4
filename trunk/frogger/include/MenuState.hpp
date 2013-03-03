//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.hpp"
#include "TrackManager.h"
#include "SoundFXManager.h"
#include "Records.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

class MenuState : public AppState
{
public:
    MenuState();

	DECLARE_APPSTATE_CLASS(MenuState)

	void enter();
	void createMenuScene();
        void createButtons();
        void showButtons();
        void hideButtons();
	void exit();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	void buttonHit(OgreBites::Button* button);

	void update(double timeSinceLastFrame);

        void mostrarOverlayCreditos ( bool mostrar );
        void mostrarOverlayHighScores ( bool mostrar );
        void muestra_highscores();

private:
	bool                        m_bQuit;
  TrackPtr _mainTrack;
  SoundFXPtr _menuFX;
  Ogre::OverlayManager* m_pOverlayMgr;
  bool _mostradoCreditos; // Nos dice si se estan mostrando los creditos o no ahora
  bool _mostradoHighScores; // Nos dice si se estan mostrando los highscores o no ahora

  //Menu principal
  Rectangle2D* rect_titulo;
  Rectangle2D* rect_nave;

  //Creditos
  Rectangle2D* rect_creditos;

  //HighScores
  Rectangle2D* rect_highscores;

};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||
