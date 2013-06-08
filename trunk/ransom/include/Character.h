#ifndef CHARACTER_H
#define CHARACTER_H

#include "Utilities.h"
//#include "AdvancedOgreFramework.hpp"
#include <pthread.h>

using namespace std;

#define VELOCIDAD 10.0

/// \brief Class to manage the character
class Character
{
  public:
    /// \brief character constructor parametrized
    /// \param sceneMgr reference to scene manager (ogre)
    /// \param world reference to dynamic world (bullet)
    /// \param name name to identify the character
    /// \param pos_X initial position in coordenate X
    /// \param pos_Y initial position in coordenate Y
    /// \param pos_Z initial position in coordenate Z
    Character     ( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    float initial_pos_X,
                    float initial_pos_Y,
                    float initial_pos_Z );
    /// \brief default destructor
    virtual ~Character();
    /// \brief copy constructor
    /// \param other source character to copy
    Character     ( const Character& other );
    /// \brief assignment operator to copy another character
    /// \param other source character to copy
    Character&    operator=(const Character& other);
    /// \brief method to perform the character walk movement ( forward or backward )
    /// This method uses OgreBullet to move the character
    /// \param reverse if value is true then wall to backward else will be forward (by default)
    void          walk ( bool reverse = false );
    /// \brief method to perform the character walk movement to position
    /// This method uses OgreBullet to move the character
    /// \param pos destiny position to walk
    void          walk_to ( const Ogre::Vector3& pos );
    /// \brief method to rotate the character "angle" (radians)
    /// \param angle value in radians to rotate
    void          turn ( Ogre::Real angle );
    /// \brief method to return if the character is dead
    /// \return true/false according to the character was dead or not
    inline bool   is_death() const { return ( _health <= 0 ); };
    /// \brief method to get health level (0-100)
    /// \return float value with the health level
    inline float  getHealth() const { return _health; };
    /// \brief method to set health level (0-100)
    /// \param newHealth float value to set
    void          setHealth ( float newHealth );
    /// \brief method to get the character name
    /// \return character name
    inline string getName() const { return _name; };
    /// \brief method to set the character name
    /// \param newName new character name
    inline void   setName ( const string& newName ) { _name = newName; };
    /// \brief method to set the coordenate X for the initial position
    /// \param newX initial coordenate X
    inline void   setInitial_PosX ( float newX ) { _posX = newX; };
    /// \brief method to set the coordenate Y for the initial position
    /// \param newY initial coordenate Y
    inline void   setInitial_PosY ( float newY ) { _posY = newY; };
    /// \brief method to set the coordenate Z for the initial position
    /// \param newZ initial coordenate Z
    inline void   setInitial_PosZ ( float newZ ) { _posZ = newZ; };
    /// \brief method to get the coordenate X for the initial position
    /// \return initial coordenate X
    inline float  getInitial_PosX() const { return _posX; };
    /// \brief method to get the coordenate Y for the initial position
    /// \return initial coordenate Y
    inline float  getInitial_PosY() const { return _posY; };
    /// \brief method to get the coordenate Z for the initial position
    /// \return initial coordenate Z
    inline float  getInitial_PosZ() const { return _posZ; };
    /// \brief method to get the scene node reference
    /// \return scene node reference
    inline Ogre::SceneNode* getSceneNode() const { return _node; };
    /// \brief method to get rigid body reference
    /// \return rigid body reference
    inline OgreBulletDynamics::RigidBody* getRigidBody() const { return _rigidBody; };
    /// \brief method to get entity reference
    /// \return entity reference
    inline Ogre::Entity* getEntity() const { return _entity; };
    /// \brief method to print character info
    void          print();
    /// \brief method to stop character movement
    void          stop_move();

  protected:

    /// \brief protected method to copy a character
    /// this method is used in assingnment operator and copy constructor
    /// \param other source character to copy
    void          copy ( const Character& other );

    /// \brief character name
    string            _name;
    /// \brief initial coordenate X
    float             _posX;
    /// \brief initial coordenate Y
    float             _posY;
    /// \brief initial coordenate Z
    float             _posZ;
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

  private:
    /// \brief mutex for the method turn
    pthread_mutex_t   _mutex_turn;
    /// \brief mutex for the method walk
    pthread_mutex_t   _mutex_walk;

};

#endif // CHARACTER_H
