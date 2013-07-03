#ifndef __XML_CHARGER__
#define __XML_CHARGER__

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <GameConfig.h>

class StateMachine;

using boost::property_tree::ptree;

/// \brief This class manage the XML charger for the game configuration
/// this class follow the singleton pattern
class XMLCharger {

  private:
    /// \brief default constructor
    XMLCharger();
    /// \brief default destructor
    ~XMLCharger();
    /// Reference to object this of our class
    static XMLCharger *_instance;

public:
    /// \brief Method to get the object of the class (singleton pattern)
    /// \return reference to the class object
    static XMLCharger& getSingleton();
    /// \brief Method to get the pointer reference of the class (singleton pattern)
    /// \return pointer reference to the class object
    static XMLCharger* getSingletonPtr();
    /// \brief method to load the xml config from disk
    /// \param routeAbsoluteFile absolute route to xml file with the configuration
    /// \param sm object where we charge the xml configuration loaded
    void LoadGameConfig ( const string& routeAbsoluteFile, GameConfig &gc );
    /// \brief method to load the xml config from disk
    /// \param routeAbsoluteMap absolute route to xml file with the map
    /// \param sm object where we charge the xml configuration loaded
//    void LoadMap ( const string& routeAbsoluteMap, GameConfig &gc );
};

#endif
