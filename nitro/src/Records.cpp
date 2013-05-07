#include "Records.h"

Records* Records::msSingleton = NULL;

const char *FILENAME = "highscores.dat";

Records::Records()
  {
    _records.clear();
  }

Records::~Records()
  {
    _records.clear();
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

void Records::add ( int seconds )
  {
    cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":in" << endl;
    cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << " seconds " << seconds << endl;

    char cad[100];
    std::list<string>::iterator it;
    string fecha;
    string hora;

    memset ( cad, 0, 10 );

    getFechaHora ( fecha, hora );

    sprintf ( cad, "%d|%s|%s", seconds, fecha.c_str(), hora.c_str() );

    if ( _records.size() == 0 )
      {
	it = _records.begin();
      }
    else
      {
	for ( it = _records.begin(); it != _records.end(); ++it )
	  {
	    if ( compare ( seconds, *it ) > 0 ) //newValue > *it
	      {
		break;
	      }
	  }
      }

    _records.insert ( it, string(cad) );

    // _records.push_back ( newValue );
    // sort ( _records.begin(), _records.end() );
    // reverse ( _records.begin(), _records.end() );

    cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":out" << endl;
  }

int Records::compare ( int seconds, string value2 )
  {
    cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":in" << endl;
    cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ": seconds " << seconds << " - value2 " << value2 << endl;

    int level2;
    int seconds2;
    int ret = 0;
    char resto[100];

    sscanf ( value2.c_str(), "%d|%s", &seconds2, resto );

    if ( seconds < seconds2 )
      {
	ret = 1;
      }

    cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ":out" << endl;

    return ret;
  }

string Records::getValue ( unsigned int index )
  {
    string value = "";
    std::list<string>::iterator it;

    if ( index >= 0 && index < _records.size() )
      {
	it = _records.begin();
	advance ( it, index );
	if ( it != _records.end() )
	  {
	    value = *it;
	  }
      }

    return value;
  }

void Records::Copy ( Records &source )
  {
    _records.clear();

    for ( unsigned int i = 0; i < source.getSize(); i++ )
      _records.push_back ( source.getValue ( i ) );
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
        std::list<string>::iterator it;

        for ( it = _records.begin(); it != _records.end(); ++it )
	  {
            os << *it << endl;
	  }

        os.close();
    }
  }

 void Records::read()
   {
     ifstream is ( FILENAME );
     string value = "";
     string final = "";

     _records.clear();

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
	     if (!is.eof())
	       {
		 cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << ": value = " << value << endl;
		 _records.push_back ( value );
	       }
	   }

        is.close();
    }

   }

void Records::compacta ( unsigned int max_size )
  {
    while ( _records.size() > max_size )
      {
	_records.pop_back();
      }
  }

void Records::print()
  {
    std::list<string>::iterator it;
    unsigned int i = 0;

    for ( it = _records.begin(); it != _records.end(); ++it, ++i )
      {
	cout << " elem " << i+1 << " : " << *it << endl;
      }

  }

void Records::getFechaHora ( string &fecha, string &hora )
{
  time_t timer;
  struct tm *t;
  char cad[100];

  timer = time(NULL);
  t = localtime(&timer);

  sprintf ( cad, "%02d-%02d-%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

  fecha = cad;

  sprintf ( cad, "%02d:%02d", t->tm_hour, t->tm_min );

  hora = cad;
}
