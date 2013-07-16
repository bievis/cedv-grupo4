#ifndef ENEMY_H
#define ENEMY_H

#include <Character.h>
#include <OgreTextureManager.h>
#include "MyTextureListener.h"
#include "EnemyRoute.h"
#include "GameConfig.h"
#include "Hero.h"

#define SIZE_LIFE_BAR 2.0f
#define NAME_TEXTUTE_CAM "RttT_"
#define NAME_MATERIAL_CAM "RttMat_"
#define TIMER_MAX_BLOCKED 3.0f

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
    /// \param route route of enemy
    /// \param ptrHero reference to hero
    Enemy ( Ogre::SceneManager* sceneMgr,
            OgreBulletDynamics::DynamicsWorld* world,
            const string& name,
            const Ogre::Vector3& v_pos,
            const EnemyRoute &route,
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
    /// \brief method to watch around the enemy 360º
    void                            watch_around(double timeSinceLastFrame);
    /// \brief method to update lifeBar
    void                            updateLifeBar();
    /// \brief method to update enemy in frame
    void                            update ( double timeSinceLastFrame, std::vector<Character*>   vCharacteres);
    /// \brief method to show dummy or not
    void                            showDummy ( bool show );
    /// \brief method to get the distance between the enemy and the hero
    /// \return a real value with the distance
    const Ogre::Real&               get_distance_with_hero();
    /// \brief this method reorient the enemy direction to the hero position
    void                            reorient_enemy_to_hero();
    /// \brief method to set the enemy state
    /// \param newState new state to set
    void                            setCurrentState ( const eSTATES_ENEMY& newState );
    /// \brief method to get the enemy state
    /// \return the current enemy state
    inline const eSTATES_ENEMY&     getCurrentState() { return _currentState; };

  protected:
    /// \brief protected method to copy a enemy
    /// this method is used in assignment operator and copy constructor
    /// \param other source enemy to copy
    void                            copy ( const Enemy& source );
    /// \brief method to play the sound associated to the alert of an enemy
    void                            play_sound_alert();
    /// \brief method to play the sound associated to the enemy hurt
    void                            play_sound_hurt();

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
    /// \brief current enemy state
    eSTATES_ENEMY _currentState;
    /// \brief time elapsed in the game
    double _timeElapsed_Global;
    /// \brief time elapsed between the enemy watch to the hero and dissapear him
    double _timeElapsed_Watching;
    /// \brief time elapsed between shoots
    double _timeElapsed_Shooting;
    /// \brief attribute to save the time when the enemy sees the hero for the first time
    double _timeFirstVision;
    /// \brief time when start the chasing
    double _timeStartChasing;
    /// \brief time used to know if the enemy is blocked
    double _timeBlocked;
    /// \brief time used to know the last time when the sound was played
    double _timeLastSoundAlertPlayed;
    /// \brief current position to use when we check if the enemy is blocked
    Ogre::Vector3 _currentPosition;
    /// \brief sentinel used to validate if the enemy is at the point where he saw the hero the last time
    bool _sentinel_dest;
    /// \brief reference to the hero object
    Hero* _refHero;
    /// \brief last position of our hero when he was viewed
    Ogre::Vector3 _positionLastViewed;
    /// \brief reference to sound alert number 1
    SoundFXPtr                _soundAlert1FX;
    /// \brief reference to sound alert number 2
    SoundFXPtr                _soundAlert2FX;
    /// \brief reference to sound hurt number 1
    SoundFXPtr                _soundHurt1FX;
    /// \brief reference to sound hurt number 2
    SoundFXPtr                _soundHurt2FX;
    /// \brief counter to use with the alert current sound (it can be 1 or 2)
    unsigned int              _currentSoundAlert;
    /// \brief counter to use with the hurt current sound (it can be 1 or 2)
    unsigned int              _currentSoundHurt;
    /// \brief list of returns points for when the enemy is chasing the hero
    std::vector<Ogre::Vector3>   _vReturnsPoints;
    /// \brief indicates that spinning
    bool _stopAround;
    /// \brief number of turns
    unsigned int _aroundNumber;
    /// \brief timer of seach
    double _timeSeach;
    /// \brief last health
    float _lastHealth;
};

#endif // ENEMY_H
