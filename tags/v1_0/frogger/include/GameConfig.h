#ifndef __GAME_CONFIG__
#define __GAME_CONFIG__

#include <vector>
#include <iostream>
#include <GameConfigException.hpp>
#include <Level.hpp>

using namespace std;

class GameConfig {

 private:
  void copy ( const GameConfig& source );
 
 protected:
  std::vector<Level *> _vLevels;

 public:
  GameConfig();
  virtual ~GameConfig();

  GameConfig ( const GameConfig& source );
  GameConfig operator= ( const GameConfig& source );

  void clear();

  inline unsigned int getNumLevels() const { return _vLevels.size(); };

  //Index vendrá dado entre 1 y N
  void getLevel ( unsigned int index, Level& level );

  void addLevel ( Level& newLevel );
 
  void print();
};



#endif
