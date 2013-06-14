#include "State.h"

//*****************************************************************************
// PUBLIC SECTION
//*****************************************************************************

State::State()
{
    clear();
}

State::State ( const string &name )
{
    clear();
    _name = name;
}

State::~State()
{
    clear();
}

State::State( const State& other )
{
    map<string, Action>::const_iterator it_a;
    pair< map<string, Action>::iterator, bool > ret_a;
    string msg = "";

    clear();

    //Copy name
    _name = other.getName();

    //Copy actions
    for ( it_a = other.getActions()->begin(); it_a != other.getActions()->end(); advance ( it_a, 1 ) )
    {
        ret_a = _mapActions.insert ( pair<string, Action>( it_a->first, it_a->second ) );

        if ( false == ret_a.second )
        {
          string msg = "The action '" + it_a->first + "' exists in the state '" + _name + "'";
          throw StateMachineException( msg );
        }
    }
}

bool State::addAction ( const Action &new_action )
{
    pair< map<string, Action>::const_iterator, bool > ret;
    bool res = true;
//    Action a_new ( new_action );

    ret = _mapActions.insert ( pair<string, Action>( new_action.getName(), new_action ) );

    if ( false == ret.second )
      {
        string msg = "The action '" + new_action.getName() + "' exists in the state '" + _name + "'";
        throw StateMachineException( msg );
      }

    return res;
}

void State::print_info()
  {
    char cad[100];
    string msg;

    cout << "+ state # name = '" << _name << "'" << endl;

    sprintf ( cad, "%d", _mapActions.size() );
    cout << "        # actions = " << cad << endl;

    for ( map<string, Action>::iterator it_a = _mapActions.begin(); it_a != _mapActions.end(); advance ( it_a, 1 ) )
      it_a->second.print_info();
  }

void State::clear()
{
    try
      {
        _mapActions.clear();
        _name = "";
      }
    catch (...)
      {
    	  string msg = "Error clearing the state '" + _name + "'";
        throw StateMachineException ( msg );
      }
}

bool State::find_action ( const string &name )
{
    bool res = false;
    map<string, Action>::iterator it_a;

    it_a = _mapActions.find ( name );

    if ( it_a != _mapActions.end() )
      res = true;

    return res;
}
