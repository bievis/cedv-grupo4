#include "Records.h"

Records* Records::msSingleton = NULL;

const char *FILENAME = "highscores.dat";

Records::Records()
  {
    _vRecords.clear();
  }

Records::~Records()
  {
    _vRecords.clear();
  }

Records& Records::getSingleton()
  {
    if ( NULL == msSingleton )
      msSingleton = new Records;

    return (*msSingleton);
  }

Records* Records::getSingletonPtr()
  {
    if ( NULL == msSingleton )
      msSingleton = new Records();

    return (msSingleton);
  }

void Records::add ( string newValue )
  {
    _vRecords.push_back ( newValue );
    sort ( _vRecords.begin(), _vRecords.end() );
    reverse ( _vRecords.begin(), _vRecords.end() );
  }

string Records::getValue ( unsigned int index )
  {
    string value = "";

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
    ofstream os ( FILENAME );

    if ( !os.is_open() ) 
      {
        cout << "Error opening " << FILENAME << " for write" << endl;
        return;
      }
    else 
      {
        cout << "Opened " << FILENAME << " for write" << endl;
        /// Write Data
        for ( unsigned int i = 0; i < _vRecords.size(); i++ ) {
            os << " " << _vRecords[i];
        }
        os.close();
    }
  }

 void Records::read()
   { 
     ifstream is ( FILENAME );
     string value = "";
     string final = "";
     unsigned int i = 1;

     if ( !is.is_open() ) 
       {
	 cout << "Error opening " << FILENAME << " for read" << endl;
	 return;
       }
     else 
       {
	 cout << "Opened " << FILENAME << " for read" << endl;
	 
	 while ( !is.eof() ) 
	   {
	     is >> value;
             final += value + string(" ");

	     if ( ( i % 5 ) == 0 )
	       {
	         _vRecords.push_back ( final );
		 final = "";
	       }

	     i++;
	   }

        is.close();
    }

   }

void Records::compacta ( unsigned int max_size )
  {
    while ( _vRecords.size() > max_size )
      {
	_vRecords.pop_back();
      }
  }
