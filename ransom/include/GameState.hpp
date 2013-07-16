#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "AppState.hpp"

#include "DotSceneLoader.hpp"
#include "TrackManager.h"
#include "SoundFXManager.h"
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include "Records.h"

#include "Utilities.h"
#include "Enemy.h"
#include "Hero.h"
#include "Hostage.h"
#include "XMLCharger.h"
#include "GameConfig.h"
#include "MiniMapTextureListener.h"
#include "CamerasController.h"
#include "Fader.h"

#define FILE_ROUTE_XML "./config/config.xml"
#define NAME_TEXTUTE_MINIMAP "RttT_Map"
#define NAME_MATERIAL_MINIMAP "RttMat_Map"
// Tiempo que se esta mostrando la pantalla final
#define TIMER_END_SHOW 10.0f

/// \brief This class manage the Game State
class GameState : public AppState, FaderCallback
  {

public:
    /// \brief default constructor
    GameState();

    DECLARE_APPSTATE_CLASS(GameState)

    /// \brief this method was performed at enter of this app state
    void enter();
    /// \brief this method was performed at exit of this app state
    void exit();
    /// \brief this method was performed at pause the game
    bool pause();
    /// \brief this method was performed at resume from pause game
    void resume();
    /// \brief this method built the GUI
    void buildGUI();
    /// \brief this method was performed when the key was pressed
    /// \param keyEventRef key event received
    bool keyPressed ( const OIS::KeyEvent &keyEventRef );
    /// \brief this method was performed when the key was released
    /// \param keyEventRef key event received
    bool keyReleased ( const OIS::KeyEvent &keyEventRef );
    /// \brief this method was performed when the mouse was moved
    /// \param arg mouse event received
    bool mouseMoved ( const OIS::MouseEvent &arg );
    /// \brief this method was performed when the mouse button was pressed
    /// \param arg mouse event received
    /// \param id mouse button identifier
    bool mousePressed ( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    /// \brief this method was performed when the mouse button was released
    /// \param arg mouse event received
    /// \param id mouse button identifier
    bool mouseReleased ( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    /// \brief this method was performed when the left mouse button was pressed
    /// \param evt mouse event received
    void onLeftPressed ( const OIS::MouseEvent &evt );
    /// \brief this method will be run every frame
    void update ( double timeSinceLastFrame );
    /// \brief this method load the parts in the scene
    void LoadScenaryParts();
    /// \brief this method translate the time from seconds to hh:mm:ss
    /// \param time time in seconds
    string getTime ( double time );
    /// \brief this method update hero panel life
    void updatePanelLife();
    /// \brief this method was executed when the Fader effect was over
    void fadeOutCallback(void);

private:
    /// \brief this attribute will be true when we found to exit the state
    bool		                  m_bQuit;
    /// \brief this attribute will be true when the left button was down
    bool	            	      m_bLMouseDown;
    /// \brief this attribute will be true when the right button was down
    bool                      m_bRMouseDown;
    /// \brief reference to overlay manager
    Ogre::OverlayManager*     m_pOverlayMgr;
    /// \brief soundtrack played during the gameplay
    TrackPtr                  _gameTrack;
    /// \brief reference to the ogrebullet world
    OgreBulletDynamics::DynamicsWorld * _world;
    /// \brief reference to the hero
    Hero*                     m_hero;
    /// \brief queue for the enemies references
    std::deque<Enemy*>        m_enemies;
    /// \brief vector for the hostage references
    std::vector<Hostage*>	    m_hostages;
    /// \brief vector for the characters references
    std::vector<Character*>   _vCharacters;
    /// \brief camara reference used to the minimap
    Ogre::Camera*             _cameraMiniMap;
    /// \brief reference to camera controller
    CamerasController*		    _camerasController;
    /// \brief reference to texture listener used in the minimap
    MiniMapTextureListener*   _textureListener;
    /// \brief reference to render to texture used in the minimap
    Ogre::RenderTexture*      _rtex;
    /// \brief instance for the game config object
    GameConfig                _gc;
    /// \brief vector for the fader effects on every overlay with this effect
    std::vector<Fader*>       _vFader;
    /// \brief reference to the fader effect used in the game finish
    Fader*                    _faderFinish;
    /// \brief time in seconds
    double                    _tiempo;
    /// \brief time to view the final overlay with the victory/defeat overlay
    double                    _tiempoMostrarFinal;
    /// \brief hostages number
    unsigned int              _hostages;
    /// \brief reference to static geometry
    Ogre::StaticGeometry*     _staticGeometry;
    /// \brief sound effects when the game over occurs
    SoundFXPtr                _soundGameOver;
    /// \brief sound effects when the finish victory occurs
    SoundFXPtr                _soundFinish;
    /// \brief index of the last panel life viewed (from 1 to 4)
    unsigned int              _lastPanelLife;

    //Private Methods

    /// \brief method to clear the state
    void clear();
    /// \brief method to create map
    void CreateMap();
    /// \brief method to create scene world
    void CreateInitialWorld();
    /// \brief method to create game camera and minimap camera
    void CreateCameras();
    /// \brief method to create mini map of game
    void CreateMiniMap();
    /// \brief method to detecting collisions between Hero with Hostages
    /// \param world reference to world (OgreBullet)
    /// \param hero Hero
    /// \param hostages List of hostages
    /// \return reference to Hostage colliding with hero
    Hostage* detectCollisionHeroWithHostages(OgreBulletDynamics::DynamicsWorld* world,
												Hero* hero,
												std::vector<Hostage*> hostages);
};

#endif
