#include "Records.h"

Records::Records()
  {
    _vRecords.clear();
  }

Records::~Records()
  {
    _vRecords.clear();
  }

void Records::add ( unsigned int value )
  {
    _vRecords.push_back ( value );
    sort ( _vRecords.begin(), _vRecords.end() );
    reverse ( _vRecords.begin(), _vRecords.end() );
  }

unsigned int Records::getValue ( unsigned int index )
  {
    int value = 0;

    if ( index >= 0 && index < _vRecords.size() )
      value = _vRecords[index];

    return value;
  }

void Records::Copy ( Records &source )
  {
    _vRecords.clear();

    for ( unsigned int i = 0; i < source.getSize(); i++ )
      _vRecords.push_back ( source.getValue ( i ) );
  }  

Records::Records ( Records& source )
  {
    Copy ( source );
  }

Records Records::operator= ( Records& source )
  {
    Copy ( source );
    return *this;
  }

void Records::write()
  {
    ofstream os( "records.dat" );

    if ( !os.is_open() ) 
      {
        cout << "Error opening records.dat for write" << endl;
        return;
      }
    else 
      {
        cout << "Opened points.dat for write" << endl;
        /// Write Data
        for ( unsigned int i = 0; i < _vRecords.size(); i++ ) {
            os << " " << _vRecords[i];
        }
        os.close();
    }
  }

 void Records::read()
   { 
     ifstream is("records.dat");
     unsigned int value = 0;

     if ( !is.is_open() ) 
       {
	 cout << "Error opening points.dat for read" << endl;
	 return;
       }
     else 
       {
	 cout << "Opened points.dat for read" << endl;
	 
	 while ( !is.eof() ) 
	   {
	     is >> value;
	     _vRecords.push_back ( value );
	   }

        is.close();
    }

   }
