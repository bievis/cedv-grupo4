#include "StateMachine.h"

//*****************************************************************************
// PRIVATE SECTION
//*****************************************************************************

bool StateMachine::find_state ( string nameState, map<string, State>::iterator &it_state )
  {
    bool res = true;

    it_state = _mapStates.find ( nameState );

    if ( it_state == _mapStates.end() )
      {
        res = false;
        cout << "The State <" << nameState << "> was not found." << endl;
      }

    return res;
  }

void StateMachine::initialize_machine ( const string &initState )
  {
    try
      {

        setCurrentState ( initState );
        _mapStates.clear();

      }
    catch ( StateMachineException &exc )
      {
        throw;
      }
    catch (...)
      {
        string msg = "Error Initializing state machine";

        throw StateMachineException ( msg );
      }
  }

//*****************************************************************************
// PUBLIC SECTION
//*****************************************************************************

StateMachine::StateMachine()
  {
    initialize_machine ( "" );
  }

StateMachine::StateMachine( const string &initState )
  {
    initialize_machine ( initState );
  }

StateMachine::~StateMachine()
  {
    try
      {

        clear();

      }
    catch (...)
      {
        cout << "Error destroying state machine" << endl;
        //throw StateMachineException ( msg );
      }
  }

StateMachine::StateMachine ( const StateMachine& other )
  {
    map<string, State>::const_iterator it_s;
    pair< map<string, State>::const_iterator, bool> ret_s;

    _currentState = other.getCurrentState();

    //Copy states
    for ( it_s = other.getStates()->begin(); it_s != other.getStates()->end(); advance ( it_s, 1 ) )
      {
        ret_s = _mapStates.insert ( pair<string, State>( it_s->first, it_s->second ) );

        if ( false == ret_s.second )
          {
            string msg = "the state '" + it_s->first + "' exists";

            cout << msg << endl;

            throw StateMachineException ( msg );
          }
      }
  }

void StateMachine::setCurrentState( const string &newCurrentState )
  {
    string msgCurrent = newCurrentState;
    //Convertimos a mayusculas
    std::transform ( msgCurrent.begin(), msgCurrent.end(), msgCurrent.begin(), ::toupper );

    _currentState = newCurrentState;

    cout << "The current state is '" << msgCurrent << "'" << endl;
  }

const State& StateMachine::getCurrentStateObject() const
  {
    bool res = false;
    map<string, State>::iterator it_state;

    res = find_state ( _currentState, it_state );

    if ( !res )
      {
        string msg = "The state <" + _currentState + "> was not found in the states set";
        throw StateMachineException ( msg );
      }

    return it_state->second;
  }


bool StateMachine::addState ( const State &new_state )
  {
    pair< map<string, State>::const_iterator, bool> ret;
    bool res = true;
    State s_new ( new_state );

    ret = _mapStates.insert ( pair<string, State>( s_new.getName(), s_new ) );

    if ( false == ret.second )
      {
        res = false;
        string msg;

        msg = "The state '" + s_new.getName() + "' exists";

        cout << msg << endl;

        throw StateMachineException( msg );
      }

    return res;
  }

void StateMachine::print_info()
  {
    char cad[250];
    string msg = "";

    sprintf ( cad, "states = %d", _mapStates.size() );

    msg = cad;

    cout << msg << endl;

    for ( std::map<string, State>::iterator it = _mapStates.begin(); it != _mapStates.end(); advance ( it, 1 ) )
      it->second.print_info();
  }

bool StateMachine::validateXML()
  {
    bool res = true;
    std::map<string, State>::iterator it_state;
    string msg = "";

    // At least, there is one state in the map
    if ( _mapStates.empty() )
      {
        res = false;

		    msg = "there aren't states";

        cout << msg << endl;
      }

    if ( res )
      {
        for ( std::map<string, State>::iterator it_s = _mapStates.begin(); it_s != _mapStates.end(); advance ( it_s, 1 ) )
          {
            //Validate number of actions was correct
            if ( it_s->second.getActions()->size() == 0 )
              {
                res = false;

                msg = "There aren't actions in the state '" + it_s->first + "'";

                cout << msg << endl;
              }
          }
      }

    return res;
  }

void StateMachine::clear()
  {
    _mapStates.clear();
  }

bool StateMachine::load_from_file ( const string& route_absolute )
  {
    return StateMachineXMLCharger::getSingleton().LoadFile( route_absolute, *this );
  }
