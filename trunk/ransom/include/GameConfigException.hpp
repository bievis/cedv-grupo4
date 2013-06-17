#ifndef __GAME_CONFIG_EXCEPTION__
#define __GAME_CONFIG_EXCEPTION__

#include <string.h>
#include <stdio.h>
#include <exception>

using namespace std;

/// \brief Class with the Exception in the GameConfig class
class GameConfigException: public exception {

  private:
    /// \brief message to set
    char *_msg;

  public:
    /// \brief default constructor with the message
    /// \param msg message to show
    GameConfigException ( const char *msg );
    /// \brief default destructor
    virtual ~GameConfigException() throw ();
    /// Method to get the message configured
    /// \return message to show
    virtual const char* what() const throw();
};

#endif
