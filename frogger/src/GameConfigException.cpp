#include <GameConfigException.hpp>

// ## Exception Class Game Config ##

GameConfigException::GameConfigException ( const char *msg ) : exception() 
  {
    _msg = new char [ strlen ( msg ) ];
    strcpy ( _msg, msg );
  }

const char* GameConfigException::what() const throw()
  {
    return _msg;
  }

GameConfigException::~GameConfigException() throw ()
  {
    if ( _msg )
      delete _msg;
  }
