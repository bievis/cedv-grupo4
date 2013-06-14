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

class StateMachineXMLCharger {

private:

    StateMachineXMLCharger();
    ~StateMachineXMLCharger();

    static StateMachineXMLCharger *_instance;

public:

    static StateMachineXMLCharger& getSingleton();
    static StateMachineXMLCharger* getSingletonPtr();

    bool LoadFile ( string routeAbsoluteFile, StateMachine &sm );

};

#endif
