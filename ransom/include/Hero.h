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
    /// \param initial_pos initial position in coordenate (X, Y, Z)
    Hero     ( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    const Ogre::Vector3& initial_pos );
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
    /// \brief method to print hero info
    void          print();

  protected:
    /// \brief protected method to copy an hero
    /// this method is used in assingnment operator and copy constructor
    /// \param other source hero to copy
    void copy ( const Hero& source );

};

#endif // HERO_H
