#ifndef STATEMACHINEEXCEPTION_H
#define STATEMACHINEEXCEPTION_H

#include <string>
#include <iostream>

using namespace std;

/// \brief Define an exception in our state machine
///
/// \brief This class define the standard exception in our state machine
class StateMachineException
{
    public:
        /// \brief default constructor
        ///
        /// this constructor make a exception with standard message of unhandled exception
        StateMachineException();
        /// \brief exception constructor with a message
        ///
        /// this constructor make a exception with a message give us how parameter
        StateMachineException( const string &msg );
        /// \brief default destructor
        ///
        /// the destructor clean the message inside exception
        ~StateMachineException() { _message = ""; };
        /// \brief get the message of the exception
        ///
        /// this method returned the message explaining the exception
        /// \return the message explaining the exception
        inline string what() const { return _message; };

    private:
        /// \brief the message explaining the exception
        string _message;
};


#endif // STATEMACHINEEXCEPTION_H
