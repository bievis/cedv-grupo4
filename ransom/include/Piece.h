#ifndef __PIECE_H__
#define __PIECE_H__

#include "EnemyRoute.h"
#include <deque>

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
    /// \brief method to get the enemy routes in this piece
    /// \return deque with the enemy routes in this piece
//    inline const std::deque<EnemyRoute*>& getEnemyRoutes() const { return m_routes; };

    inline unsigned int getNumEnemyRoutes() const { return m_routes.size(); };

    void addEnemyRoute ( const EnemyRoute& newRoute );

    EnemyRoute* getEnemyRoute ( unsigned int index );

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

};

#endif // __PIECE_H__
