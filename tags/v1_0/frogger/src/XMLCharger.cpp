#include <XMLCharger.h>

XMLCharger* XMLCharger::_instance = 0;

XMLCharger* XMLCharger::getSingletonPtr()
  {
    if ( 0 == _instance )
      {
	_instance = new XMLCharger();
      }

    return _instance;
  }

XMLCharger& XMLCharger::getSingleton()
  {
    if ( 0 == _instance )
      {
	_instance = new XMLCharger();
      }

    return *_instance;
  }

void XMLCharger::LoadFile( string routeAbsoluteFile, GameConfig &gc )
  {
    ifstream is;
    try {
      is.open ( routeAbsoluteFile.c_str() );
    }
    catch ( std::ifstream::failure e ) {
      std::cerr << "Exception opening/reading file '" << routeAbsoluteFile << "'" << endl;
      return;
    }

    gc.clear();

    ptree pt;
    Level lvl;
    Row row;
    string way = "";

    read_xml ( is, pt );

    BOOST_FOREACH ( ptree::value_type const& v, pt.get_child("config") )
      {
        if ( v.first == "level" )
          {
	    lvl.clear();

            // // f.carrier = v.second.get<std::string>("carrier");
            // // f.number = v.second.get<unsigned>("number");
            // cout << "Level " << v.second.get<unsigned>("<xmlattr>.id") << endl;

            boost::property_tree::ptree levels = (boost::property_tree::ptree) v.second;

            BOOST_FOREACH ( ptree::value_type const& v, levels.get_child("crater") )
              {
                if ( v.first == "row" )
                  {
		    row.clear();
		    way = "";

    		    row.set_name ( v.second.get<string>("name") );
		    // row.set_num_elements ( v.second.get<unsigned>("elements") );
		    row.set_speed ( v.second.get<double>("speed") );
		    way = v.second.get<string>("way");
		    if ( ( way == "RIGHT" ) || ( way == "right" ) )
		      row.set_way ( RIGHT );
		    else
      		      row.set_way ( LEFT );
		    row.set_distance ( v.second.get<double>("distance") );

		    lvl.getCrater().addRow ( row );
                  }
              }

            BOOST_FOREACH ( ptree::value_type const& v, levels.get_child("road") )
              {
                if ( v.first == "row" )
                  {
		    row.clear();
		    way = "";

       		    row.set_name ( v.second.get<string>("name") );
		    // row.set_num_elements ( v.second.get<unsigned>("elements") );
		    row.set_speed ( v.second.get<double>("speed") );
		    way = v.second.get<string>("way");
		    if ( ( way == "RIGHT" ) || ( way == "right" ) )
		      row.set_way ( RIGHT );
		    else
      		      row.set_way ( LEFT );
		    row.set_distance ( v.second.get<double>("distance") );

		    lvl.getRoad().addRow ( row );
                  }
              }

	    gc.addLevel ( lvl );
          }
      }

    is.close();
  }

XMLCharger::XMLCharger()
  {
  }

XMLCharger::~XMLCharger()
  {
  }
