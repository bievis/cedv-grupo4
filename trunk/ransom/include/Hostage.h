#ifndef HOSTAGE_H
#define HOSTAGE_H

#include <Character.h>

/// \brief timer show the hostage when is liberate
#define TIMER_PATICLE_LIBERATE 2.0

enum STATE_HOSTAGE {
	LIBERATE, CAPTURE, LIBERATED
};

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
	/// \brief method to get state of hostage
    /// \return state of hostage
    inline STATE_HOSTAGE  getState() const { return _state; };
    /// \brief method to print hero info
    void          print();
	/// \brief method to change animation of character
    void          changeAnimation(string nameAnimation);
	/// \brief method to change state to liberate
    void liberate();
	/// \brief method to set change visibility of hostage
    /// \param visible new visivility
    void   setVisible ( const bool visible );
	/// \brief method to update character in frame
    void update(double timeSinceLastFrame);
  protected:
    /// \brief protected method to copy an hero
    /// this method is used in assingnment operator and copy constructor
    /// \param other source hero to copy
    void copy ( const Hostage& source );

  private:
	Ogre::ParticleSystem* _particleLiberation;
	Ogre::SceneNode* _particleLiberationNode;
	STATE_HOSTAGE _state;
};

#endif // HOSTAGE_H
