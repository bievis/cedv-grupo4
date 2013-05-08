#include "MenuState.hpp"

using namespace Ogre;

MenuState::MenuState()
  {
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();
    _mostradoCreditos   = false;
    _mostradoHighScores = false;
    // rect_titulo         = NULL;
    // rect_nave           = NULL;
    _rect_creditos       = NULL;
    _rect_highscores     = NULL;
  }

void MenuState::enter()
  {
    // Carga del sonido.
    _menuTrack = TrackManager::getSingleton().load("menu.mp3");
    _menuFX = SoundFXManager::getSingleton().load("boton.wav");

    // Reproducción del track principal...
    _menuTrack->play();

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

    Ogre::Overlay *overlay = m_pOverlayMgr->getByName ( "GUI_Menu" );
    overlay->show();

    _rect_background = new Ogre::Rectangle2D ( true );
    _rect_background->setCorners ( -1, 1, 1, -1 );
    _rect_background->setMaterial ( "Menu/Background" );

    // Render the background before everything else
    _rect_background->setRenderQueueGroup ( Ogre::RENDER_QUEUE_BACKGROUND );

    // Attach background to the scene
    Ogre::SceneNode* node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode ( "Background" );
    node->attachObject ( _rect_background );

//    put_overlay ( rect_titulo, "Menu/Titulo", -2.0, 1.0, 2.0, -1.0 );

    // Attach background to the scene
//    node->attachObject(rect_titulo);

    // Create background rectangle covering the whole screen
    _rect_creditos = new Rectangle2D(true);
    _rect_creditos->setCorners(-1.0, 1.0, 1.0, -1.0);
    _rect_creditos->setMaterial("Menu/Creditos");

    // Render the background before everything else
    _rect_creditos->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

    // Use infinite AAB to always stay visible
    //    _rect_creditos->setBoundingBox(aabInf);

    _rect_creditos->setVisible ( false );

    // Attach background to the scene
    node->attachObject(_rect_creditos);

    // put_overlay ( _rect_creditos, "Menu/Creditos", -2.0, 1.0, 2.0, -1.0, true );
    // node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode ( "Creditos" );
    // // Attach background to the scene
    // node->attachObject(_rect_creditos);

    // // Create background rectangle covering the whole screen
    // _rect_highscores = new Rectangle2D(true);
    // _rect_highscores->setCorners(-1.0, 1.0, 1.0, -1.0);
    // _rect_highscores->setMaterial("Menu/HighScores");

    // // Render the background before everything else
    // _rect_highscores->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

    // // Use infinite AAB to always stay visible
    // //    _rect_creditos->setBoundingBox(aabInf);

    // _rect_highscores->setVisible ( false );

    // // Attach background to the scene
    // node->attachObject(_rect_highscores);

    // Attach background to the scene
//    node->attachObject(rect_highscores);

    //************************************************************
    //ATENCION!!!: NO ENTIENDO PORQUE TENGO QUE HACER ÉSTO
    //PERO COMO NO PONGA ÉSTO AQUÍ, LA PRIMERA VEZ QUE CARGUE
    //LAS PUNTUACIONES NO APARECE NADA
    //************************************************************
    // Overlay *over = NULL;
    // over = m_pOverlayMgr->getByName ( "Background" );
    // if ( over ) over->show();
    //************************************************************

  }

void MenuState::exit()
  {
    OgreFramework::getSingletonPtr()->getLogMgrPtr()->logMessage("Leaving MenuState...");

    // Parar del track principal...
    _menuTrack->stop();

    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
      OgreFramework::getSingletonPtr()->getRootPtr()->destroySceneManager(m_pSceneMgr);

    Ogre::Overlay *overlay = m_pOverlayMgr->getByName("GUI_Menu");
    overlay->hide();

    if ( _rect_background )
       delete _rect_background;

    if ( _rect_creditos )
      delete _rect_creditos;

    if ( _rect_highscores )
      delete _rect_highscores;

    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->clearAllTrays();
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->getSDKTrayMgrPtr()->setListener(0);
  }

bool MenuState::keyPressed ( const OIS::KeyEvent &keyEventRef )
  {
    if ( OgreFramework::getSingletonPtr()->getKeyboardPtr()->isKeyDown ( OIS::KC_ESCAPE ) )
      {
	cout << _mostradoCreditos << endl;
        if ( _mostradoCreditos )
          {
            mostrarOverlayCreditos ( false );
          }
        else if ( _mostradoHighScores )
          {
            mostrarOverlayHighScores ( false );
          }
        else
          {
            m_bQuit = true;
            // return true;
          }
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

    if ( m_bQuit == true )
      {
        shutdown();
        return;
      }
  }

void MenuState::buttonHit(OgreBites::Button *button)
  {
    _menuFX->play();
    if ( button->getName() == "ExitBtn" )
      m_bQuit = true;
    else if ( button->getName() == "StartBtn" )
      {
	hideButtons();
	changeAppState(findByName("GameState"));
      }
    else if ( button->getName() == "HighScoresBtn" )
      mostrarOverlayHighScores ( true );
    else if ( button->getName() == "CreditsBtn" )
      mostrarOverlayCreditos ( true );
  }

// Muestra u oculta los creditos
void MenuState::mostrarOverlayCreditos ( bool mostrar )
  {
    _mostradoCreditos = mostrar;
    //    Ogre::OverlayElement *elem = NULL;
    Ogre::Overlay *overlay = m_pOverlayMgr->getByName ( "GUI_Menu" );

    if ( mostrar )
      {
        overlay->hide();
        _rect_creditos->setVisible ( true );
        hideButtons();
      }
    else
      {
        overlay->show();
        _rect_creditos->setVisible ( false );
        showButtons();
      }
  }

// Muestra u oculta los highscores
void MenuState::mostrarOverlayHighScores ( bool mostrar )
  {
    Overlay *over = NULL;
    OverlayElement *oe = NULL;
    Ogre::Overlay *overlay = m_pOverlayMgr->getByName ( "GUI_Menu" );

    _mostradoHighScores = mostrar;

    over = m_pOverlayMgr->getByName ( "PantallaRecords" );
    oe = m_pOverlayMgr->getOverlayElement("highScoresValues");

    if ( mostrar )
      {
	// Ocultamos los elementos GUI del menú
        overlay->hide();

	// Ocultamos los botones
        hideButtons();

	//Mostramos el fondo
	//        _rect_highscores->setVisible ( true );
        //Mostramos el panel que contiene el overlay element con el texto
	if ( over )
          over->show();
        //Mostramos el overlay element que tiene unicamente el texto
	oe->show();

        muestra_highscores();
      }
    else
      {
	//Ocultamos el overlay element que tiene unicamente el texto
	oe->hide();
        //Ocultamos el panel que contiene el overlay element con el texto
	if ( over )
	  over->hide();

	// Mostramos los elementos GUI del menú
        overlay->show();

	//Ocultamos el fondo
	//        _rect_highscores->setVisible ( false );

	// Mostramos los botones
        showButtons();
      }
  }

void MenuState::muestra_highscores()
  {
    //    cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":in" << endl;
    OverlayElement *oe = NULL;

    Records::getSingleton().read();

    oe = m_pOverlayMgr->getOverlayElement("highScoresValues");

    // // string msg = " 1. Lv3 * 30s * 14-01-2013 * 22:17\n";
    // // msg += " 2. Lv3 * 35s * 12-02-2013 * 20:18\n";
    // // msg += " 3. Lv3 * 56s * 12-02-2013 * 10:08\n";

     string msg = "";
    // int level = 0;
    int seconds = 0;
    char fecha[100];
    char hora[100];
    char new_hora[100];
    int minutes = 0;

    if ( Records::getSingleton().getSize() > 0 )
      {
        char cad[100];

        // cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":size = " << Records::getSingleton().getSize() << endl;

        for ( unsigned int i = 0; i < Records::getSingleton().getSize(); i++ )
          {
     	    //Entre la fecha y la hora no detecta el separador '|' así que plan B, es decir, cojo posiciones de bytes
     	    //y luego formateo la hora en new_hora que ya no tendría dentro el caracter '|'
     	    sscanf ( Records::getSingleton().getValue(i).c_str(), "%d|%10s%6s", &seconds, fecha, hora );
     	    memcpy ( new_hora, hora+1, strlen(hora)-1);
     	    new_hora[5]=0;
	    minutes = seconds / 60;
	    seconds = seconds % 60;
    //     // cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " level = " << level << endl;
    //     // cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " seconds = " << seconds << endl;
    //     // cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " fecha = " << fecha << endl;
    //     // cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " hora = " << new_hora << endl;
            sprintf ( cad, " %d - %02d:%02d - %s - %s\n", i+1, minutes, seconds, fecha, new_hora );
	    //	    cout << cad << endl;
            msg += string ( cad );
          }
      }
    else
      msg = "No hay registros";

    oe->setColour ( Ogre::ColourValue ( 1.0, 1.0, 0.0 ) );
    oe->setCaption ( msg );
    //    cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":out" << endl;
}
