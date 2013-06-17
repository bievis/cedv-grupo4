#ifndef __STATE_MACHINE_XML_CHARGER__
#define __STATE_MACHINE_XML_CHARGER__

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <StateMachine.h>
#include <State.h>
#include <Action.h>

class StateMachine;

using boost::property_tree::ptree;

/// \brief This class manage the XML charger for the state machine
/// this class follow the singleton pattern
class StateMachineXMLCharger {

  private:
    /// \brief default constructor
    StateMachineXMLCharger();
    /// \brief default destructor
    ~StateMachineXMLCharger();
    /// Reference to object this of our class
    static StateMachineXMLCharger *_instance;

public:
    /// \brief Method to get the object of the class (singleton pattern)
    /// \return reference to the class object
    static StateMachineXMLCharger& getSingleton();
    /// \brief Method to get the pointer reference of the class (singleton pattern)
    /// \return pointer reference to the class object
    static StateMachineXMLCharger* getSingletonPtr();
    /// \brief method to load the xml config from disk
    /// \param routeAbsoluteFile absolute route to xml file with the configuration
    /// \param sm object where we charge the xml configuration loaded
    bool LoadFile ( string routeAbsoluteFile, StateMachine &sm );

};

#endif
