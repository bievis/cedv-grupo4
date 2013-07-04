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

#define NAME_MAP "Map"
#define FILE_ROUTE_XML "./config/config.xml"
#define MAP_ROUTE_XML "./config/Mapa.tmx"
#define NAME_TEXTUTE_MINIMAP "RttT_Map"
#define NAME_MATERIAL_MINIMAP "RttMat_Map"

class GameState : public AppState, FaderCallback
  {

public:
    GameState();

    DECLARE_APPSTATE_CLASS(GameState)

    void enter();
    void exit();
    bool pause();
    void resume();

    void buildGUI();

    bool keyPressed ( const OIS::KeyEvent &keyEventRef );
    bool keyReleased ( const OIS::KeyEvent &keyEventRef );

    bool mouseMoved ( const OIS::MouseEvent &arg );
    bool mousePressed ( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased ( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    void onLeftPressed ( const OIS::MouseEvent &evt );

    void update(double timeSinceLastFrame);

    void LoadScenaryParts();

    string getTime ( double tiempo );

    void updatePanelLife();

    void fadeOutCallback(void);

private:
    bool          _estaEnMeta; // Indica que ya ha entrado en la linea de la meta
    bool          _estaEnPreMeta; // Indica que ya ha entrado en la linea de la meta
    int           _controlMeta; // Controla el paso por meta
    bool		      m_bQuit;
    bool		      m_bLMouseDown;
    bool                      m_bRMouseDown;
    Ogre::OverlayManager*     m_pOverlayMgr;

//    double                    _tiempo;
//    double                    _mejorTiempo;
//    bool                      _empieza_a_contar;

    TrackPtr                  _gameTrack;

//    btScalar                  _velocidad;

    OgreBulletDynamics::DynamicsWorld * _world;
    OgreBulletCollisions::DebugDrawer * _debugDrawer;

    Hero*                     m_hero;
    std::deque<Enemy*>        m_enemies;
    std::vector<Hostage*>	    m_hostages;
    std::vector<Character*>   _vCharacteres;

    Ogre::Camera*             _cameraMiniMap;
    CamerasController*		    _camerasController;
    MiniMapTextureListener*   _textureListener;
    Ogre::RenderTexture*      _rtex;

    // Game configuration
    GameConfig                _gc;

    std::vector<Fader*>       _vFader;
    Fader*                    _faderGameOver;
    Fader*                    _faderFinish;
    double                    _tiempo;
    unsigned int              _hostages;

	Ogre::StaticGeometry *_staticGeometry;

    SoundFXPtr                _soundGameOver;
    SoundFXPtr                _soundFinish;

    //Private Methods
	// \brief method to create the state
	void clear();

    // \brief method to create map
    void CreateMap();

    // \brief method to Create woeld of the scene
    void CreateInitialWorld();
    // \brief method to create game camera and minimap camera
    void CreateCameras();
    // \brief method to create mini map of game
    void CreateMiniMap();
    // \brief method to detecting collisions between Hero with Hostages
    /// \param world reference to world (OgreBullet)
    /// \param hero Hero
    /// \param hostages List of hostages
    /// \return reference to Hostage colliding with hero
    Hostage* detectCollisionHeroWithHostages(OgreBulletDynamics::DynamicsWorld* world,
												Hero* hero,
												std::vector<Hostage*> hostages);
};

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||
