#ifndef __GAME_CONFIG__
#define __GAME_CONFIG__

#include <vector>
#include <iostream>
#include <GameConfigException.hpp>
#include <EnemyRoute.h>

using namespace std;

class GameConfig {

 private:
  void copy ( const GameConfig& source );

 protected:
  unsigned int _numEnemies;
  Ogre::Vector3 _initialPos_Hero;
  std::vector<EnemyRoute*> _vEnemyRoutes;

 public:
  GameConfig();
  virtual ~GameConfig();

  GameConfig ( const GameConfig& source );
  GameConfig operator= ( const GameConfig& source );

  void clear();

  inline unsigned int getNumEnemyRoutes() const { return _vEnemyRoutes.size(); };

  //Index vendrá dado entre 1 y N
  const EnemyRoute& getEnemyRoute ( unsigned int index ) const;

  void addEnemyRoute ( const EnemyRoute& newRoute );

  //inline void getInitialPosHero ( Ogre::Vector3& v ) { v = _initialPos_Hero; };
  inline const Ogre::Vector3& getInitialPosHero() const { return _initialPos_Hero; };
  inline void setInitialPosHero ( const Ogre::Vector3& pos ) { _initialPos_Hero = pos; };

  inline unsigned int getNumEnemies() const { return _numEnemies; };
  inline void setNumEnemies ( unsigned int newValue ) { _numEnemies = newValue; };

  void print();
};



#endif
