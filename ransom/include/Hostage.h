#ifndef HOSTAGE_H
#define HOSTAGE_H

#include <Character.h>

class Hostage : public Character
{
  public:
    /// \brief Hostage constructor parametrized
    /// \param sceneMgr reference to scene manager (ogre)
    /// \param world reference to dynamic world (bullet)
    /// \param name name to identify the Hostage
    /// \param initia_pos initial position in coordenate (X, Y, Z)
    Hostage     ( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    const Ogre::Vector3& initial_pos );
    /** Default destructor */
    virtual ~Hostage();
    /** Copy constructor
     *  \param other Object to copy from
     */
    Hostage(const Hostage& other);
    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    Hostage& operator=(const Hostage& other);
    /// \brief method to print hero info
    void          print();
	/// \brief method to change animation of character
    void          changeAnimation(string nameAnimation);
  protected:
    /// \brief protected method to copy an hero
    /// this method is used in assingnment operator and copy constructor
    /// \param other source hero to copy
    void copy ( const Hostage& source );

  private:
};

#endif // HOSTAGE_H
