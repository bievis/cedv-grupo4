#ifndef CHARACTER_H
#define CHARACTER_H

#include "Utilities.h"
//#include "AdvancedOgreFramework.hpp"
//#include <pthread.h>

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

    void          move ( float posX, float posY, float posZ );
    void          is_death();

    float         getHealth() const;
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

    void          print();

  protected:

    void          copy ( const Character& other );

  private:
    string            m_name;
    float             m_posX;
    float             m_posY;
    float             m_posZ;
    float             m_health; // 0% - 100%

    Ogre::SceneNode*  m_node;

//    pthread_mutex_t   m_mutex;

};

#endif // CHARACTER_H
