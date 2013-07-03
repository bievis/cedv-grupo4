#ifndef PIECE_H
#define PIECE_H

#include "EnemyRoute.h"
#include <deque>

class Piece
{

  public:
    Piece();
    virtual ~Piece();
    Piece ( const Piece& other );

    Piece& operator=(const Piece& rhs);

    inline const Ogre::Vector3& getPosition() const { return m_pos; };
    inline void setPosition (Ogre::Vector3 val) { m_pos = val; };
    inline double getWidth() const { return m_width; };
    inline void setWidth ( double val ) { m_width = val; };
    inline double getHeight() const { return m_height; };
    inline void setHeight ( double val ) { m_height = val; };

    const std::deque<EnemyRoute*>& getEnemyRoutes() const;

    void print();

    void clear();

  protected:
    void copy ( const Piece& source );

  private:
    Ogre::Vector3 m_pos;
    double m_width;
    double m_height;

    std::deque<EnemyRoute*> m_routes;

};

#endif // PIECE_H
