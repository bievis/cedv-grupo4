#include "StateMachineException.h"

StateMachineException::StateMachineException()
  {
    string msg = "unhandled error ocurred";
    _message = msg;
  }

StateMachineException::StateMachineException( const string &msg )
{
  _message = msg;
}

