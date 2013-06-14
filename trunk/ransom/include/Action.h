#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

/// \brief Define an action in the platform
///
/// This class define an action can be performed in the platform
class Action
{
    public:
        /// \brief default constructor
        ///
        /// this constructor make a action with empty name and their attributes empty too
        Action();
        /// \brief constructor with an action name will be created
        ///
        /// this constructor make an action with a name (parameter) and their attributes (nextStateOK, nextModeOK, ...) are empty
        /// \param name action name
        Action( const string &name );
        /// \brief copy constructor
        ///
        /// this constructor give us the posibility to copy another action in the action actual
        /// \param other the action to copy
        /// \throw StateMachineException
        Action( const Action &other );
        /// \brief get the name of the action
        ///
        /// this method returned the name given to the action
        /// \return the name of the action
        inline const string& getName() const { return _name; };
        /// \brief set the name of the action
        ///
        /// this method set the name to the action
        /// \param newName the name of the action
        inline void setName( const string &newName ) { _name = newName; };
        /// \brief print the action in console
        ///
        /// this method print the action name addition to its other attributes
        void print_info();
        /// \brief clean action
        ///
        /// this method clean the values of its name and attributes
        /// \throw StateMachineException
        void clear();
        /// \brief override the operator = to copy an action in other
        ///
        /// this method override to copy an action (original) in our action
        /// \param original the original action to copy
        /// \return copy of the original action
        Action &operator= ( const Action &original );

    private:
        /// \brief name of the action
        string _name;

        /// \brief this method copy the action passed how parameter in our object action
        void Copy ( const Action& other );

};

#endif // ACTION_H
