#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <iostream>
#include <algorithm>
#include <map>

#include "State.h"
#include "StateMachineException.h"
#include "StateMachineXMLCharger.h"

using namespace std;

/// \brief class to manage the state machine
///
/// this class manage all around the state machine, ie, contains the states and actions
class StateMachine
{
    public:
        /// \brief default constructor
        ///
        /// this constructor make a empty state machine without states
        StateMachine();
        /// \brief constructor with a initial state
        ///
        /// this constructor make a empty state machine without states but with a initial state
        /// \param initState initial state of the state machine
        StateMachine( const string &initState );
        /// \brief copy constructor
        ///
        /// this constructor give us the posibility to copy other state machine in the actual state machine
        /// \param other the state machine to copy
        StateMachine( const StateMachine &other );
        /// \brief default destructor
        ///
        /// the destructor will clean the state's map and the initial state
        virtual ~StateMachine();
        /// \brief get the states for this state machine
        ///
        /// this method get a reference to the map with the states availables in this state machine
        /// \return reference to the map of states
        inline std::map<string, State> *getStates() { return &_mapStates; };
        /// \brief get the current state
        ///
        /// this method returned the current state of the state machine
        /// \return the name of the current state
        inline const string& getCurrentState() const { return _currentState; };
        /// \brief get the object state for the current state
        /// \return state object
        const State& getCurrentStateObject();
        /// \brief set the current state of the state machine
        ///
        /// this method set the current state of the state machine. In case of we like stablish a determinate state.
        /// \param newCurrentState the name of the new current state
        void setCurrentState( const string &newCurrentState );
        /// \brief add a state in the state machine
        ///
        /// this method add a state in the state machine, concretly, insert the state new_state in the map mapStates with the key name of the state
        /// \param new_state state will to be inserted in the map
        bool addState ( const State &new_state );
        /// \brief validate if the action "name" is posible in the current state
        ///
        /// this method ask to the state machine if it's posible in the current state to do the action "name"
        /// \param name the name of the action than we try to execute
        /// \return true/false if it's posible perform the action "name" inside the actions in the current state
        bool validateAction ( const string &name );
        /// \brief validate the correct configuration of the XML file
        ///
        /// this method validate :
        ///     - exist at least one state inside the state machine
        ///     - exist at least one action inside each state
        ///
        /// \return true/false if it's correct the configuration of the XML file
        bool validateXML();
        /// \brief print the info about state machine in console, also print current info
        ///
        /// this method print info about the states and actions of the state machine. Also print current info
        void print_info();
        /// \brief clean map inside state machine
        ///
        /// this method clean the states map
        void clear();
        /// \brief find the state inside the states
        ///
        /// this method find the state inside the states of the state machine and returned true/false if it's found. The state will be returned in the parameter "it_state" how an iterator
        /// \param nameState name of the state to find
        /// \param it_state the iterator with the state to find
        /// \return true/false if it's found the state in the state machine
        bool find_state ( string nameState, std::map<string, State>::iterator &it_state );
        /// \brief method to load xml config file for the state machine
        /// \param route_absolute route to the xml file
        /// \param true/false if load or not the file
        bool load_from_file ( const string& route_absolute );

    private:
        /// \brief map with states of this state machine
        std::map<string, State> _mapStates;
        /// \brief current state of the state machine
        string _currentState;

        /// \brief find the action inside the current state
        ///
        /// this method find the action inside the current state and returned true/false if it's found. The action will be returned in the parameter "ac"
        /// \param name name of the action to find
        /// \param ac the action returned
        /// \return true/false if it's found the action in the current state
        bool find_action ( const string &name, Action &ac );
        /// \brief initialize the state machine with a set of values
        ///
        /// this method is used by all constructors to set the state machine (only for unified code purposes)
        /// \param initState initial state
        void initialize_machine ( const string &initState );

};

#endif // STATEMACHINE_H
