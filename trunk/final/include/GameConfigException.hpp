#ifndef __GAME_CONFIG_EXCEPTION__
#define __GAME_CONFIG_EXCEPTION__

#include <string.h>
#include <stdio.h>
#include <exception>

using namespace std;

class GameConfigException: public exception {

 private:
  char *_msg;

public:
  GameConfigException ( const char *msg );

  virtual ~GameConfigException() throw ();
  virtual const char* what() const throw();  
};

#endif
