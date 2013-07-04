#ifndef ENEMYROUTE_H
#define ENEMYROUTE_H

#include <OgreVector3.h>

/// \brief Class to manage the enemy route
class EnemyRoute
{
  public:
    /// \brief Default constructor
    EnemyRoute();
    /// \brief Default destructor
    virtual ~EnemyRoute();
    /// Copy constructor
    /// \param other Object to copy from
    EnemyRoute ( const EnemyRoute& other );
    /// Assignment operator
    /// \param other Object to assign from
    /// \return A reference to this
    EnemyRoute& operator= ( const EnemyRoute& other );
    /// \brief Method to get value of route closed.
    /// \return The current value of _closed
    inline bool getRouteClosed() const { return _closed; }
    /// \brief Method to set value of route closed.
    /// \param newValue New value to set
    inline void setRouteClosed ( bool newValue ) { _closed = newValue; };
    /// \brief Method to get value identificator for the route
    /// \return The current value of _id
    inline unsigned int getID() const { return _id; };
    /// \brief Method to set value identificator for the route
    /// \param newValue New value to set
    inline void setID ( unsigned int newValue ) { _id = newValue; };
    /// \brief Method to get number of points in the route
    /// \return Number of points
    inline int getNumPoints() const { return (int)(_vPoints.size()); };
    /// \brief Method to get proximity value
    /// \return The current value of _proximity
    inline float getProximity() const { return _proximity; };
	/// \brief Method to set point in the list
    /// \param newPos New point to set
	/// \param index Index of new point
	void setPoint ( Ogre::Vector3& newPos, unsigned int index );
    /// \brief Method to set proximity value
    /// \param newValue New value to set
    inline void setProximity ( float newValue ) { _proximity = newValue; };
    /// \brief Method to get the point acoording the index value in the points vector
    /// \return Point selected
    const Ogre::Vector3& getPoint ( unsigned int index ) const;
    /// \brief Method to add a point to the vector
    /// \param point to add to the points vector
    void addPoint ( const Ogre::Vector3& v );
    /// \brief Method to clear class
    /// Free the vector and clear other elements
    void clear();
    /// \brief Method to print info over the route
    void print();

  protected:
    /// \brief Method to copy the route
    /// \param source Object to copy from
    void copy ( const EnemyRoute& source );

  private:
    /// \brief route identificator
    unsigned int _id;
    /// \brief value to obtain if a route is closed or not
    /// A route closed will be a route whitout end, namely, a route which the
    /// next point after the last point will be the first point
    bool _closed;
    /// \brief Proximity value indicates how near will pass the character to the point to set a visited this
    float _proximity;
    /// \brief Points vector for the route
    std::vector<Ogre::Vector3> _vPoints;
};

#endif // ENEMYROUTE_H
