#include "Action.h"

Action::Action()
{
    clear();
}

Action::Action ( const string &name )
{
    clear();

    _name = name;
}

void Action::clear()
{
    _name = "";
}

Action::Action ( const Action& other )
{
    Copy ( other );
}

void Action::Copy ( const Action& original )
{
    clear();

    _name = original.getName();
}

void Action::print_info()
{
	cout << "                @ action : name = '" << _name << "'" << endl;
}

Action &Action::operator= (const Action& original)
{
    Copy ( original );
    return *this;
}
