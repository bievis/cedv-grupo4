#ifndef __GAME_CONFIG__
#define __GAME_CONFIG__

#include <vector>
#include <iostream>
#include <GameConfigException.hpp>
#include <EnemyRoute.h>
#include <OgreStringConverter.h>
#include <Piece.h>

#define SIZE_PART 32.0f

struct ST_PART_MAP {
	Piece *pPiece;
	Ogre::Vector3 pos;
};

typedef struct ST_PART_MAP T_PART_MAP;

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
  /// \brief rows of map
  unsigned int _rowsMap;
  /// \brief cols map
  unsigned int _colsMap;
  /// \brief the pieces which compounds the map
  std::deque<Piece *> _pieces;
  /// \brief the pieces which compounds the map
  std::vector<T_PART_MAP> _piecesMap;

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
  /// \brief method to get the rows Map
  /// \return rows Map
  inline unsigned int getRowsMap() { return _rowsMap; };
  /// /brief method to set the rows Map
  /// /param newValue new rows Map to set
  inline void setRowsMap ( unsigned int rowsMap ) { _rowsMap = rowsMap; };
  /// \brief method to get the cols Map
  /// \return cols Map
  inline unsigned int getColsMap() { return _colsMap; };
  /// /brief method to set the cols Map
  /// /param newValue new cols Map to set
  inline void setColsMap ( unsigned int colsMap ) { _colsMap = colsMap; };
  /// \brief method to print info
  void print();
  /// \brief method to add a map piece
  /// \param newPiece new piece to add
  void addPiece ( const Piece& newPiece );
  /// \brief method to create Map random
  void createMapRandom ();
  /// \brief method to get the number of pieces
  /// \return number of pieces
  inline unsigned int getNumPieces() const { return _pieces.size(); };
  /// \brief method to get piece of config
  /// \param index identificator piece to get
  /// \return piece associated to identificator
  Piece* getPiece ( unsigned int index );
  /// \brief method to get the number of piece of map
  /// \return number of pieces of map
  T_PART_MAP getPieceMap ( unsigned int index );
  /// \brief method to get piece of map
  /// \param index identificator piece of map to get
  /// \return piece of map associated to identificator
  inline unsigned int getNumPiecesMap() const { return _piecesMap.size(); };
};

#endif
