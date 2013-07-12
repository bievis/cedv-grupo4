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

    for ( unsigned int i = 0; i < _pieces.size(); i++ )
      {
        if ( _pieces[i] )
          {
            delete _pieces[i];
          }
      }

    _pieces.clear();

	_piecesMap.clear();
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
		  _vEnemyRoutes.push_back(source._vEnemyRoutes[i]);
      }

    for ( i = 0; i < source.getNumHostages(); i++ )
      {
		  _vPositionHostages.push_back(source._vPositionHostages[i]);
      }

	_rowsMap = source._rowsMap;
    _colsMap = source._colsMap;

    Piece *ptr = NULL;
    for ( i = 0; i < source.getNumPieces(); i++ )
      {
        if ( ((GameConfig&)source).getPiece(i) )
          {
            ptr = new Piece ( *(((GameConfig&)source).getPiece(i)) );
            _pieces.push_back ( ptr );
          }
      }

	for ( i = 0; i < source._piecesMap.size(); i++ )
      {
		  T_PART_MAP part;
		  part.pos = source._piecesMap[i].pos;
		  part.pPiece = source._piecesMap[i].pPiece;
		  _piecesMap.push_back(part);
      }

  }

const EnemyRoute& GameConfig::getEnemyRoute ( unsigned int index ) const
  {

	if ( _vEnemyRoutes.size() > 0 )
	{
		return *(_vEnemyRoutes[index]);
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

    for ( i = 0; i < _pieces.size(); i++ )
      {
        msg = "Piece" + Ogre::StringConverter::toString(i);
        cout << msg << "    : " << endl;
        if ( _pieces[i] ) _pieces[i]->print();
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
    _pieces.push_back ( ptrNew );
  }

Piece* GameConfig::getPiece ( unsigned int index )
  {
    Piece* ptr = NULL;

    if ( index < _pieces.size() )
      ptr = _pieces[index];

    return ptr;
  }

T_PART_MAP GameConfig::getPieceMap ( unsigned int index )
  {
	  return _piecesMap[index];
  }

void GameConfig::createMapRandom () {
	srand(time(NULL));

	/*
	* Calculamos las posiciones donde podemos poner piezas en el mapa
	*/

	float sizeX = _colsMap;
	float sizeZ = _rowsMap;
	// numero de piezas por cuadrante a lo anchho y a lo largo
	float nPartsCX = sizeX / 2.0f; // Dividir en 2 cuadrantes
	float nPartsCZ = sizeZ / 2.0f; // Dividir en 2 cuadrantes

	// Coordenada de referencia de cada cuadrante
	Ogre::Vector3 C1(-1,1,-1);
	Ogre::Vector3 C2(1,1,-1);
	Ogre::Vector3 C3(-1,1,1);
	Ogre::Vector3 C4(1,1,1);
	std::vector<Ogre::Vector3> pointsParts;

	// Posicion de la primera pieza en cada cuadrante
	float posX = SIZE_PART / 2.0;
	float posZ = SIZE_PART / 2.0;
	Ogre::Vector3 p(posX,0,posZ);

	for (unsigned int iz = 0; iz < nPartsCZ; iz++ ) {
		p.x = SIZE_PART / 2.0;
		for (unsigned int ix = 0; ix < nPartsCX; ix++ ) {
			pointsParts.push_back(p * C1);
			pointsParts.push_back(p * C2);
			pointsParts.push_back(p * C3);
			pointsParts.push_back(p * C4);

			p.x = p.x + SIZE_PART;
		}
		p.z = p.z + SIZE_PART;
	}

	/*
	* Por cada posicion añadimos una pieza aleatoriamente
	*/
	Piece* piece;
	EnemyRoute *enemyRoute;
	EnemyRoute *newEnemyRoute;
	unsigned int aleatorio;
	for ( unsigned int i = 0; i < pointsParts.size(); i++ )
	{
		// Desde 0 hasta n-1 piezas, porque es un array
		aleatorio = rand() % getNumPieces();

		piece = getPiece(aleatorio);

		// Posicion de la pieza en el mapa
		T_PART_MAP partMap;
		partMap.pos = pointsParts[i];
		partMap.pPiece = piece;
		_piecesMap.push_back(partMap);
		Ogre::Vector3 p = Ogre::Vector3::ZERO;

		// Recalculamos las rutas de los enemigos
		for ( unsigned int j = 0; j < piece->getNumEnemyRoutes(); j++ ) {
			enemyRoute = piece->getEnemyRoute(j);
			newEnemyRoute = new EnemyRoute(*enemyRoute);
			for ( int k = 0; k < newEnemyRoute->getNumPoints(); k++ ) {
				p = newEnemyRoute->getPoint(k) + partMap.pos;
				newEnemyRoute->setPoint(p, k);
			}
			_vEnemyRoutes.push_back(newEnemyRoute);
		}

		// Recalculamos las rutas de los rehenes
		for ( unsigned int j = 0; j < piece->getNumPosHostages(); j++ ) {
			_vPositionHostages.push_back(piece->getPosHostage(j) + partMap.pos);
		}
	}

	_numEnemies = _vEnemyRoutes.size();
}
