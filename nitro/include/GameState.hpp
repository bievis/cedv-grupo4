#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "AppState.hpp"

#include "DotSceneLoader.hpp"
//#include "Personaje.h"
#include "TrackManager.h"
#include "SoundFXManager.h"
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include "Records.h"
#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>

#include <vector>
#include <Coche.hpp>

// Segundos en los que se queda el personaje muerto
//#define TIEMPO_MUERTO 1.0

//enum EstadoJuego  { GAME, GAME_OVER, NEXT_LEVEL, WINNER };

// enum QueryFlags
//   {
//     OGRE_HEAD_MASK	= 1<<0,
//     CUBE_MASK		= 1<<1
//   };

#define _NUM_COCHES_ 1

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

    bool keyPressed ( const OIS::KeyEvent &keyEventRef );
    bool keyReleased ( const OIS::KeyEvent &keyEventRef );

    bool mouseMoved ( const OIS::MouseEvent &arg );
    bool mousePressed ( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased ( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    void onLeftPressed ( const OIS::MouseEvent &evt );

    void update(double timeSinceLastFrame);

    void LoadScenaryParts();

private:
    bool          _estaEnMeta;
    bool		      m_bQuit;
    bool		      m_bLMouseDown;
    bool                      m_bRMouseDown;
    Ogre::OverlayManager*     m_pOverlayMgr;

    //    unsigned int              _level;
    //    char                      _vidas;
    double                    _tiempo;
    double                    _mejorTiempo;
    bool                      _empieza_a_contar;
    //    double                    _tiempoTotal;
    //    double                    _tiempoNextLevel;
    //    double                    _tiempoMuertoFin;

    //    GameConfig*               _ptrGameConfig;
    //    EstadoJuego               _estado;

    TrackPtr                  _gameTrack;
    TrackPtr                  _gameoverTrack;
    TrackPtr                  _winnerTrack;
    SoundFXPtr                _moverPersonajeFX;
    SoundFXPtr                _sonidoMetaFX;

    btScalar                  _velocidad;

    OgreBulletDynamics::DynamicsWorld * _world;
    OgreBulletCollisions::DebugDrawer * _debugDrawer;

    // OgreBulletDynamics::WheeledRigidBody  *mCarChassis;
    // OgreBulletDynamics::VehicleTuning     *mTuning;
    // OgreBulletDynamics::VehicleRayCaster  *mVehicleRayCaster;
    // OgreBulletDynamics::RaycastVehicle    *mVehicle;

    // Ogre::Entity    *mChassis;
    // Ogre::Entity    *mWheels[4];
    // Ogre::SceneNode *mWheelNodes[4];

    // float mSteering;

    std::vector<Coche *> _vCoches;

    //Methods

    void CreateInitialWorld();

    string getTime(double tiempo);
    void Mostrar_Velocidad ( float velocidad, bool ocultar = false );
//    void actualizarVidas();
//    void cargarNivel();
};

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||
