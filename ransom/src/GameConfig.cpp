#include <GameConfig.h>

// ## Class Game Config ##

GameConfig::GameConfig()
  {
  }

void GameConfig::clear()
  {
    // for ( unsigned int i = 0; i < _vLevels.size(); i++ )
    //   delete _vLevels[i];

    // _vLevels.clear();
  }

GameConfig::~GameConfig()
  {
    clear();
  }

GameConfig::GameConfig ( const GameConfig &source )
  {
    copy ( source );
  }

GameConfig GameConfig::operator= ( const GameConfig &source )
  {
    copy ( source );
    return *this;
  }

void GameConfig::copy ( const GameConfig &source )
  {
    // Level lvl;

    // clear();

    // for ( unsigned int i = 0; i < source.getNumLevels(); i++ )
    //   {
    //     _vLevels[i] = source._vLevels[i];
    //   }
  }

// void GameConfig::getLevel ( unsigned int index, Level& level )
//   {
//     if ( ( index <= _vLevels.size() ) && ( index > 0 ) )
//       {
//     	if ( _vLevels[index-1] )
//     	  {
//             level = *(_vLevels[index-1]);
//     	  }
//     	else
//     	  {
//     	    throw GameConfigException ( "El nivel es un valor vacío" );
//     	  }
//       }
//     else
//       {
//     	char cad[100];
//     	sprintf ( cad, "El nivel debe estar entre 1 y %ld", _vLevels.size() );
//     	throw GameConfigException ( cad );
//       }
//   }

// void GameConfig::addLevel ( Level& newLevel )
//   {
//     Level *newLv = new Level ( newLevel );
//     _vLevels.push_back ( newLv );
//   }

void GameConfig::print()
  {
    // cout << "==================" << endl;
    // cout << "Game Configuration" << endl;


    // for ( unsigned int i = 0; i < _vLevels.size(); i++ )
    //   {
    //     cout << "==================" << endl;
    // 	cout << " - Level " << i+1 << endl;
    //     cout << "==================" << endl;
    // 	_vLevels[i]->print();
    //   }

    // cout << "==================" << endl;
  }
