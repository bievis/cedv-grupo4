#include "Coche.hpp"

// *** Metodos Publicos *** //

Coche::Coche()
  {
    init();
  }

Coche::Coche ( const string& name, float pos_x, float pos_y, float pos_z, Ogre::SceneManager* sceneMgr, OgreBulletDynamics::DynamicsWorld* world, eColour_Chassis color )
  {
    init();

    _name = name;

    _color = color;

    _x = pos_x;
    _y = pos_y;
    _z = pos_z;

    build ( sceneMgr, world );
  }

Coche::~Coche()
  {
  }

Coche::Coche ( const Coche& source )
  {
    copy ( source );
  }

Coche& Coche::operator= ( const Coche& source )
  {
    copy ( source );
    return *this;
  }

void Coche::setCarChassis ( OgreBulletDynamics::WheeledRigidBody* source )
  {
    _mCarChassis = source;
  }

void Coche::setTuning ( OgreBulletDynamics::VehicleTuning* source )
  {
    _mTuning = source;
  }

void Coche::setVehicleRayCaster ( OgreBulletDynamics::VehicleRayCaster* source )
  {
    _mVehicleRayCaster = source;
  }

void Coche::setVehicle ( OgreBulletDynamics::RaycastVehicle* source )
  {
    _mVehicle = source;
  }

void Coche::setChassis ( Ogre::Entity* source )
  {
    _mChassis = source;
  }

void Coche::setWheels ( unsigned int index, Ogre::Entity* source )
  {
    if ( _mWheels )
      {
	if ( _mWheels[index%4] )
	  delete _mWheels[index%4];

	// ATENCION: Copia de puntero porque es el Scene Manager quien nos dá el recurso
	//           y no es necesario crearse uno nuevo
	//******************************************************************************
	// _mWheels[index%4] = new Ogre::Entity ( *source );
	_mWheels[index%4] = source;
      }
  }

void Coche::setWheelNodes ( unsigned int index, Ogre::SceneNode* source )
  {
    if ( _mWheelNodes )
      {
	if ( _mWheelNodes[index%4] )
	  delete _mWheelNodes[index%4];

	// ATENCION: Copia de puntero porque es el Scene Manager quien nos dá el recurso
	//           y no es necesario crearse uno nuevo
	//******************************************************************************
	// _mWheelNodes[index%4] = new Ogre::SceneNode ( *source );
	_mWheelNodes[index%4] = source;
      }
  }

const char* Coche::getFilenameMesh()
  {
    string filename = "chassis_DEFAULT.mesh";

    switch ( _color )
      {
        case BLUE : filename = "chassis_BLUE.mesh"; break;
        case GREEN : filename = "chassis_GREEN.mesh"; break;
        case YELLOW : filename = "chassis_YELLOW.mesh"; break;
        case RED : filename = "chassis_RED.mesh"; break;
      }

    return filename.c_str();
  }

void Coche::build ( Ogre::SceneManager* sceneMgr, OgreBulletDynamics::DynamicsWorld* world )
  {
    char name[100];

    setWheelRadius ( 0.5f );
    setWheelWidth ( 0.4f );
    setWheelFriction ( 1e30f );
    setRollInfluence ( 0.1f );
    setSuspensionRestLength ( 0.6 );
    setEngineForce ( 4000.0 );

    const Ogre::Vector3 chassisShift(0, 1.0, 0);
    float connectionHeight = 0.7f;
    setSteering ( 0.0 );

    memset ( name, 0, sizeof(char)*100 );
    sprintf ( name, "%s_chassis", getName().c_str() );
    setChassis ( sceneMgr->createEntity ( name, getFilenameMesh() ) );
    SceneNode *node = sceneMgr->getRootSceneNode()->createChildSceneNode ();

    SceneNode *chassisnode = node->createChildSceneNode();
    chassisnode->attachObject ( getChassisPtr() );
    chassisnode->setPosition ( chassisShift );

    BoxCollisionShape* chassisShape = new BoxCollisionShape ( Ogre::Vector3 ( 1.f, 0.75f, 2.1f ) );
    CompoundCollisionShape* compound = new CompoundCollisionShape();
    compound->addChildShape ( chassisShape, chassisShift );

    memset ( name, 0, sizeof(char)*100 );
    sprintf ( name, "%s_carChassis", getName().c_str() );
    setCarChassis ( new WheeledRigidBody ( name, world ) );

    Vector3 CarPosition = Vector3 ( _x, _y, _z );
    getCarChassisPtr()->setShape ( node, compound, 0.6, 0.6, 800, CarPosition, Quaternion::IDENTITY );
    getCarChassisPtr()->setDamping(0.2, 0.2);
    getCarChassisPtr()->disableDeactivation();

    setTuning ( new VehicleTuning(20.2, 4.4, 2.3, 500.0, 10.5) );

    setVehicleRayCaster ( new VehicleRayCaster ( world ) );

    setVehicle ( new RaycastVehicle ( getCarChassisPtr(), getTuningPtr(), getVehicleRayCasterPtr() ) );

    getVehiclePtr()->setCoordinateSystem(0, 1, 2);

    Ogre::Vector3 wheelDirectionCS0(0,-1,0);
    Ogre::Vector3 wheelAxleCS(-1,0,0);

    for (size_t i = 0; i < 4; i++)
      {
	memset ( name, 0, sizeof(char)*100 );
	sprintf ( name, "%s_wheel%d", getName().c_str(), (int)i );

	setWheels ( i, sceneMgr->createEntity ( name, "wheel.mesh" ) );
	getWheelsPtr(i)->setCastShadows ( true );

	setWheelNodes ( i, sceneMgr->getRootSceneNode()->createChildSceneNode() );
	getWheelNodesPtr(i)->attachObject ( getWheelsPtr(i) );
      }

    bool isFrontWheel = true;
    float wheelRadius = 2 - getWheelRadius();
    float wheelWidth = 1 - ( 0.3 * getWheelWidth() );
    Ogre::Vector3 connectionPointCS0 ( 1 - ( 0.3 * getWheelWidth() ),
				       connectionHeight, 2 - getWheelRadius() );

    for ( unsigned int i = 0; i < 4; i++ )
      {
	getVehiclePtr()->addWheel ( getWheelNodesPtr(i), connectionPointCS0, wheelDirectionCS0,
				    wheelAxleCS, getSuspensionRestLength(), getWheelRadius(),
				    isFrontWheel, getWheelFriction(), getRollInfluence() );
	if ( i == 1 )
	  { // Para i = 2 e i = 3 son las ruedas traseras
	    isFrontWheel = false;
	    wheelRadius = -2 + getWheelRadius();
	  }
	else if ( i == 0 )
	  {
	    wheelWidth = -1 + ( 0.3 * getWheelWidth() );
	  }
	else if ( i == 2 )
	  {
	    wheelWidth = 1 - ( 0.3 * getWheelWidth() );
	  }

	connectionPointCS0 = Ogre::Vector3 ( wheelWidth, connectionHeight, wheelRadius );
      }

  }

void Coche::clear()
  {
    if ( _mChassis )
      delete _mChassis;

    if ( _mTuning )
      delete _mTuning;

    if ( _mVehicleRayCaster )
      delete _mVehicleRayCaster;

    if ( _mVehicle )
      delete _mVehicle;

    if ( _mCarChassis )
      delete _mCarChassis;

    for ( unsigned int i = 0; i < 4; i++ )
      {
	if ( _mWheels[i] )
	  delete _mWheels[i];
      	if ( _mWheelNodes[i] )
	  delete _mWheelNodes[i];
      }

    init();
  }

void Coche::accelerate ( bool endereza )
  {
    // Para enderezar las ruedas si no se pulsa hacia la izquierda o la derecha
    if ( _mSteering != 0.0 && endereza )
      {
	if ( _mSteering > 0 )
	  _mSteering -= 0.01;
	else if ( _mSteering < 0 )
	  _mSteering += 0.01;

	_mVehicle->setSteeringValue ( _mSteering, 0 );
	_mVehicle->setSteeringValue ( _mSteering, 1 );
      }

     _mVehicle->applyEngineForce ( _engineForce, 0 );
     _mVehicle->applyEngineForce ( _engineForce, 1 );
  }

void Coche::decelerate ( bool endereza )
  {
    // Para enderezar las ruedas si no se pulsa hacia la izquierda o la derecha
    if ( _mSteering != 0.0 && endereza )
      {
	if ( _mSteering > 0 )
	  _mSteering -= 0.01;
	else if ( _mSteering < 0 )
	  _mSteering += 0.01;

	_mVehicle->setSteeringValue ( _mSteering, 0 );
	_mVehicle->setSteeringValue ( _mSteering, 1 );
      }

    _mVehicle->applyEngineForce ( (-1) * _engineForce, 0 );
    _mVehicle->applyEngineForce ( (-1) * _engineForce, 1 );
  }

void Coche::turn_left()
  {
    if ( _mSteering < 0.6 )
      _mSteering += 0.01;

    _mVehicle->setSteeringValue ( _mSteering, 0 );
    _mVehicle->setSteeringValue ( _mSteering, 1 );
  }

void Coche::turn_right()
  {
    if ( _mSteering > -0.6 )
      _mSteering -= 0.01;

    _mVehicle->setSteeringValue ( _mSteering, 0 );
    _mVehicle->setSteeringValue ( _mSteering, 1 );
  }

void Coche::print_info()
  {
    unsigned int i = 0;

    cout << "============================================" << endl;
    cout << "Car Info" << endl;
    cout << "============================================" << endl;
    cout << "Name                        : " << _name << endl;
    cout << "Color                       : ";
    switch ( _color )
      {
        case DEFAULT: cout << "DEFAULT" << endl; break;
        case RED: cout << "RED" << endl; break;
        case BLUE: cout << "BLUE" << endl; break;
        case GREEN: cout << "GREEN" << endl; break;
        case YELLOW: cout << "YELLOW" << endl; break;
        default: cout << "UNKNOWN" << endl;
      }
    cout <<
    cout << "Wheel Radius                : " << _wheelRadius << endl;
    cout << "Wheel Width                 : " << _wheelWidth << endl;
    cout << "Wheel Friction              : " << _wheelFriction << endl;
    cout << "Roll Influence              : " << _rollInfluence << endl;
    cout << "Suspension Rest Length      : " << _suspensionRestLength << endl;
    cout << "Engine Force                : " << _engineForce << endl;
    cout << "Initial coordenates [X,Y,Z] : [" << _x << "," << _y << "," << _z << "]" << endl;
    printf ( "Car Chassis Ref.             : 0x%x\n", _mCarChassis );
    printf ( "Vehicle Tuning Ref.          : 0x%x\n", _mTuning );
    printf ( "Vehicle RayCaster Ref.       : 0x%x\n", _mVehicleRayCaster );
    printf ( "Raycast Vehicle Ref.         : 0x%x\n", _mVehicle );
    printf ( "Entity Chassis Ref.          : 0x%x\n", _mChassis );
    printf ( "Entity Wheels Ref.\n" );
    for ( i = 0; i < 4; i++ )
      printf ( "    [ %d ]                   : 0x%x\n", i, _mWheels[i] );
    printf ( "Scene Node Wheels Ref.\n" );
    for ( i = 0; i < 4; i++ )
      printf ( "    [ %d ]                   : 0x%x\n", i, _mWheelNodes[i] );
    cout << "Steering                      : " << _mSteering << endl;
    cout << "============================================" << endl;
  }

// *** Metodos Privados *** //

void Coche::copy ( const Coche& source )
  {
    clear();

    _name = source.getName();

    _wheelRadius = source.getWheelRadius();
    _wheelWidth = source.getWheelWidth();
    _wheelFriction = source.getWheelFriction();
    _rollInfluence = source.getRollInfluence();
    _suspensionRestLength = source.getSuspensionRestLength();
    _engineForce = source.getEngineForce();

    if ( source.getCarChassisPtr() )
      setCarChassis ( source.getCarChassisPtr() );
    if ( source.getTuningPtr() )
      setTuning ( source.getTuningPtr() );
    if ( source.getVehicleRayCasterPtr() )
      setVehicleRayCaster ( source.getVehicleRayCasterPtr() );
    if ( source.getVehiclePtr() )
      setVehicle ( source.getVehiclePtr() );
    if ( source.getCarChassisPtr() )
      setCarChassis ( source.getCarChassisPtr() );
    for ( unsigned int i = 0; i < 4; i++ )
      {
	if ( source.getWheelsPtr(i) )
	  setWheels ( i, source.getWheelsPtr(i) );
	if ( source.getWheelNodesPtr(i) )
	  setWheelNodes ( i, source.getWheelNodesPtr(i) );
      }

  }

void Coche::init()
  {
    _name = "";
    _wheelRadius = 0;
    _wheelWidth = 0;
    _wheelFriction = 0;
    _rollInfluence = 0;
    _suspensionRestLength = 0;
    _engineForce = 0;
    _mChassis = NULL;
    _mTuning = NULL;
    _mVehicleRayCaster = NULL;
    _mVehicle = NULL;
    _mCarChassis = NULL;
    for ( unsigned int i = 0; i < 4; i++ )
      {
	_mWheels[i] = NULL;
	_mWheelNodes[i] = NULL;
      }
  }
