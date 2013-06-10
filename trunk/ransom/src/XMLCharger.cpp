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

void XMLCharger::LoadFile ( string routeAbsoluteFile, GameConfig &gc )
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

    read_xml ( is, pt );

    float posX, posY, posZ;

    BOOST_FOREACH ( ptree::value_type const& v, pt.get_child("config") )
      {
        if ( v.first == "hero" )
          {

            posX = v.second.get<float>("<xmlattr>.posX");
            posY = v.second.get<float>("<xmlattr>.posY");
            posZ = v.second.get<float>("<xmlattr>.posZ");

            gc.setInitialPosHero ( Ogre::Vector3 ( posX, posY, posZ ) );

          }
        else if ( v.first == "enemy" )
          {

            gc.setNumEnemies ( v.second.get<unsigned int>("<xmlattr>.num") );

            boost::property_tree::ptree routes = (boost::property_tree::ptree) v.second;

            BOOST_FOREACH ( ptree::value_type const& v2, routes.get_child("enemy_routes") )
              {
                if ( v2.first == "route" )
                  {

                    EnemyRoute route;

                    unsigned int id = v2.second.get<unsigned int>("<xmlattr>.id");

                    route.setID ( id );

                    std::string cad = v2.second.get<std::string>("<xmlattr>.closed");

                    if ( cad == "true" )
                      route.setRouteClosed ( true );
                    else
                      route.setRouteClosed ( false );

                    boost::property_tree::ptree points = (boost::property_tree::ptree) v2.second;

                    BOOST_FOREACH ( ptree::value_type const& v3, points.get_child("points") )
                      {

                        if ( v3.first == "point" )
                          {
                              posX = v3.second.get<float>("<xmlattr>.posX");
                              posY = v3.second.get<float>("<xmlattr>.posY");
                              posZ = v3.second.get<float>("<xmlattr>.posZ");
                              route.addPoint ( Ogre::Vector3 ( posX, posY, posZ ) );
                          }
                      }

                    gc.addEnemyRoute ( route );

                    route.clear();
                  }
              }

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
