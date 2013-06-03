#ifndef CHARACTER_H
#define CHARACTER_H

#include "Utilities.h"
#include "AdvancedOgreFramework.hpp"
#include <pthread.h>

class Character
{
  public:
    Character ( Ogre::SceneManager* sceneMgr, OgreBulletDynamics::DynamicsWorld* world, const string& name );
    virtual ~Character();
    Character(const Character& other);

    Character&    operator=(const Character& other);

    void          move ( float posX, float posY, float posZ );
    void          is_death();

    float         getHealth() const;
    void          setHealth ( float newHealth );

    inline string getName() const { return m_name; };
    inline void   setName ( const string& newName ) { m_name = newName; };

//    inline void   setPosX ( float newX ) { m_posX = newX; };
//    inline void   setPosY ( float newY ) { m_posY = newY; };
//    inline void   setPosZ ( float newZ ) { m_posZ = newZ; };
//
//    inline float  getPosX() const { return m_posX; };
//    inline float  getPosY() const { return m_posY; };
//    inline float  getPosZ() const { return m_posZ; };

    void          print();

  protected:

    void          copy ( const Character& other );

  private:
    string            m_name;
//    float             m_posX;
//    float             m_posY;
//    float             m_posZ;
    float             m_health; // 0% - 100%

    Ogre::SceneNode*  m_node;

//    pthread_mutex_t   m_mutex;

};

#endif // CHARACTER_H
