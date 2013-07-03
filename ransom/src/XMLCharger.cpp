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

void XMLCharger::LoadGameConfig ( const string& routeAbsoluteFile, GameConfig &gc )
  {
    ifstream is;
    unsigned int numEnemies = 0;

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
        else if ( v.first == "map" )
          {

            boost::property_tree::ptree pieces = (boost::property_tree::ptree) v.second;

            BOOST_FOREACH ( ptree::value_type const& v1, v.second )
              {

                if ( v1.first == "piece" )
                  {
                    Piece piece;

                    BOOST_FOREACH ( ptree::value_type const& v11, v1.second )
                      {

                        if ( v11.first == "<xmlattr>" )
                          {
                            float value = 0;

                            posX = v11.second.get<float>("posX");
                            posY = v11.second.get<float>("posY");
                            posZ = v11.second.get<float>("posZ");
                            piece.setPosition ( Ogre::Vector3 ( posX, posY, posZ ) );

                            value = v11.second.get<float>("width");
                            piece.setWidth ( value );

                            value = v11.second.get<float>("height");
                            piece.setHeight ( value );
                          }
                        else if ( v11.first == "routes" )
                          {

                            BOOST_FOREACH ( ptree::value_type const& v2, v11.second )
                              {

                                if ( v2.first == "route" )
                                  {
                                    numEnemies++;

                                    EnemyRoute *route = new EnemyRoute();

                                    unsigned int id = v2.second.get<unsigned int>("<xmlattr>.id");

                                    route->setID ( id );

                                    std::string cad = v2.second.get<std::string>("<xmlattr>.closed");

                                    route->setProximity ( v2.second.get<float>("<xmlattr>.proximity") );

                                    if ( cad == "true" )
                                      route->setRouteClosed ( true );
                                    else
                                      route->setRouteClosed ( false );

                                    boost::property_tree::ptree points = (boost::property_tree::ptree) v2.second;

                                    BOOST_FOREACH ( ptree::value_type const& v3, points.get_child("points") )
                                      {

                                        if ( v3.first == "point" )
                                          {
                                              posX = v3.second.get<float>("<xmlattr>.posX");
                                              posY = v3.second.get<float>("<xmlattr>.posY");
                                              posZ = v3.second.get<float>("<xmlattr>.posZ");
                                              route->addPoint ( Ogre::Vector3 ( posX, posY, posZ ) );
                                          }
                                      }

                                    piece.getEnemyRoutes().push_back ( route );
                                    gc.addEnemyRoute ( *route );
                                  }
                              }
                          }
                      }

                    gc.addPiece ( piece );
                  }

              }
          }
      }

      gc.setNumEnemies ( numEnemies );

  }

//void XMLCharger::LoadMap ( const string& routeAbsoluteMap, GameConfig& gc )
//  {
//    ifstream is;
//
//    try {
//      is.open ( routeAbsoluteMap.c_str() );
//    }
//    catch ( std::ifstream::failure e ) {
//      std::cerr << "Exception opening/reading file '" << routeAbsoluteMap << "'" << endl;
//      return;
//    }
//
//    ptree pt;
//
//    read_xml ( is, pt );
//
//    int width, height, gid, cont, fila, col;
//
//    BOOST_FOREACH ( ptree::value_type const& v, pt.get_child("map") )
//      {
//        if ( v.first == "layer" )
//          {
//            width = v.second.get<int>("<xmlattr>.width");
//            gc.setPlaneWidth ( width );
//            height = v.second.get<int>("<xmlattr>.height");
//            gc.setPlaneHeight ( height );
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
//                      fila = (( cont % width == 0)?( cont / width ):(( cont / width ) + 1));
//                      col = (( cont % width == 0)?320:( cont % width ));
//                      gc.addColumnPosition ( Ogre::Vector3 ( fila, 0, col ) );
//                    }
//
//                  cont++;
//                }
//              }
//          }
//      }
//
//    is.close();
//
//}

XMLCharger::XMLCharger()
  {
  }

XMLCharger::~XMLCharger()
  {
  }
