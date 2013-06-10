#ifndef __XML_CHARGER__
#define __XML_CHARGER__

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <GameConfig.h>

using boost::property_tree::ptree;

class XMLCharger {

private:

    XMLCharger();
    ~XMLCharger();
  
    static XMLCharger *_instance;

public:

    static XMLCharger& getSingleton();
    static XMLCharger* getSingletonPtr();

    void LoadFile ( string routeAbsoluteFile, GameConfig &gc );

};

#endif
