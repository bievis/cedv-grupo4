#ifndef CHARACTER_H
#define CHARACTER_H

#include "Utilities.h"
#include <OgreTextureManager.h>
//#include "AdvancedOgreFramework.hpp"
#include <pthread.h>
#include "MyTextureListener.h"

using namespace std;

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
    /// \param color character color
    /// \param checkingObjects enable/disable everything related to render to texture
    Character     ( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    float initial_pos_X,
                    float initial_pos_Y,
                    float initial_pos_Z,
                    eColor color,
                    bool checkingObjects );
    /// \brief default destructor
    virtual ~Character();
    /// \brief copy constructor
    /// \param other source character to copy
    Character     ( const Character& other );
    /// \brief assignment operator to copy another character
    /// \param other source character to copy
    Character&    operator=(const Character& other);
    /// \brief method to move the character to the position (X,Y,Z)
    /// This method uses Ogre to move the character
    /// \param posX destiny position in coordenate X
    /// \param posY destiny position in coordenate Y
    /// \param posZ destiny position in coordenate Z
    void          move_to ( float posX, float posY, float posZ );
    /// \brief method to perform the character walk movement ( forward or backward )
    /// This method uses OgreBullet to move the character
    /// \param reverse if value is true then wall to backward else will be forward (by default)
    void          walk ( bool reverse = false );
    /// \brief method to rotate the character "angle" (radians)
    /// \param angle value in radians to rotate
    void          turn ( Ogre::Real angle );
    /// \brief method to return if the character is dead
    /// \return true/false according to the character was dead or not
    inline bool   is_death() const { return ( m_health <= 0 ); };
    /// \brief method to get health level (0-100)
    /// \return float value with the health level
    inline float  getHealth() const { return m_health; };
    /// \brief method to set health level (0-100)
    /// \param newHealth float value to set
    void          setHealth ( float newHealth );
    /// \brief method to get the character name
    /// \return character name
    inline string getName() const { return m_name; };
    /// \brief method to set the character name
    /// \param newName new character name
    inline void   setName ( const string& newName ) { m_name = newName; };
    /// \brief method to set the coordenate X for the initial position
    /// \param newX initial coordenate X
    inline void   setInitial_PosX ( float newX ) { m_posX = newX; };
    /// \brief method to set the coordenate Y for the initial position
    /// \param newY initial coordenate Y
    inline void   setInitial_PosY ( float newY ) { m_posY = newY; };
    /// \brief method to set the coordenate Z for the initial position
    /// \param newZ initial coordenate Z
    inline void   setInitial_PosZ ( float newZ ) { m_posZ = newZ; };
    /// \brief method to get the coordenate X for the initial position
    /// \return initial coordenate X
    inline float  getInitial_PosX() const { return m_posX; };
    /// \brief method to get the coordenate Y for the initial position
    /// \return initial coordenate Y
    inline float  getInitial_PosY() const { return m_posY; };
    /// \brief method to get the coordenate Z for the initial position
    /// \return initial coordenate Z
    inline float  getInitial_PosZ() const { return m_posZ; };
    /// \brief method to get the scene node reference
    /// \return scene node reference
    inline Ogre::SceneNode* getSceneNode() const { return m_node; };
    /// \brief method to get rigid body reference
    /// \return rigid body reference
    inline OgreBulletDynamics::RigidBody* getRigidBody() const { return m_rigidBody; };
    /// \brief method to get texture pointer
    /// this object inherits from SharedPtr
    /// \return texture pointer
    inline Ogre::TexturePtr& getTexturePtr() { return m_rtt; };
    /// \brief method to get render to texture reference
    /// \return render to texture reference
    inline Ogre::RenderTexture* getRenderTexture() const { return m_rtex; };
    /// \brief method to get the camera POV reference
    /// \return camera reference
    inline Ogre::Camera* getCameraPOV() const { return m_camPOV; };
    /// \brief method to get entity reference
    /// \return entity reference
    inline Ogre::Entity* getEntity() const { return m_entity; };
    /// \brief method to print character info
    void          print();
    /// \brief this method is used to check if anybody is in our camera scope
    /// \return true/false according to see or not to see anybody
    bool          haveYouSeenAnybody() const;

  protected:

    /// \brief protected method to copy a character
    /// this method is used in assingnment operator and copy constructor
    /// \param other source character to copy
    void          copy ( const Character& other );

  private:
    /// \brief character name
    string            m_name;
    /// \brief initial coordenate X
    float             m_posX;
    /// \brief initial coordenate Y
    float             m_posY;
    /// \brief initial coordenate Z
    float             m_posZ;
    /// \brief health level with values between 0 and 100
    float             m_health; // 0% - 100%
    /// \brief reference to scene manager
    Ogre::SceneManager* m_sceneMgr;
    /// \brief reference to entity
    Ogre::Entity* m_entity;
    /// \brief reference to scene node
    Ogre::SceneNode*  m_node;
    /// \brief reference to rigid body
    OgreBulletDynamics::RigidBody* m_rigidBody;

    // Para el render a textura
    /// \brief centinel to add the render to texture
    bool m_checkingObjects;
    /// \brief texture pointer
    Ogre::TexturePtr m_rtt;
    /// \brief reference to render to texture object
    Ogre::RenderTexture* m_rtex;
    /// \brief reference to camera used to character POV
    Ogre::Camera* m_camPOV;
    /// \brief reference to texture listener
    MyTextureListener* m_textureListener;
    /// \brief mutex for the method move_to
    pthread_mutex_t   m_mutex_move;
    /// \brief mutex for the method turn
    pthread_mutex_t   m_mutex_turn;
    /// \brief mutex for the method walk
    pthread_mutex_t   m_mutex_walk;

};

#endif // CHARACTER_H