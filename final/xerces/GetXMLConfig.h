/* #ifndef __XML_CHARGER__ */
/* #define __XML_CHARGER__ */

/* /\* #include <boost/property_tree/xml_parser.hpp> *\/ */
/* /\* #include <boost/property_tree/ptree.hpp> *\/ */
/* /\* #include <boost/foreach.hpp> *\/ */
/* #include <GameConfig.h> */

/* /\* using boost::property_tree::ptree; *\/ */

/* class XMLCharger { */

/* private: */

/*   XMLCharger() {}; */
/*   virtual ~XMLCharger() {}; */

/*   static XMLCharger *_instance; */

/* public: */

/*   static XMLCharger& getSingleton(); */
/*   static XMLCharger* getSingletonPtr(); */

/*   void LoadFile ( string routeAbsoluteFile, GameConfig &gc ); */

/* }; */

/* #endif */

#ifndef __GET_XML_CONFIG_HPP__
#define __GET_XML_CONFIG_HPP__

/**
 *  @file
 *  Class "GetConfig" provides the functions to read the XML data.
 *  @version 1.0
 */

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <string>
#include <stdexcept>

// Error codes

enum {
   ERROR_ARGS = 1,
   ERROR_XERCES_INIT,
   ERROR_PARSE,
   ERROR_EMPTY_DOCUMENT
};

class GetXMLConfig
  {
public:
    static GetXMLConfig* getSingletonPtr();
    static GetXMLConfig& getSingleton();

    void readConfigFile ( std::string& ) throw ( std::runtime_error );

    inline char *getOptionA() const { return _instance?m_OptionA:NULL; };
    inline char *getOptionB() const { return _instance?m_OptionB:NULL; };

private:
    static GetXMLConfig *_instance;

    xercesc::XercesDOMParser *m_ConfigFileParser;

    char* m_OptionA;
    char* m_OptionB;

    // Internal class use only. Hold Xerces data in UTF-16 SMLCh type.

    XMLCh* TAG_root;
    XMLCh* TAG_ApplicationSettings;
    XMLCh* ATTR_OptionA;
    XMLCh* ATTR_OptionB;

    GetXMLConfig();
    ~GetXMLConfig();
};
#endif
