#ifndef __GAME_CONFIG__
#define __GAME_CONFIG__

#include <vector>
#include <iostream>
#include <GameConfigException.hpp>
#include <EnemyRoute.h>
#include <OgreStringConverter.h>
#include <Piece.h>

using namespace std;

/// \brief Class to manage the game configuration
/// Hostages, Enemies & Hero config
class GameConfig {

 private:
  /// \brief Method to copy object
  /// \param source source object to copy
  void copy ( const GameConfig& source );

 protected:
  /// \brief enemy number to view
  unsigned int _numEnemies;
  /// \brief initial position point for the hero
  Ogre::Vector3 _initialPos_Hero;
  /// \brief set of enemy routes
  std::vector<EnemyRoute*> _vEnemyRoutes;
  /// \brief set of hostages positions
  std::deque<Ogre::Vector3> _vPositionHostages;
  /// \brief plane height
  unsigned int _planeHeight;
  /// \brief plane width
  unsigned int _planeWidth;
  /// \brief the pieces which compounds the map
  std::deque<Piece *> _pieces;

 public:
  /// \brief default constructor
  GameConfig();
  /// \brief default destructor
  virtual ~GameConfig();
  /// \brief copy constructor
  /// \param source source object to copy
  GameConfig ( const GameConfig& source );
  /// \brief assignment operator
  /// \param source source object to copy
  GameConfig operator= ( const GameConfig& source );
  /// \brief method to clear the object
  /// free the vector and deque, also clear the elements in the object
  void clear();
  /// \brief method to get routes vector size
  /// \return number of routes
  inline unsigned int getNumEnemyRoutes() const { return _vEnemyRoutes.size(); };
  /// \brief method to get the enemy route assigned with identificator (index)
  /// \param index identificator route to get
  /// \return route associated to identificator
  const EnemyRoute& getEnemyRoute ( unsigned int index ) const;
  /// \brief method to add an enemy route
  /// \param newRoute new route to add
  void addEnemyRoute ( const EnemyRoute& newRoute );
  /// \brief method to get initial position of the hero
  /// \return initial position of the hero
  inline const Ogre::Vector3& getInitialPosHero() const { return _initialPos_Hero; };
  /// \brief method to set initial position of the hero
  /// \param pos initial position of the hero
  inline void setInitialPosHero ( const Ogre::Vector3& pos ) { _initialPos_Hero = pos; };
  /// \brief method to get enemies number
  /// \return enemies number
  inline unsigned int getNumEnemies() const { return _numEnemies; };
  /// \brief method to set enemies number
  /// \param newValue new enemies number to set
  inline void setNumEnemies ( unsigned int newValue ) { _numEnemies = newValue; };
  /// \brief method to get hostages number
  /// \return hostages number
  inline unsigned int getNumHostages() const { return _vPositionHostages.size(); };
  /// \brief method to get the position of the hostage(index)
  /// \param index the hostage index to find in the deque
  const Ogre::Vector3& getPositionHostage ( unsigned int index ) const;
  /// \brief method to add a hostage position, namely, a hostage
  /// \param p hostage position
  void addHostagePosition ( const Ogre::Vector3& p );
  /// \brief method to get the plane height
  /// \return plane height
  inline unsigned int getPlaneHeight() { return _planeHeight; };
  /// /brief method to set the plane height
  /// /param newValue new plane height to set
  inline void setPlaneHeight ( unsigned int newValue ) { _planeHeight = newValue; };
  /// \brief method to get the plane width
  /// \return plane width
  inline unsigned int getPlaneWidth() { return _planeWidth; };
  /// /brief method to set the plane width
  /// /param newValue new plane width to set
  inline void setPlaneWidth ( unsigned int newValue ) { _planeWidth = newValue; };
  /// \brief method to print info
  void print();
  /// \brief method to add a map piece
  /// \param newPiece new piece to add
  void addPiece ( const Piece& newPiece );
  /// \brief method to get pieces queue
  /// \return pieces queue
  inline const std::deque<Piece *>& getPieces() const { return _pieces; };

};

#endif
