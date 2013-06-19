#ifndef ENEMY_H
#define ENEMY_H

#include <Character.h>
#include <OgreTextureManager.h>
#include "MyTextureListener.h"
#include "EnemyRoute.h"
#include "GameConfig.h"
#include "Hero.h"

#define SIZE_LIFE_BAR 2.0f

/// \brief enumerator with the states of our enemy character
enum eSTATES_ENEMY {
  WATCHING,
  ALERT,
  SHOOTING,
  CHASING
};

class Enemy : public Character
{
  public:
    /// \brief Enemy constructor parametrized
    /// \param sceneMgr reference to scene manager (ogre)
    /// \param world reference to dynamic world (bullet)
    /// \param name name to identify the Enemy
    /// \param v_pos initial position in coordenate X, Y, Z with object Ogre::Vector3
    /// \param config object with the configuration loaded by xml
    /// \param id_route route assigned to the enemy
    Enemy ( Ogre::SceneManager* sceneMgr,
            OgreBulletDynamics::DynamicsWorld* world,
            const string& name,
            const Ogre::Vector3& v_pos,
            const GameConfig& config,
            unsigned int id_route,
            Hero* ptrHero );
    /** Default destructor */
    virtual ~Enemy();
    /// \brief copy constructor
    /// \param other source enemy to copy
    Enemy ( const Enemy& other );
    /// \brief assignment operator to copy another enemy
    /// \param other source enemy to copy
    Enemy& operator= ( const Enemy& other );
    /// \brief method to get texture pointer
    /// this object inherits from SharedPtr
    /// \return texture pointer
    inline const Ogre::TexturePtr&  getTexturePtr() { return _rtt; };
    /// \brief method to get render to texture reference
    /// \return render to texture reference
    inline Ogre::RenderTexture*     getRenderTexture() const { return _rtex; };
    /// \brief method to get the camera POV reference
    /// \return camera reference
    inline Ogre::Camera*            getCameraPOV() const { return _camPOV; };
    /// \brief this method is used to check if anybody is in our camera scope
    /// \return true/false according to see or not to see anybody
    bool                            haveYouSeenAnybody();
    /// \brief method to print enemy info
    void                            print();
    /// \brief method to walk using the route built to the enemy
    void                            walk_in_route();
        /// \brief method to perform the character walk movement to position
    /// This method uses OgreBullet to move the character
    /// \param pos destiny position to walk
    /// \return true/false if the character arrived to destiny point
    bool                            walk_to ( const Ogre::Vector3& pos, bool running = false );
    /// \brief method to update lifeBar
    void                            updateLifeBar();
    /// \brief method to update enemy in frame
    void                            update ( double timeSinceLastFrame );
    /// \brief method to show dummy or not
    void                            showDummy ( bool show );

    void                            watch_around();

  protected:
    /// \brief protected method to copy a enemy
    /// this method is used in assignment operator and copy constructor
    /// \param other source enemy to copy
    void                            copy ( const Enemy& source );

    void                            setCurrentState ( const eSTATES_ENEMY& newState );

    inline const eSTATES_ENEMY&     getCurrentState() { return _currentState; };

  private:
    /// \brief texture pointer
    Ogre::TexturePtr _rtt;
    /// \brief reference to render to texture object
    Ogre::RenderTexture* _rtex;
    /// \brief reference to camera used to enemy POV
    Ogre::Camera* _camPOV;
    /// \brief reference to texture listener
    MyTextureListener* _textureListener;
    /// \brief route object with the enemy route
    EnemyRoute _route;
    /// \brief current index point to walk
    int _current_point;
    /// \brief way in the route ( true = forward / false = backward )
    bool _way;
    /// \brief reference to BillboardSet of Lifebar
    Ogre::BillboardSet* _bbSetLife;
    /// \brief reference to Billboard of Lifebar
    Ogre::Billboard* _lifeBar;
    /// \brief reference to node of Lifebar
    Ogre::SceneNode* _lifeNode;
    /// \brief object with the enemy state machine
//    StateMachine _sm;
    eSTATES_ENEMY _currentState;


    double _timeElapsed_Global;

    double _timeElapsed_Watching;

    double _timeFirstVision;
    double _timeStartChasing;

    bool _centinel_dest;

    Hero* _refHero;

    Ogre::Vector3 _positionLastViewed;
};

#endif // ENEMY_H
