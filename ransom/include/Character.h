#ifndef CHARACTER_H
#define CHARACTER_H

#include "Utilities.h"
#include <OgreTextureManager.h>
//#include "AdvancedOgreFramework.hpp"
#include <pthread.h>

using namespace std;

class Character
{
  public:
    Character ( Ogre::SceneManager* sceneMgr,
                OgreBulletDynamics::DynamicsWorld* world,
                const string& name,
                float initial_pos_X,
                float initial_pos_Y,
                float initial_pos_Z,
                eColor color );
    virtual ~Character();
    Character(const Character& other);

    Character&    operator=(const Character& other);

    void          move_to ( float posX, float posY, float posZ );
    void          walk ( bool reverse = false );
    void          turn ( Ogre::Real angle );

    inline bool   is_death() const { return ( m_health <= 0 ); };

    inline float  getHealth() const { return m_health; };
    void          setHealth ( float newHealth );

    inline string getName() const { return m_name; };
    inline void   setName ( const string& newName ) { m_name = newName; };

    inline void   setInitial_PosX ( float newX ) { m_posX = newX; };
    inline void   setInitial_PosY ( float newY ) { m_posY = newY; };
    inline void   setInitial_PosZ ( float newZ ) { m_posZ = newZ; };

    inline float  getInitial_PosX() const { return m_posX; };
    inline float  getInitial_PosY() const { return m_posY; };
    inline float  getInitial_PosZ() const { return m_posZ; };

    inline Ogre::SceneNode* getSceneNode() const { return m_node; };
    inline OgreBulletDynamics::RigidBody* getRigidBody() const { return m_rigidBody; };
//    inline Ogre::TexturePtr& getTexturePtr() { return m_rtt; };
//    inline Ogre::RenderTexture* getRenderTexture() const { return m_rtex; };
//    inline Ogre::Camera* getCameraPOV() const { return m_camPOV; };
    inline Ogre::Entity* getEntity() const { return m_entity; };

    void          print();

//    bool          check_vision();

  protected:

    void          copy ( const Character& other );

  private:
    string            m_name;
    float             m_posX;
    float             m_posY;
    float             m_posZ;
    float             m_health; // 0% - 100%

    Ogre::SceneManager* m_sceneMgr;

    Ogre::Entity* m_entity;
    Ogre::SceneNode*  m_node;
    OgreBulletDynamics::RigidBody* m_rigidBody;

    // Para el render a textura
//    Ogre::TexturePtr m_rtt;
//    Ogre::RenderTexture* m_rtex;
//    Ogre::Camera* m_camPOV;

    pthread_mutex_t   m_mutex_move;
    pthread_mutex_t   m_mutex_turn;
    pthread_mutex_t   m_mutex_walk;

};

#endif // CHARACTER_H
