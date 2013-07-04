#ifndef __PIECE_H__
#define __PIECE_H__

#include "EnemyRoute.h"
#include <deque>
#include <string>

/// \brief Class to manage the map pieces
class Piece
{

  public:
    /// \brief Default constructor
    Piece();
    /// \brief Default destructor
    virtual ~Piece();
    /// Copy constructor
    /// \param other Object to copy from
    Piece ( const Piece& other );
    /// Assignment operator
    /// \param other Object to assign from
    /// \return A reference to this
    Piece& operator=(const Piece& rhs);

    /// \brief method to get the mesh file name
    /// \return mesh file name
    inline const std::string& getNameMeshFile() const { return m_name_mesh; };
    /// \brief method to set the mesh file name
    /// \param newName new mesh file name
    inline void setNameMeshFile ( const std::string& newName ) { m_name_mesh = newName; };
    /// \brief method to get the piece position in the map
    /// \return piece position
    inline const Ogre::Vector3& getPosition() const { return m_pos; };
    /// \brief method to set the piece position in the map
    /// \param val new position to set
    inline void setPosition (Ogre::Vector3 val) { m_pos = val; };
    /// \brief method to get the piece width
    /// \return piece width
    inline double getWidth() const { return m_width; };
    /// \brief method to set the piece width
    /// \param val new piece width
    inline void setWidth ( double val ) { m_width = val; };
    /// \brief method to get the piece height
    /// \return piece height
    inline double getHeight() const { return m_height; };
    /// \brief method to set the piece height
    /// \param val new piece height
    inline void setHeight ( double val ) { m_height = val; };
    /// \brief method to get the number of enemy routes in the queue
    /// \return number of enemy routes
    inline unsigned int getNumEnemyRoutes() const { return m_routes.size(); };
    /// \brief method to add an enemy route
    /// \param newRoute new route to add
    void addEnemyRoute ( const EnemyRoute& newRoute );
    /// \brief method to get the enemy routes in this piece
    /// \param index element index to get from the queue
    /// \return reference to the enemy route
    EnemyRoute* getEnemyRoute ( unsigned int index );
	/// \brief method to get the number of pisitions in the queue
    /// \return number of positions of hostages
    inline unsigned int getNumPosHostages() const { return m_posHostages.size(); };
    /// \brief method to add a position of hostage
    /// \param newPos new position to add
    void addPosHostage ( const Ogre::Vector3& newPos );
    /// \brief method to get the position of hostage in this piece
    /// \param index element index to get from the queue
    /// \return reference to the position of hostage
    Ogre::Vector3 getPosHostage ( unsigned int index );

    /// \brief method to print info over the piece
    void print();
    /// \brief method to clear the piece
    void clear();

  protected:
    /// \brief Method to copy the piece
    /// \param source Object to copy from
    void copy ( const Piece& source );

  private:
    /// \brief piece position
    Ogre::Vector3 m_pos;
    /// \brief piece width
    double m_width;
    /// \brief piece height
    double m_height;
    /// \brief deque with the enemy routes in this piece
    std::deque<EnemyRoute*> m_routes;
	/// \brief deque with the position of hostages in the piece
	std::deque<Ogre::Vector3> m_posHostages;
    /// \brief name mesh file to load
    std::string m_name_mesh;

};

#endif // __PIECE_H__
