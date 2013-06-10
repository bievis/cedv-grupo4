#ifndef HERO_H
#define HERO_H

#include <Character.h>

class Hero : public Character
{
  public:
    /// \brief Hero constructor parametrized
    /// \param sceneMgr reference to scene manager (ogre)
    /// \param world reference to dynamic world (bullet)
    /// \param name name to identify the Hero
    /// \param pos_X initial position in coordenate X
    /// \param pos_Y initial position in coordenate Y
    /// \param pos_Z initial position in coordenate Z
    Hero     ( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    float initial_pos_X,
                    float initial_pos_Y,
                    float initial_pos_Z );
    /** Default destructor */
    virtual ~Hero();
    /** Copy constructor
     *  \param other Object to copy from
     */
    Hero(const Hero& other);
    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    Hero& operator=(const Hero& other);
    /// \brief method to get the scene node reference
    /// \return scene node reference
    inline Ogre::SceneNode* getSceneNodeDUMMY() const { return _node; };
    /// \brief method to get entity reference
    /// \return entity reference
    inline Ogre::Entity* getEntityDUMMY() const { return _entity; };
    /// \brief method to print hero info
    void          print();

  protected:
    /// \brief protected method to copy an hero
    /// this method is used in assingnment operator and copy constructor
    /// \param other source hero to copy
    void copy ( const Hero& source );

  private:
      /// \brief reference to entity DUMMY
    Ogre::Entity* _entityDummy;
    /// \brief reference to scene node DUMMY
    Ogre::SceneNode*  _nodeDummy;

};

#endif // HERO_H
