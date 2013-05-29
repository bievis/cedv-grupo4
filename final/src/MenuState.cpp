#include "MenuState.hpp"

using namespace Ogre;

MenuState::MenuState()
  {
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();
    _mostradoCreditos   = false;
    _mostradoHighScores = false;
    _show_inputbox      = false;
  }

void MenuState::enter()
  {
    // // Carga del sonido.
    // _menuTrack = TrackManager::getSingleton().load("menu.mp3");
    // _menuFX = SoundFXManager::getSingleton().load("boton.wav");

    // // Reproducción del track principal...
    // _menuTrack->play();

    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage ( "Entering MenuState..." );

    m_pSceneMgr = OgreFramework::getSingletonPtr()->getRootPtr()->createSceneManager ( ST_GENERIC, "MenuSceneMgr" );
    m_pSceneMgr->setAmbientLight ( Ogre::ColourValue ( 0.7f, 0.7f, 0.7f ) );

    m_pCamera = m_pSceneMgr->createCamera("MenuCam");
    m_pCamera->setPosition(Vector3(0, 25, -50));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(1);

    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->getViewportPtr()->getActualWidth()) /
			      Real(OgreFramework::getSingletonPtr()->getViewportPtr()->getActualHeight()));

    OgreFramework::getSingletonPtr()->getViewportPtr()->setCamera(m_pCamera);

    m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->destroyAllWidgets();

    createMenuScene();

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->showCursor();
}

void MenuState::createButtons()
  {
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->createButton ( OgreBites::TL_CENTER, "StartBtn", "Start Game", 350 );
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->createButton ( OgreBites::TL_CENTER, "HighScoresBtn", "Best Times", 350 );
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->createButton ( OgreBites::TL_CENTER, "CreditsBtn", "Credits", 350 );
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->createButton ( OgreBites::TL_CENTER, "ExitBtn", "Exit Game", 350 );

    Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();
  }

void MenuState::showButtons ()
  {
    createButtons();
  }

void MenuState::hideButtons ()
  {
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->destroyAllWidgets();
  }

void MenuState::createMenuScene()
  {
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->setWidgetSpacing ( 5 );

    createButtons();

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_Menu", true );

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "Background_Menu", true );
  }

void MenuState::exit()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Leaving MenuState...");

    // // Parar del track principal...
    // _menuTrack->stop();

    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
      OgreFramework::getSingletonPtr()->getRootPtr()->destroySceneManager(m_pSceneMgr);

    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Menu");
    overlay->hide();

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->clearAllTrays();
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->setListener(0);
  }

bool MenuState::keyPressed ( const OIS::KeyEvent &keyEventRef )
  {
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_ESCAPE ) )
      {
        if ( _mostradoCreditos )
          {
            show_screen ( CREDITS_SCREEN, false );
          }
        else if ( _mostradoHighScores )
          {
            show_screen ( HIGHSCORES_SCREEN, false );
          }
        else
          {
             m_bQuit = true;
          }
      }
    else if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_A ) )
      {
        inputbox(!_show_inputbox);
      }

    OgreFramework::getSingletonPtr()->keyPressed ( keyEventRef );
    return true;
  }

bool MenuState::keyReleased ( const OIS::KeyEvent &keyEventRef )
  {
    OgreFramework::getSingletonPtr()->keyReleased ( keyEventRef );
    return true;
  }

bool MenuState::mouseMoved(const OIS::MouseEvent &evt)
  {
    if ( OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->injectMouseMove(evt) ) return true;
    return true;
  }

bool MenuState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
  {
    if ( OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->injectMouseDown ( evt, id ) ) return true;
    return true;
  }

bool MenuState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
  {
    if ( OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->injectMouseUp ( evt, id ) ) return true;
    return true;
  }

void MenuState::update(double timeSinceLastFrame)
  {
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->frameRenderingQueued ( m_FrameEvent );

    // if ( _mostradoHighScores )
    //   {
    //     refresca_highscores();
    //   }

    if ( m_bQuit == true )
      {
        shutdown();
        return;
      }
  }

// void MenuState::refresca_highscores()
// {
//     OverlayElement *oe = NULL;

//     oe = m_pOverlayMgr->getOverlayElement("highScoresValues");
//     oe->setCaption ( _msg_highscore );
// }

void MenuState::buttonHit(OgreBites::Button *button)
  {
    // _menuFX->play();
    if ( button->getName() == "ExitBtn" )
      m_bQuit = true;
    // else if ( button->getName() == "StartBtn" )
    //   {
    // 	hideButtons();
    // 	changeAppState(findByName("GameState"));
    //   }
    else if ( button->getName() == "HighScoresBtn" )
      show_screen ( HIGHSCORES_SCREEN, true );
    //   mostrarOverlayHighScores ( true );
    else if ( button->getName() == "CreditsBtn" )
      {
//        _mostradoCreditos = true;
//        OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->hideCursor();
//        hideButtons();
//        Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_Menu", false );
//        Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "Creditos_Menu", true );
        show_screen ( CREDITS_SCREEN, true );
      }

  }

// Muestra u oculta los creditos
// void MenuState::mostrarOverlayCreditos ( bool mostrar )
//   {
//     _mostradoCreditos = mostrar;
//     //    Ogre::OverlayElement *elem = NULL;
//     Ogre::Overlay *overlay = m_pOverlayMgr->getByName ( "GUI_Menu" );

//     if ( mostrar )
//       {
//         overlay->hide();
//         _rect_creditos->setVisible ( true );
//         hideButtons();
//       }
//     else
//       {
//         overlay->show();
//         _rect_creditos->setVisible ( false );
//         showButtons();
//       }
//   }

// Muestra u oculta los highscores
// void MenuState::mostrarOverlayHighScores ( bool mostrar )
//   {
//     Overlay *over_rec = m_pOverlayMgr->getByName ( "PantallaRecords" );
//     Overlay *over_gui = m_pOverlayMgr->getByName ( "GUI_Menu" );

//     _mostradoHighScores = mostrar;

//     if ( mostrar )
//       {
// 	// // Ocultamos los elementos GUI del menú
//         over_gui->hide();

// 	// // Ocultamos los botones
//         hideButtons();

//         // //Mostramos el panel que contiene el overlay element con el texto
// 	over_rec->show();

//         muestra_highscores();
//       }
//     else
//       {
//         //Ocultamos el panel que contiene el overlay element con el texto
//         over_rec->hide();

//      	// Mostramos los elementos GUI del menú
//         over_gui->show();

//      	// Mostramos los botones
//         showButtons();
//       }
//   }

// void MenuState::muestra_highscores()
//   {
//     Records::getSingleton().read();

//     string msg = "";
//     int seconds = 0;
//     char fecha[100];
//     char hora[100];
//     char new_hora[100];
//     int minutes = 0;

//     if ( Records::getSingleton().getSize() > 0 )
//       {
//         char cad[100];

//         for ( unsigned int i = 0; i < Records::getSingleton().getSize(); i++ )
//           {
//      	    //Entre la fecha y la hora no detecta el separador '|' así que plan B, es decir, cojo posiciones de bytes
//      	    //y luego formateo la hora en new_hora que ya no tendría dentro el caracter '|'
//      	    sscanf ( Records::getSingleton().getValue(i).c_str(), "%d|%10s%6s", &seconds, fecha, hora );
//      	    memcpy ( new_hora, hora+1, strlen(hora)-1);
//      	    new_hora[5]=0;
//     	    minutes = seconds / 60;
//     	    seconds = seconds % 60;
//             sprintf ( cad, " %d - %02d:%02d [ %s - %s ]\n", i+1, minutes, seconds, fecha, new_hora );
//             msg += string ( cad );
//           }
//       }
//     else
//       msg = "        No hay registros";

//     _msg_highscore = msg;
// }

void MenuState::show_screen ( eScreens scr, bool visible )
  {
    if ( CREDITS_SCREEN == scr )
      {
        _mostradoCreditos = visible;
        Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "Creditos_Menu", visible );
      }
    else if ( HIGHSCORES_SCREEN == scr )
      {
        _mostradoHighScores = visible;
        Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "HighScores_Menu", visible );
      }

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_Menu", !visible );

    if ( visible )
      {
        hideButtons();
        OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->hideCursor();
      }
    else
      {
        showButtons();
        OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->showCursor();
      }
  }

void MenuState::inputbox ( bool visible )
  {
    _show_inputbox = visible;

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "Common_InputBox", visible );

    Utilities::getSingleton().put_overlay ( m_pOverlayMgr, "GUI_Menu", !visible );

    if ( visible )
      {
        hideButtons();
        OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->hideCursor();
      }
    else
      {
        showButtons();
        OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->showCursor();
      }
  }
