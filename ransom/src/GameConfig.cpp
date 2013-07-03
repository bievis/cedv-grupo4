#include <GameConfig.h>

// ## Class Game Config ##

GameConfig::GameConfig()
  {
  }

void GameConfig::clear()
  {
    for ( unsigned int i = 0; i < _vEnemyRoutes.size(); i++ )
      {
        if ( _vEnemyRoutes[i] )
          delete _vEnemyRoutes[i];
      }

    _vEnemyRoutes.clear();

    _vPositionHostages.clear();

//    _vColumns.clear();
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
    if (this == &source) return *this; // handle self assignment

    copy ( source );

    return *this;
  }

void GameConfig::copy ( const GameConfig &source )
  {
    unsigned int i = 0;

    clear();

    _initialPos_Hero = source.getInitialPosHero();
    _numEnemies = source.getNumEnemies();

    for ( i = 0; i < source.getNumEnemyRoutes(); i++ )
      {
        _vEnemyRoutes[i] = source._vEnemyRoutes[i];
      }

    for ( i = 0; i < source.getNumHostages(); i++ )
      {
        _vPositionHostages[i] = source._vPositionHostages[i];
      }

    _planeHeight = source._planeHeight;
    _planeWidth = source._planeWidth;

//    for ( i = 0; i < source.getNumColumns(); i++ )
//      {
//        _vColumns[i] = source._vColumns[i];
//      }
  }

const EnemyRoute& GameConfig::getEnemyRoute ( unsigned int index ) const
  {

    if ( _vEnemyRoutes.size() > 0 )
      {
        for ( unsigned int i = 0; i < _vEnemyRoutes.size(); i++ )
        {
          cout << "_vEnemyRoutes[i]->getID() == " << _vEnemyRoutes[i]->getID() << endl;
          if ( _vEnemyRoutes[i]->getID() == index )
          {
            return *(_vEnemyRoutes[i]);
          }

        }
	    }

    char cad[100];
    sprintf ( cad, "El ID de la ruta %d no ha sido localizado.", index );
    throw GameConfigException ( cad );

  }

void GameConfig::addEnemyRoute ( const EnemyRoute& newRoute )
  {
    EnemyRoute* ptrNew = new EnemyRoute ( newRoute );
    _vEnemyRoutes.push_back ( ptrNew );
  }

void GameConfig::print()
  {
    string msg = "";
    unsigned int i;

    cout << "========================" << endl;
    cout << " * Game Configuration *" << endl;
    cout << "========================" << endl;
    cout << "Num Enemies      : " << _numEnemies << endl;
    cout << "Initial Pos Hero : " << _initialPos_Hero << endl;
    cout << "Enemy Routes     : " << _vEnemyRoutes.size() << endl;

    for ( i = 0; i < _vEnemyRoutes.size(); i++ )
      {
        _vEnemyRoutes[i]->print();
      }

    for ( i = 0; i < _vPositionHostages.size(); i++ )
      {
        msg = "Pos. Hostage" + Ogre::StringConverter::toString(i);
        cout << msg << "    : " << _vPositionHostages[i] << endl;
      }

    for ( i = 0; i < m_pieces.size(); i++ )
      {
        msg = "Piece" + Ogre::StringConverter::toString(i);
        cout << msg << "    : " << endl;
        if ( m_pieces[i] ) m_pieces[i]->print();
      }

    cout << "========================" << endl;
  }

const Ogre::Vector3& GameConfig::getPositionHostage ( unsigned int index ) const
  {
    if ( index < _vPositionHostages.size() )
      return _vPositionHostages[index];
    else
      throw GameConfigException ( "Error Position Hostage : Index Out of Bounds" );
  }

void GameConfig::addHostagePosition ( const Ogre::Vector3& p )
  {
    _vPositionHostages.push_back ( p );
  }

void GameConfig::addPiece ( const Piece& newPiece )
  {
    Piece* ptrNew = new Piece ( newPiece );
    m_pieces.push_back ( ptrNew );
  }
