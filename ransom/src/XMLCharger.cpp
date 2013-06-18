#include <XMLCharger.h>
#include <StateMachine.h>

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

void XMLCharger::LoadGameConfig ( const string& routeAbsoluteFile, GameConfig &gc )
  {
    ifstream is;

    // Lectura del fichero de configuración del juego

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
//        else if ( v.first == "stage" )
//          {
//
//            unsigned int value = 0;
//            value = v.second.get<float>("<xmlattr>.height");
//            gc.setPlaneHeight ( value );
//            value = v.second.get<float>("<xmlattr>.width");
//            gc.setPlaneWidth ( value );
//
//            boost::property_tree::ptree columns = (boost::property_tree::ptree) v.second;
//
//            BOOST_FOREACH ( ptree::value_type const& v2, columns.get_child("columns") )
//              {
//                if ( v2.first == "column" )
//                  {
//                    posX = v2.second.get<float>("<xmlattr>.posX");
//                    posY = v2.second.get<float>("<xmlattr>.posY");
//                    posZ = v2.second.get<float>("<xmlattr>.posZ");
//
//                    gc.addColumnPosition ( Ogre::Vector3 ( posX, posY, posZ ) );
//                  }
//              }
//
//          }

        else if ( v.first == "hostage" )
          {

            boost::property_tree::ptree positions = (boost::property_tree::ptree) v.second;

            BOOST_FOREACH ( ptree::value_type const& v2, positions.get_child("hostage_positions") )
              {
                if ( v2.first == "position" )
                  {
                    posX = v2.second.get<float>("<xmlattr>.posX");
                    posY = v2.second.get<float>("<xmlattr>.posY");
                    posZ = v2.second.get<float>("<xmlattr>.posZ");

                    gc.addHostagePosition ( Ogre::Vector3 ( posX, posY, posZ ) );
                  }
              }

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

                    route.setProximity ( v2.second.get<float>("<xmlattr>.proximity") );

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

//    // Lectura del mapa
//
//    try {
//      is.open ( routeAbsoluteFileMap.c_str() );
//    }
//    catch ( std::ifstream::failure e ) {
//      std::cerr << "Exception opening/reading file '" << routeAbsoluteFileMap << "'" << endl;
//      return;
//    }
//
//    ptree pt2;
//
//    read_xml ( is, pt2 );
//
//    int width, height, gid, cont;
//
//    BOOST_FOREACH ( ptree::value_type const& v, pt2.get_child("map") )
//      {
//        // <layer name="Mapa" width="320" height="320">
//        if ( v.first == "layer" )
//          {
//            width = v.second.get<int>("<xmlattr>.width");
//            height = v.second.get<int>("<xmlattr>.height");
//
//            cont = 1;
//
//            boost::property_tree::ptree data = (boost::property_tree::ptree) v.second;
//
//            BOOST_FOREACH ( ptree::value_type const& v2, data.get_child("data") )
//              {
//                if ( v2.first == "tile" )
//                {
//                  gid = v2.second.get<int>("<xmlattr>.gid");
//
//                  if ( gid != 0 )
//                    {
//                      cout << "Fila, Columna = " << cont / width << ", " << cont % width << endl;
//                    }
//
//                  cont++;
//                }
//              }
//          }
//      }
//
//    is.close();
  }

void XMLCharger::LoadMap ( const string& routeAbsoluteMap, GameConfig& gc )
  {
    ifstream is;

    // Lectura del mapa

    try {
      is.open ( routeAbsoluteMap.c_str() );
    }
    catch ( std::ifstream::failure e ) {
      std::cerr << "Exception opening/reading file '" << routeAbsoluteMap << "'" << endl;
      return;
    }

    ptree pt;

    read_xml ( is, pt );

    int width, height, gid, cont, fila, col;

    BOOST_FOREACH ( ptree::value_type const& v, pt.get_child("map") )
      {
        // <layer name="Mapa" width="320" height="320">
        if ( v.first == "layer" )
          {
            width = v.second.get<int>("<xmlattr>.width");
            gc.setPlaneWidth ( width );
            height = v.second.get<int>("<xmlattr>.height");
            gc.setPlaneHeight ( height );

//            boost::property_tree::ptree columns = (boost::property_tree::ptree) v.second;
//
//            BOOST_FOREACH ( ptree::value_type const& v2, columns.get_child("columns") )
//              {
//                if ( v2.first == "column" )
//                  {
//                    posX = v2.second.get<float>("<xmlattr>.posX");
//                    posY = v2.second.get<float>("<xmlattr>.posY");
//                    posZ = v2.second.get<float>("<xmlattr>.posZ");
//
//                    gc.addColumnPosition ( Ogre::Vector3 ( posX, posY, posZ ) );
//                  }
//              }

            cont = 1;

            boost::property_tree::ptree data = (boost::property_tree::ptree) v.second;

            BOOST_FOREACH ( ptree::value_type const& v2, data.get_child("data") )
              {
                if ( v2.first == "tile" )
                {
                  gid = v2.second.get<int>("<xmlattr>.gid");

                  if ( gid != 0 )
                    {
                      fila = (( cont % width == 0)?( cont / width ):(( cont / width ) + 1));
                      col = (( cont % width == 0)?320:( cont % width ));
//                      cout << "Fila, Columna = " << fila << ", " << col << endl;
                      gc.addColumnPosition ( Ogre::Vector3 ( fila, 0, col ) );
                    }

                  cont++;
                }
              }
          }
      }

    is.close();

}

bool XMLCharger::LoadStateMachine ( const string& routeAbsoluteFile, StateMachine &sm )
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

XMLCharger::XMLCharger()
  {
  }

XMLCharger::~XMLCharger()
  {
  }
