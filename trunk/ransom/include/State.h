#ifndef STATE_H
#define STATE_H

#include <string>
#include <map>
#include <iostream>
#include "StateMachineException.h"
#include "Action.h"

using namespace std;

/// \brief Define a state of the state machine
///
/// This class define the states availables in our state machine. Also, the state include posible modes and posible next states for the actual state
class State
{
    public:
        /// \brief default constructor
        ///
        /// this constructor make a state with empty name and without modes
        State();
        /// \brief constructor with a name will be created
        ///
        /// this constructor make a state with a name (parameter) and without modes and without next states
        /// \param name state name
        State( const string &name );
        /// \brief copy constructor
        ///
        /// this constructor give us the posibility to copy other state in the actual state
        /// \param other the state to copy
        /// \throw StateMachineException
        State( const State &other );
        /// \brief default destructor
        ///
        /// the destructor will clean the actions map and next states set
        virtual ~State();
        /// \brief get the name of the state
        ///
        /// this method returned the name given to the state
        /// \return the name of the state
        inline string getName() const { return _name; };
        /// \brief set the name of the state
        ///
        /// this method set the name to the state
        /// \param newName the name of the state
        inline void setName( const string &newName ) { _name = newName; };
        /// \brief get the actions for this state
        ///
        /// this method get a reference to the map with the actions in this state
        /// \return reference to the map of actions
        inline std::map<string, Action> *getActions() { return &_mapActions; };
        /// \brief add a action in the state
        ///
        /// this method add a action for this state, concretly, insert the action new_action in the map mapModes with the key name of the action
        /// \param new_action action will be inserted in the map
        /// \throw StateMachineException
        bool addAction ( const Action &new_action );
        /// \brief print the state in console
        ///
        /// this method print the state name addition to its actions
        void print_info();
        /// \brief clean maps and sets inside state
        ///
        /// this method clean the actions map
        /// \throw StateMachineException
        void clear();
        /// \brief find an action inside the state
        /// \param name the name of the action to find in mode currentMode
        /// \return true/false if it's found or not
        bool find_action ( const string &name );

    private:
        /// \brief name of the state
        string _name;
        /// \brief map with modes of this state
        std::map<string, Action> _mapActions;

};

#endif // STATE_H
