#include <GameConfig.h>

// ## Class Rows ##

Row::~Row()
  {
  }

void Row::copy ( const Row& source )
  {
    _name = source.get_name();
    _elements = source.get_num_elements();
    _speed = source.get_speed();
    _way = source.get_way();
    _distance = source.get_distance();
  }

Row::Row ( const Row& source )
  {
    copy ( source );
  }

Row Row::operator= ( const Row& source )
  {
    copy ( source );
    return *this;
  }

void Row::print() const
  {
    cout << "     - Name = " << _name << endl;
    cout << "     - Elements = " << _elements << endl;
    cout << "     - Speed = " << _speed << endl;
    cout << "     - Way = " << ((_way == RIGHT)?"RIGHT":"LEFT") << endl;
    cout << "     - Distance = " << _speed << endl;
  }

void Row::clear()
  {
    _name = "";
    _elements = 0;
    _speed = 0;
    _way = RIGHT;
    _distance = 0;
  }

// ## Class Elemento ##

Level::Level()
  {
  }

Level::~Level()
  {
  }

void Level::copy ( Level& source )
  {
    _crater = source.getCrater();
    _road = source.getRoad();
  }

Level::Level ( Level& source )
  {
    copy ( source );
  }

Level& Level::operator= ( Level& source )
  {
    copy ( source );
    return *this;
  }

void Level::clear()
  {
    _crater.clear();
    _road.clear();
  }

void Level::print()
  {
    cout << "   *** Crater *** " << endl;
    _crater.print();
    cout << "   ***  Road  *** " << endl;
    _road.print();
  }

// ## Class Elemento ##

void Elemento::clear()
  {
    for ( unsigned int i = 0; i < _rows.size(); i++ )
      {
	if ( _rows[i] ) 
	  delete _rows[i];
      }
    _rows.clear();
  }

Elemento::Elemento()
  {
    clear();
  }

Elemento::~Elemento()
  {
    clear();
  }

void Elemento::copy ( const Elemento& source )
  {
    clear();  

    for ( unsigned int i = 0; i < source._rows.size(); i++ )
      {
        addRow ( *(source._rows[i]) );
      }
  }

Elemento::Elemento ( const Elemento& source )
  {
    copy ( source );
  }

Elemento Elemento::operator= ( const Elemento& source )
  {
    copy ( source );
    return *this;
  }

bool Elemento::addRow ( const Row& newValue )
  {
    Row *newRow = new Row ( newValue );
    _rows.push_back ( newRow );

    return true;
  }

bool Elemento::getRow ( unsigned int elem, Row& value )
  {
    if ( _rows.size() < elem && _rows[elem] )
      value = *(_rows[elem]);

    return true;
  }

void Elemento::print()
  {
    for ( unsigned int i = 0; i < _rows.size(); i++ )
      {
	cout << "   # Row " << i+1 << endl;
	_rows[i]->print();
      }
  }

// ## Class Game Config ##

GameConfig::GameConfig()
  {
  }

void GameConfig::clear()
  {
    for ( unsigned int i = 0; i < _vLevels.size(); i++ )
      delete _vLevels[i];

    _vLevels.clear();
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
    Level lvl;

    clear();

    for ( unsigned int i = 0; i < source.getNumLevels(); i++ )
      {
        _vLevels[i] = source._vLevels[i];
      }
  }

bool GameConfig::getLevel ( unsigned int index, Level& level )
  {
    if ( index < _vLevels.size() )
      {
	if ( _vLevels[index] )
	  {
            level = *(_vLevels[index]);
	  }
      }

    return true;
  } 

bool GameConfig::addLevel ( Level& newLevel )
  {
    Level *newLv = new Level ( newLevel );
    _vLevels.push_back ( newLv );

    return true;
  }

void GameConfig::print()
  {
    cout << "==================" << endl;
    cout << "Game Configuration" << endl;


    for ( unsigned int i = 0; i < _vLevels.size(); i++ )
      {
        cout << "==================" << endl;
	cout << " - Level " << i+1 << endl;
        cout << "==================" << endl;
	_vLevels[i]->print();
      } 

    cout << "==================" << endl;
  }
