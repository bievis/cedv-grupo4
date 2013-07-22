#include <Level.hpp>

// ## Class Level ##

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
