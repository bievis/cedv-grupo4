#ifndef ENEMYROUTE_H
#define ENEMYROUTE_H

#include <OgreVector3.h>

class EnemyRoute
{
  public:
    /** Default constructor */
    EnemyRoute();
    /** Default destructor */
    virtual ~EnemyRoute();
    /** Copy constructor
     *  \param other Object to copy from
     */
    EnemyRoute(const EnemyRoute& other);
    /** Assignment operator
     *  \param other Object to assign from
     *  \return A reference to this
     */
    EnemyRoute& operator=(const EnemyRoute& other);
    /** Access _closed
     * \return The current value of _closed
     */
    inline bool getRouteClosed() const { return _closed; }
    /** Set _closed
     * \param newValue New value to set
     */
    inline void setRouteClosed ( bool newValue ) { _closed = newValue; };

    inline unsigned int getID() const { return _id; };

    inline void setID ( unsigned int newValue ) { _id = newValue; };

    inline unsigned int getNumPoints() const { return _vPoints.size(); };

    void getPoint ( unsigned int index, Ogre::Vector3& v );

    void addPoint ( const Ogre::Vector3& v );
    void clear();
    void print();

  protected:
    void copy ( const EnemyRoute& source );

  private:
    unsigned int _id;
    bool _closed;
    std::deque<Ogre::Vector3> _vPoints;
};

#endif // ENEMYROUTE_H
