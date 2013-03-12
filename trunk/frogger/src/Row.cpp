#include <Row.hpp>

// ## Class Rows ##

Row::~Row()
  {
  }

void Row::copy ( const Row& source )
  {
    _name = source.get_name();
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
    cout << "     - Speed = " << _speed << endl;
    cout << "     - Way = " << ((_way == RIGHT)?"RIGHT":"LEFT") << endl;
    cout << "     - Distance = " << _speed << endl;
  }

void Row::clear()
  {
    _name = "";
    _speed = 0;
    _way = RIGHT;
    _distance = 0;
  }
