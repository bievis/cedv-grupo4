#include "Coche.hpp"

// *** Metodos Publicos *** //

Coche::Coche()
  {
    init();
  }

Coche::Coche ( const string& name, float pos_x, float pos_y, float pos_z, Ogre::SceneManager* sceneMgr, OgreBulletDynamics::DynamicsWorld* world )
  {
    init();

    _name = name;

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
    // if ( _mCarChassis )
    //   delete _mCarChassis;

    // _mCarChassis = new OgreBulletDynamics::WheeledRigidBody ( *source );
    _mCarChassis = source;
  }

void Coche::setTuning ( OgreBulletDynamics::VehicleTuning* source )
  {
    // if ( _mTuning )
    //   delete _mTuning;

    // _mTuning = new OgreBulletDynamics::VehicleTuning ( *source );
    _mTuning = source;
  }

void Coche::setVehicleRayCaster ( OgreBulletDynamics::VehicleRayCaster* source )
  {
    // if ( _mVehicleRayCaster )
    //   delete _mVehicleRayCaster;

    // _mVehicleRayCaster = new OgreBulletDynamics::VehicleRayCaster ( *source );
    _mVehicleRayCaster = source;
  }

void Coche::setVehicle ( OgreBulletDynamics::RaycastVehicle* source )
  {
    // if ( _mVehicle )
    //   delete _mVehicle;

    // _mVehicle = new OgreBulletDynamics::RaycastVehicle ( *source );
    _mVehicle = source;
  }

void Coche::setChassis ( Ogre::Entity* source )
  {
    // if ( _mChassis )
    //   delete _mChassis;

    // _mChassis = new Ogre::Entity ( *source );
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

void Coche::build ( Ogre::SceneManager* sceneMgr, OgreBulletDynamics::DynamicsWorld* world )
  {
    char name[100];

    setWheelRadius ( 0.5f );
    setWheelWidth ( 0.4f );
    setWheelFriction ( 1e30f );
    setRollInfluence ( 0.1f );
    setSuspensionRestLength ( 0.6 );
    setEngineForce ( 1000.0 );

    const Ogre::Vector3 chassisShift(0, 1.0, 0);
    float connectionHeight = 0.7f;
    // mSteering = 0.0;
    setSteering ( 0.0 );

    memset ( name, 0, sizeof(char)*100 );
    sprintf ( name, "%s_chassis", getName().c_str() );
    // mChassis = msceneMgr->createEntity("chassis", "chassis.mesh");
    setChassis ( sceneMgr->createEntity ( name, "chassis.mesh" ) );
    SceneNode *node = sceneMgr->getRootSceneNode()->createChildSceneNode ();

    SceneNode *chassisnode = node->createChildSceneNode();
    // chassisnode->attachObject (mChassis);
    chassisnode->attachObject ( getChassisPtr() );
    chassisnode->setPosition ( chassisShift );

    BoxCollisionShape* chassisShape = new BoxCollisionShape ( Ogre::Vector3 ( 1.f, 0.75f, 2.1f ) );
    CompoundCollisionShape* compound = new CompoundCollisionShape();
    compound->addChildShape ( chassisShape, chassisShift );

    memset ( name, 0, sizeof(char)*100 );
    sprintf ( name, "%s_carChassis", getName().c_str() );
    // mCarChassis = new WheeledRigidBody("carChassis", world);
    setCarChassis ( new WheeledRigidBody ( name, world ) );

    // Vector3 CarPosition = Vector3(0, 0, -15);
    Vector3 CarPosition = Vector3 ( _x, _y, _z );
    // mCarChassis->setShape (node, compound, 0.6, 0.6, 800, CarPosition, Quaternion::IDENTITY);
    // mCarChassis->setDamping(0.2, 0.2);
    // mCarChassis->disableDeactivation();
    getCarChassisPtr()->setShape ( node, compound, 0.6, 0.6, 800, CarPosition, Quaternion::IDENTITY );
    getCarChassisPtr()->setDamping(0.2, 0.2);
    getCarChassisPtr()->disableDeactivation();

    // mTuning = new VehicleTuning(20.2, 4.4, 2.3, 500.0, 10.5);
    setTuning ( new VehicleTuning(20.2, 4.4, 2.3, 500.0, 10.5) );

    // mVehicleRayCaster = new VehicleRayCaster(world);
    setVehicleRayCaster ( new VehicleRayCaster ( world ) );

    // mVehicle = new RaycastVehicle(mCarChassis, mTuning, mVehicleRayCaster);
    setVehicle ( new RaycastVehicle ( getCarChassisPtr(), getTuningPtr(), getVehicleRayCasterPtr() ) );

    // mVehicle->setCoordinateSystem(0, 1, 2);
    getVehiclePtr()->setCoordinateSystem(0, 1, 2);

    Ogre::Vector3 wheelDirectionCS0(0,-1,0);
    Ogre::Vector3 wheelAxleCS(-1,0,0);

    for (size_t i = 0; i < 4; i++)
      {
	memset ( name, 0, sizeof(char)*100 );
	sprintf ( name, "%s_wheel%d", getName().c_str(), (int)i );
	// mWheels[i] = sceneMgr->createEntity ( "wheel" + i, "wheel.mesh" );
	// mWheels[i]->setCastShadows(true);

	setWheels ( i, sceneMgr->createEntity ( name, "wheel.mesh" ) );
	getWheelsPtr(i)->setCastShadows ( true );

	// mWheelNodes[i] = sceneMgr->getRootSceneNode()->createChildSceneNode();
	// mWheelNodes[i]->attachObject ( mWheels[i] );
	setWheelNodes ( i, sceneMgr->getRootSceneNode()->createChildSceneNode() );
	getWheelNodesPtr(i)->attachObject ( getWheelsPtr(i) );
      }

    bool isFrontWheel = true;
    Ogre::Vector3 connectionPointCS0 ( 1 - ( 0.3 * getWheelWidth() ),
				       connectionHeight, 2 - getWheelRadius() );

    // mVehicle->addWheel(mWheelNodes[0], connectionPointCS0, wheelDirectionCS0,
    // 		       wheelAxleCS, gSuspensionRestLength, gWheelRadius,
    // 		       isFrontWheel, gWheelFriction, gRollInfluence);
    getVehiclePtr()->addWheel ( getWheelNodesPtr(0), connectionPointCS0, wheelDirectionCS0,
				      wheelAxleCS, getSuspensionRestLength(), getWheelRadius(),
				      isFrontWheel, getWheelFriction(), getRollInfluence() );

    // connectionPointCS0 = Ogre::Vector3(-1+(0.3*gWheelWidth),
    // 				       connectionHeight, 2-gWheelRadius);
    connectionPointCS0 = Ogre::Vector3 ( -1 + ( 0.3 * getWheelWidth() ),
					 connectionHeight, 2 - getWheelRadius() );

    // mVehicle->addWheel(mWheelNodes[1], connectionPointCS0,
    // 		       wheelDirectionCS0, wheelAxleCS, gSuspensionRestLength,
    // 		       gWheelRadius, isFrontWheel, gWheelFriction, gRollInfluence);
    getVehiclePtr()->addWheel ( getWheelNodesPtr(1), connectionPointCS0,
				      wheelDirectionCS0, wheelAxleCS, getSuspensionRestLength(),
				      getWheelRadius(), isFrontWheel, getWheelFriction(), getRollInfluence() );

    isFrontWheel = false;
    // connectionPointCS0 = Ogre::Vector3(-1+(0.3*gWheelWidth),
    // 				       connectionHeight,-2+gWheelRadius);
    connectionPointCS0 = Ogre::Vector3 ( -1 + ( 0.3 * getWheelWidth() ),
					 connectionHeight , -2 + getWheelRadius() );

    // mVehicle->addWheel(mWheelNodes[2], connectionPointCS0,
    // 		       wheelDirectionCS0, wheelAxleCS, gSuspensionRestLength,
    // 		       gWheelRadius, isFrontWheel, gWheelFriction, gRollInfluence);
    getVehiclePtr()->addWheel ( getWheelNodesPtr(2), connectionPointCS0,
				      wheelDirectionCS0, wheelAxleCS, getSuspensionRestLength(),
				      getWheelRadius(), isFrontWheel, getWheelFriction(), getRollInfluence() );

    // connectionPointCS0 = Ogre::Vector3(1-(0.3*gWheelWidth),
    // 				       connectionHeight,-2+gWheelRadius);
    connectionPointCS0 = Ogre::Vector3 ( 1 - ( 0.3 * getWheelWidth() ),
					 connectionHeight, -2 + getWheelRadius() );

    // mVehicle->addWheel(mWheelNodes[3], connectionPointCS0,
    // 		       wheelDirectionCS0, wheelAxleCS, gSuspensionRestLength,
    // 		       gWheelRadius, isFrontWheel, gWheelFriction, gRollInfluence);
    getVehiclePtr()->addWheel ( getWheelNodesPtr(3), connectionPointCS0,
				      wheelDirectionCS0, wheelAxleCS, getSuspensionRestLength(),
				      getWheelRadius(), isFrontWheel, getWheelFriction(), getRollInfluence() );
  }
