#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h> // Numeros Aleatorios
#include "Utilities.h"
#include "SoundFXManager.h"
#include "OgreBulletCollisionsRay.h"

using namespace std;

#define VELOCIDAD 2.0
#define VELOCIDAD_ANIMACION 3.0
#define VELOCITY_SHOT 30.0f

#define HEALTH_SHOT 25.0

#define VELOCIDAD_RUNNING 5.0
//#define VELOCIDAD_RUNNING_ANIMACION 6.0

#define MAX_HEALTH 100.0

#define ZERO_DISTANCE 0.0

// Animation Name of Mesh
#define MOVE_ANIMATION "Move"
#define STOP_ANIMATION "Stop"

/// \brief timer show the hostage when is death
#define TIMER_PATICLE_DEATH 2.0f

// Position of shot with respect to character
#define POSITION_SHOT Ogre::Vector3(-0.2780,1.0726,1.6939) 

enum CHARACTER_TYPE {
	HERO, ENEMY, HOSTAGE
};

enum CHARACTER_STATE {
	LIVE, DEAD, END
};

/// \brief Class to manage the character
class Character
{

  private:
    /// \brief protected method to copy a character
    /// this method is used in assingnment operator and copy constructor
    /// \param other source character to copy
    void          copy ( const Character& other );

  protected:
    /// \brief character name
    string            _name;
	/// \brief type of character
	CHARACTER_TYPE _type;
    /// \brief character initial coordenate (X, Y, Z)
    Ogre::Vector3     _v_pos;
    /// \brief health level with values between 0 and 100
    float             _health; // 0% - 100%
    /// \brief reference to scene manager
    Ogre::SceneManager* _sceneMgr;
    /// \brief reference to entity
    Ogre::Entity* _entity;
    /// \brief reference to scene node
    Ogre::SceneNode*  _node;
    /// \brief reference to rigid body
    OgreBulletDynamics::RigidBody* _rigidBody;
    /// \brief reference to current animation
    Ogre::AnimationState* _currentAnimation;
    /// \brief reference to entity DUMMY
    Ogre::Entity* _entityDummy;
    /// \brief reference to scene node DUMMY
    Ogre::SceneNode*  _nodeDummy;
	/// \brief reference to position of shot
    Ogre::SceneNode*  _nodeShot;
	/// \brief reference to shot of character
	Ogre::ManualObject* _shot;
    /// \brief visibility of character
    bool _visible;
	// \brief is shooting
	bool _isShooting;
    /// \brief World of fisic
    OgreBulletDynamics::DynamicsWorld* _world;
	/// \brief State of Character
	CHARACTER_STATE _stateCaracter;
	// \brief Sound of shoot
	SoundFXPtr                _soundShootFX;
	/// \brief reference to sound alert number 1
    SoundFXPtr                _soundDeath1FX;
    /// \brief reference to sound alert number 1
    SoundFXPtr                _soundDeath2FX;
    /// \brief reference to sound alert number 1
    SoundFXPtr                _soundDeath3FX;
	/// \brief reference to particle with character dead
	Ogre::ParticleSystem* _particleDeath;
	/// \brief reference to node of particle with character dead
	Ogre::SceneNode* _particleDeathNode;
	double _timerParticleDeath;
	/// \brief distance with other character when this character shoot to other character
	double _distanceWithOtherCaracter;

	/// \brief method to get scene node of shot reference
    /// \return scene node reference
	inline Ogre::SceneNode* getNodeShot() const { return _nodeShot; };
	// \brief method to detecting collisions between Shot whit characteres
    /// \param world reference to world (OgreBullet)
    /// \param characteres List of characteres
	/// \param characterCollision Character collisions shot
    /// \return if collition or not
    bool detectCollisionShot(OgreBulletDynamics::DynamicsWorld* world,
												std::vector<Character*> characteres,
												Character** characterCollision);
	// \brief method to scale shot
    /// \param scaleZ Scale of shot
	void setScaleShot (Ogre::Real scaleZ);

	/// \brief method to play the sound associated to the death of an character
    void                            play_sound_death();

	/// \brief method to death the caracter
    void  death();
	 /// \brief method to validate the success rate to shoot and hurt our hero or fail
    /// This method perform the next:
    /// a) Generate a random number between 1 and 100
    /// b) According the distance, the farther will be more difficult to hit the hero
    /// Joining the random value and the distance obtaining the result
    /// \param distance distance between the enemy and the hero
    /// \param rate success rate returned by the method (only for info purposes)
    /// \return true/false if it was hit the shoot in the hero
    bool                            validate_success_rate ( double distance, double* rate );
	/// \brief method to update the shot
    void                  updateShot ( double timeSinceLastFrame, std::vector<Character*>   vCharacteres);
  public:
    /// \brief character constructor parametrized
    /// \param sceneMgr reference to scene manager (ogre)
    /// \param world reference to dynamic world (bullet)
    /// \param name name to identify the character
    /// \param v_pos initial position in coordenate X, Y, Z with object Ogre::Vector3
    /// \param pos_Y initial position in coordenate Y
    /// \param pos_Z initial position in coordenate Z
    /// \param type Type of character (to load the correct MESH file)
    /// \param animation Name of animation init
    Character ( Ogre::SceneManager* sceneMgr,
                OgreBulletDynamics::DynamicsWorld* world,
                const string& name,
                const Ogre::Vector3& v_pos,
                CHARACTER_TYPE type, string animation);
    /// \brief default destructor
    virtual ~Character();
    /// \brief copy constructor
    /// \param other source character to copy
    Character ( const Character& other );
    /// \brief assignment operator to copy another character
    /// \param other source character to copy
    Character& operator=(const Character& other);
    /// \brief method to perform the character walk movement ( forward or backward )
    /// This method uses OgreBullet to move the character
    /// \param reverse if value is true then wall to backward else will be forward
    /// \param velocity value to set the linear velocity of the character
    void                          walk ( bool reverse, float velocidad = VELOCIDAD );
    /// \brief method to rotate the character to the left
    void                          turn_left();
    /// \brief method to rotate the character to the right
    void                          turn_right();
    /// \brief method to return if the character is dead
    /// \return true/false according to the character was dead or not
    inline bool                   is_death() const { return ( _health <= 0 ); };
    /// \brief method to get health level (0-100)
    /// \return float value with the health level
    inline float                  getHealth() const { return _health; };
    /// \brief method to set health level (0-100)
    /// \param newHealth float value to set
    void                          setHealth ( float newHealth );
    /// \brief method to get the character name
    /// \return character name
    inline string                 getName() const { return _name; };
    /// \brief method to set the character name
    /// \param newName new character name
    inline void                   setName ( const string& newName ) { _name = newName; };
	/// \brief method to get the character type
    /// \return Type of charater
    inline CHARACTER_TYPE                 getType() const { return _type; };
	/// \brief method to get the State of character
    /// \return State of character
    inline CHARACTER_STATE                 getStateCharater() const { return _stateCaracter; };
	/// \brief method to get is visibility the character
    /// \return visibility
    inline bool                   isVisible() const { return _visible; };
    /// \brief method to set change visibility of character
    /// \param visible new visivility
    virtual void                  setVisible ( const bool visible );
    /// \brief method to set the coordenates ( X, Y, Z ) for the initial position
    /// \param newPos initial coordenate X, Y, Z
    inline void                   setInitial_Pos ( const Ogre::Vector3& newPos ) { _v_pos = newPos; };
    /// \brief method to get the coordenates ( X, Y, Z ) for the initial position
    /// \return initial coordenates X, Y, Z
    inline const Ogre::Vector3&   getInitial_Pos() const { return _v_pos; };
    /// \brief method to get the scene node reference
    /// \return scene node reference
    inline Ogre::SceneNode*       getSceneNode() const { return _node; };
    /// \brief method to get rigid body reference
    /// \return rigid body reference
    inline OgreBulletDynamics::RigidBody* getRigidBody() const { return _rigidBody; };
    /// \brief method to get entity reference
    /// \return entity reference
    inline Ogre::Entity*          getEntity() const { return _entity; };
    /// \brief method to get entity dummy for camera POV
    /// \return entity reference
    inline Ogre::Entity*          getEntityDummy() const { return _entityDummy; };
    /// \brief method to print character info
    void                          print();
    /// \brief method to stop character movement
    void                          stop_move();
    /// \brief method to update character in frame
    virtual void                  update ( double timeSinceLastFrame, std::vector<Character*>   vCharacteres);
    /// \brief method to change animation of character
    /// \param nameAnimation animation name to change
    virtual void                  changeAnimation ( const string& nameAnimation );
    /// \brief method to set visible the dummy entity or not
    /// \param show to show dummy or not
    virtual void                  showDummy ( bool show );
	/// \brief method shoot
	/// \distanceWithOtherCaracter distance with enemy
    void                          shoot(double distanceWithOtherCaracter = ZERO_DISTANCE);
	/// \brief method to get the position of character
    /// \return position of character
    const Ogre::Vector3&          getPosition();

};

#endif // CHARACTER_H
