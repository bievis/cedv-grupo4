//|||||||||||||||||||||||||||||||||||||||||||||||

#include "MenuState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

MenuState::MenuState()
{
    m_bQuit             = false;
    m_FrameEvent        = Ogre::FrameEvent();
    _mostradoCreditos   = false;
    _mostradoHighScores = false;
    rect_titulo         = NULL;
    rect_nave           = NULL;
    rect_creditos       = NULL;
    rect_highscores     = NULL;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::enter()
{
    // Carga del sonido.
//    _mainTrack = TrackManager::getSingleton().load("01_-_Dj_Saryon_-_Break_the_rules_Remastered_Mix_.mp3");
    _mainTrack = TrackManager::getSingleton().load("fondo.mp3");
    _menuFX = SoundFXManager::getSingleton().load("croac.wav");    

    // Reproducción del track principal...
    this->_mainTrack->play();
        
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering MenuState...");

    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    m_pCamera = m_pSceneMgr->createCamera("MenuCam");
    m_pCamera->setPosition(Vector3(0, 25, -50));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(1);

    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

    m_pOverlayMgr = Ogre::OverlayManager::getSingletonPtr();
    //    Ogre::Overlay *background = m_pOverlayMgr->getByName("Background");
    //    background->show();

    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();

    // createButtons();

    createMenuScene();

    OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
}

void MenuState::createButtons()
  {
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "StartBtn", "Start Game", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "HighScoresBtn", "Highscores", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "CreditsBtn", "Credits", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit Game", 250);

    Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();
  }

void MenuState::showButtons ()
  {
    OgreBites::Button* b = NULL;

    b = (OgreBites::Button*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("StartBtn"); 
    if ( b ) b->show();
    b = (OgreBites::Button*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("HighScoresBtn"); 
    if ( b ) b->show();
    b = (OgreBites::Button*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("CreditsBtn"); 
    if ( b ) b->show();
    b = (OgreBites::Button*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("ExitBtn"); 
    if ( b ) b->show();
  }

void MenuState::hideButtons ()
  {
    OgreBites::Button* b = NULL;

    b = (OgreBites::Button*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("StartBtn"); 
    if ( b ) b->hide();
    b = (OgreBites::Button*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("HighScoresBtn"); 
    if ( b ) b->hide();
    b = (OgreBites::Button*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("CreditsBtn"); 
    if ( b ) b->hide();
    b = (OgreBites::Button*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("ExitBtn"); 
    if ( b ) b->hide();
  }

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::createMenuScene()
  {
    //    OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);

    //    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "TitleLbl", "Frogger - The Game", 200);

    //    OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);

    OgreFramework::getSingletonPtr()->m_pTrayMgr->setWidgetSpacing ( 5 );

    createButtons();

    // Create background material
    MaterialPtr material = MaterialManager::getSingleton().create("Background", "General");
    material->getTechnique(0)->getPass(0)->createTextureUnitState("background_stars.png");
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    
    // Create background rectangle covering the whole screen
    Rectangle2D* rect = new Rectangle2D(true);
    rect->setCorners ( -2.0, 1.0, 2.0, -1.0 );
    rect->setMaterial("Background");
    
    // Render the background before everything else
    rect->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);
    
    // Use infinite AAB to always stay visible
    AxisAlignedBox aabInf;
    aabInf.setInfinite();
    rect->setBoundingBox(aabInf);
     
    // Attach background to the scene
    SceneNode* node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("Background");
    node->attachObject(rect);
    
    // Example of background scrolling
    material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.015, 0.0);

    // Create background rectangle covering the whole screen
    rect_nave = new Rectangle2D(true);
    rect_nave->setCorners(-2.0, 1.0, 2.0, -1.0);
    rect_nave->setMaterial("Menu/Nave");
    
    // Render the background before everything else
    rect_nave->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);
    
    // Use infinite AAB to always stay visible
    rect_nave->setBoundingBox(aabInf);
    
    // Attach background to the scene
    node->attachObject(rect_nave);

    // Create background rectangle covering the whole screen
    rect_titulo = new Rectangle2D(true);
    rect_titulo->setCorners(-2.0, 1.0, 2.0, -1.0);
    rect_titulo->setMaterial("Menu/Titulo");
    
    // Render the background before everything else
    rect_titulo->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);
    
    // Use infinite AAB to always stay visible
    rect_titulo->setBoundingBox(aabInf);
    
    // Attach background to the scene
    node->attachObject(rect_titulo);

    // Create background rectangle covering the whole screen
    rect_creditos = new Rectangle2D(true);
    rect_creditos->setCorners(-2.0, 1.0, 2.0, -1.0);
    rect_creditos->setMaterial("Menu/Creditos");
    
    // Render the background before everything else
    rect_creditos->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);
    
    // Use infinite AAB to always stay visible
    rect_creditos->setBoundingBox(aabInf);
    
    rect_creditos->setVisible ( false );

    // Attach background to the scene
    node->attachObject(rect_creditos);

    // Create background rectangle covering the whole screen
    rect_highscores = new Rectangle2D(true);
    rect_highscores->setCorners(-2.0, 1.0, 2.0, -1.0);
    rect_highscores->setMaterial("Menu/HighScores");
    
    // Render the background before everything else
    rect_highscores->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);
    
    // Use infinite AAB to always stay visible
    rect_highscores->setBoundingBox(aabInf);
    
    rect_highscores->setVisible ( false );

    // Attach background to the scene
    node->attachObject(rect_highscores);

    //************************************************************
    //ATENCION!!!: NO ENTIENDO PORQUE TENGO QUE HACER ÉSTO
    //PERO COMO NO PONGA ÉSTO AQUÍ, LA PRIMERA VEZ QUE CARGUE
    //LAS PUNTUACIONES NO APARECE NADA
    //************************************************************
    Overlay *over = NULL;
    over = m_pOverlayMgr->getByName ( "PantallaRecords" );
    if ( over ) over->show();
    //************************************************************    
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving MenuState...");

    // Parar del track principal...
    this->_mainTrack->stop();

    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);

    Ogre::Overlay *background = m_pOverlayMgr->getByName("Background");
    background->hide();

    if ( rect_titulo )
      delete rect_titulo;

    if ( rect_nave )
      delete rect_nave;

    if ( rect_creditos )
      delete rect_creditos;

    if ( rect_highscores )
      delete rect_highscores;

    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if ( OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE) )
      {
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
            return true;
          }
      }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

    if(m_bQuit == true)
    {
        shutdown();
        return;
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::buttonHit(OgreBites::Button *button)
  {
    _menuFX->play();
    if ( button->getName() == "ExitBtn" )
      m_bQuit = true;
    else if ( button->getName() == "StartBtn" )
      changeAppState(findByName("GameState"));
    else if ( button->getName() == "HighScoresBtn" )
      mostrarOverlayHighScores ( true );
    else if ( button->getName() == "CreditsBtn" )
      mostrarOverlayCreditos ( true );
  }

// Muestra u oculta los creditos
void MenuState::mostrarOverlayCreditos ( bool mostrar ) 
  {
    _mostradoCreditos = mostrar;

    if ( mostrar )
      {
        rect_creditos->setVisible ( true );
        rect_nave->setVisible ( false );
        rect_titulo->setVisible ( false );
        hideButtons();
      }
    else
      {
        rect_creditos->setVisible ( false );
        rect_nave->setVisible ( true );
        rect_titulo->setVisible ( true );
        showButtons();
      }
  }

// Muestra u oculta los highscores
void MenuState::mostrarOverlayHighScores ( bool mostrar ) 
  {
    cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":in" << endl;
    Overlay *over = NULL;

    _mostradoHighScores = mostrar;

    over = m_pOverlayMgr->getByName ( "PantallaRecords" );

    if ( mostrar )
      {
	if ( over )
          over->show();
        rect_highscores->setVisible ( true );
        rect_nave->setVisible ( false );
        rect_titulo->setVisible ( false );
        hideButtons();
        muestra_highscores();
      }
    else
      {
	if ( over )
	  over->hide();
        rect_highscores->setVisible ( false );
        rect_nave->setVisible ( true );
        rect_titulo->setVisible ( true );
        showButtons();
      }
    cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":out" << endl;
  }

void MenuState::muestra_highscores()
  {
    cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":in" << endl;
    OverlayElement *oe = NULL;

    Records::getSingleton().read();

    oe = m_pOverlayMgr->getOverlayElement("highScoresValues");

    string msg = " 1. Lv3 * 30s * 14-01-2013 * 22:17\n";
    msg += " 2. Lv3 * 35s * 12-02-2013 * 20:18\n";
    msg += " 3. Lv3 * 56s * 12-02-2013 * 10:08\n";
    msg += " 4. Lv2 * 45s * 11-02-2013 * 12:15\n";
    msg += " 5. Lv2 * 55s * 08-02-2013 * 19:22\n";
    msg += " 6. Lv2 * 70s * 03-02-2013 * 10:05\n";
    msg += " 7. Lv2 * 80s * 05-02-2013 * 08:09\n";
    msg += " 8. Lv2 * 81s * 04-02-2013 * 15:03\n";
    msg += " 9. Lv1 * 25s * 03-02-2013 * 10:05\n";
    msg += "10. Lv1 * 35s * 01-02-2013 * 09:26";

    // string msg = "";

    // if ( Records::getSingleton().getSize() > 0 )
    //   {
    //     char cad[100];

    //     for ( unsigned int i = 0; i < Records::getSingleton().getSize(); i++ )
    //       {
    //         sprintf ( cad, "%d. %s\n", i+1, Records::getSingleton().getValue(i).c_str() );
    //         msg += string ( cad );
    //       }
    //   }
    // else
    //   msg = "   No hay registros";

    oe->setColour ( Ogre::ColourValue ( 1.0, 1.0, 0.0 ) );
    oe->setCaption ( msg );
    cout << "*** " << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":in" << endl;
}
