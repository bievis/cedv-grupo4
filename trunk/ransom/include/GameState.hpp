#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "AppState.hpp"

#include "DotSceneLoader.hpp"
#include "TrackManager.h"
#include "SoundFXManager.h"
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include "Records.h"
#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>

#include "Utilities.h"
#include "Character.h"

//#include <vector>
// #include <Coche.hpp>

// #define _NUM_COCHES_ 1

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
    bool          _estaEnMeta; // Indica que ya ha entrado en la linea de la meta
    bool          _estaEnPreMeta; // Indica que ya ha entrado en la linea de la meta
    int           _controlMeta; // Controla el paso por meta
    bool		      m_bQuit;
    bool		      m_bLMouseDown;
    bool                      m_bRMouseDown;
    Ogre::OverlayManager*     m_pOverlayMgr;

    OgreBulletCollisions::CollisionShape *ShapeFloor;
    OgreBulletDynamics::RigidBody *defaultPlaneBodyFloor;

//    double                    _tiempo;
//    double                    _mejorTiempo;
//    bool                      _empieza_a_contar;

//    TrackPtr                  _gameTrack;
//    SoundFXPtr                _sonidoMetaFX;

//    btScalar                  _velocidad;

    OgreBulletDynamics::DynamicsWorld * _world;
    OgreBulletCollisions::DebugDrawer * _debugDrawer;

    Character*                m_hero;
    std::deque<Character*>    m_enemies;

    Ogre::Entity*             entFloor;
//    Ogre::TexturePtr          rtt;
//    Ogre::RenderTexture*      rtex;
//    Ogre::Camera*             camPOV;

    //Private Methods

    void CreatePlane();

    void check_vision();

    void CreateInitialWorld();

    // string getTime(double tiempo);
    // void Mostrar_Velocidad ( float velocidad, bool ocultar = false );
    // // \brief Reiniciar coche y lo vuelve a poner el inicio
    // void reiniciarCoche ();
    // // \brief Reiniciar coche y lo vuelve a poner el inicio
    // void insertarElementoEscena (string nombreElemento);
};

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||
