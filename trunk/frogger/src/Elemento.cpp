#include <Elemento.hpp>

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
    if ( ( elem < _rows.size() ) && _rows[elem] )
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
