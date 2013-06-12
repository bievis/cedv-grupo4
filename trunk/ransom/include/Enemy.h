#ifndef ENEMY_H
#define ENEMY_H

#include <Character.h>
#include <OgreTextureManager.h>
#include "MyTextureListener.h"

#define SIZE_LIFE_BAR 2.0f

class Enemy : public Character
{
  public:
    /// \brief Enemy constructor parametrized
    /// \param sceneMgr reference to scene manager (ogre)
    /// \param world reference to dynamic world (bullet)
    /// \param name name to identify the Enemy
    /// \param pos_X initial position in coordenate X
    /// \param pos_Y initial position in coordenate Y
    /// \param pos_Z initial position in coordenate Z
    Enemy     ( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    float initial_pos_X,
                    float initial_pos_Y,
                    float initial_pos_Z );
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
    inline Ogre::TexturePtr& getTexturePtr() { return _rtt; };
    /// \brief method to get render to texture reference
    /// \return render to texture reference
    inline Ogre::RenderTexture* getRenderTexture() const { return _rtex; };
    /// \brief method to get the camera POV reference
    /// \return camera reference
    inline Ogre::Camera* getCameraPOV() const { return _camPOV; };
    /// \brief this method is used to check if anybody is in our camera scope
    /// \return true/false according to see or not to see anybody
    bool          haveYouSeenAnybody() const;
    /// \brief method to print enemy info
    void          print();
    /// \brief method to update lifeBar
    void updateLifeBar();
    /// \brief method to update enemy in frame
    void          update(double timeSinceLastFrame);

  protected:
    /// \brief protected method to copy a enemy
    /// this method is used in assingnment operator and copy constructor
    /// \param other source enemy to copy
    void          copy ( const Enemy& source );

  private:
    /// \brief texture pointer
    Ogre::TexturePtr _rtt;
    /// \brief reference to render to texture object
    Ogre::RenderTexture* _rtex;
    /// \brief reference to camera used to enemy POV
    Ogre::Camera* _camPOV;
    /// \brief reference to texture listener
    MyTextureListener* _textureListener;
    /// \brief reference to BillboardSet of Lifebar
    Ogre::BillboardSet* _bbSetLife;
    /// \brief reference to Billboard of Lifebar
    Ogre::Billboard* _lifeBar;
    /// \brief reference to node of Lifebar
    Ogre::SceneNode* _lifeNode;
};

#endif // ENEMY_H
