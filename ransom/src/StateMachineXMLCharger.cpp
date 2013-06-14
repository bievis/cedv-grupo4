#include <StateMachineXMLCharger.h>

StateMachineXMLCharger* StateMachineXMLCharger::_instance = 0;

StateMachineXMLCharger* StateMachineXMLCharger::getSingletonPtr()
  {
    if ( 0 == _instance )
      {
        _instance = new StateMachineXMLCharger();
      }

    return _instance;
  }

StateMachineXMLCharger& StateMachineXMLCharger::getSingleton()
  {
    if ( 0 == _instance )
      {
        _instance = new StateMachineXMLCharger();
      }

    return *_instance;
  }

bool StateMachineXMLCharger::LoadFile ( string routeAbsoluteFile, StateMachine &sm )
  {
    ifstream is;

    try
      {
        is.open ( routeAbsoluteFile.c_str() );
      }
    catch ( std::ifstream::failure e )
      {
        std::cerr << "Exception opening/reading file '" << routeAbsoluteFile << "'" << endl;
        return false;
      }

    sm.clear();

    ptree pt;

    read_xml ( is, pt );

    string default_state = "";

    BOOST_FOREACH ( ptree::value_type const& v, pt.get_child("config") )
      {
        if ( v.first == "StateMachine" )
          {
            default_state = v.second.get<std::string>("<xmlattr>.default");
            sm.setCurrentState ( default_state );

            boost::property_tree::ptree states = (boost::property_tree::ptree) v.second;

            BOOST_FOREACH ( ptree::value_type const& v2, states.get_child("States") )
              {
                if ( v2.first == "State" )
                  {
                    State st;

                    string name = v2.second.get<std::string>("<xmlattr>.name");

                    st.setName ( name );

                    boost::property_tree::ptree actions = (boost::property_tree::ptree) v2.second;

                    BOOST_FOREACH ( ptree::value_type const& v3, actions.get_child("Actions") )
                      {
                        if ( v3.first == "Action" )
                          {
                            Action a;
                            string name_action = v3.second.get<std::string>("<xmlattr>.name");
                            a.setName ( name_action );
                            st.addAction ( a );
                          }
                      }

                    sm.addState ( st );
                  }
              }
          }
      }

    is.close();

    return true;
  }

StateMachineXMLCharger::StateMachineXMLCharger()
  {
  }

StateMachineXMLCharger::~StateMachineXMLCharger()
  {
  }
