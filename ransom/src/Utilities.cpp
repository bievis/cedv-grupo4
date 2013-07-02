#include "Utilities.h"
#include <OgreVector3.h>

Utilities* Utilities::msSingleton = NULL;

Utilities& Utilities::getSingleton()
  {
    if ( NULL == msSingleton )
      msSingleton = new Utilities;

    return (*msSingleton);
  }

Utilities* Utilities::getSingletonPtr()
  {
    if ( NULL == msSingleton )
      msSingleton = new Utilities();

    return (msSingleton);
  }

void Utilities::put_overlay ( Ogre::OverlayManager *overMgr, const char* name, bool visible )
{
    Ogre::Overlay *over = NULL;

    over = overMgr->getByName ( name );

    if ( over )
      {
        if ( visible )
            over->show();
        else
            over->hide();
      }
    else
      {
        cerr << "Overlay name was not found!!" << endl;
      }
}

void Utilities::put_overlay_on_rectangle ( Ogre::Rectangle2D* rect, const char* name, Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real bottom, bool visible )
  {
    // Create background rectangle covering the whole screen
    rect = new Ogre::Rectangle2D ( true );
    rect->setCorners ( left, top, right, bottom );
    rect->setMaterial ( name );

    // Render the background before everything else
    rect->setRenderQueueGroup ( Ogre::RENDER_QUEUE_BACKGROUND );

    rect->setVisible ( visible );
  }

void Utilities::put_background_with_rotation ( Ogre::SceneManager* sceneMgr, const char* background )
  {
    // Create background material
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create ( "Background", "General" );
    material->getTechnique(0)->getPass(0)->createTextureUnitState ( background );
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    Ogre::Rectangle2D* rect = NULL;
    put_overlay_on_rectangle ( rect, "Background", -2.0, 1.0, 2.0, -1.0, true );

    // Create background rectangle covering the whole screen
//    Ogre::Rectangle2D* rect = new Ogre::Rectangle2D ( true );
//    rect->setCorners ( -2.0, 1.0, 2.0, -1.0 );
//    rect->setMaterial ( "Background" );
//
//    // Render the background before everything else
//    rect->setRenderQueueGroup ( Ogre::RENDER_QUEUE_BACKGROUND );

    // Use infinite AAB to always stay visible
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    rect->setBoundingBox(aabInf);

    // Attach background to the scene
    Ogre::SceneNode* node = sceneMgr->getRootSceneNode()->createChildSceneNode ( "Background" );
    node->attachObject ( rect );

    // Example of background scrolling
    material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation ( -0.015, 0.0 );
  }

void Utilities::put_element_in_scene ( Ogre::SceneManager* sceneMgr,
                                                  OgreBulletDynamics::DynamicsWorld* world,
                                                  string name_mesh,
                                                  string name_element,
                                                  const Ogre::Vector3& initial_pos )
  {
    Ogre::Vector3 size = Ogre::Vector3::ZERO;
    Ogre::Entity *entity = sceneMgr->createEntity ( name_element, name_mesh + string ( ".mesh" ) );
    Ogre::SceneNode *node = sceneMgr->createSceneNode ( name_element );
    entity->setCastShadows(true);
    node->attachObject ( entity );

    sceneMgr->getRootSceneNode()->addChild ( node );

    Ogre::AxisAlignedBox boundingB = entity->getBoundingBox();

    size = boundingB.getSize();

    OgreBulletCollisions::BoxCollisionShape *sceneBoxShape = new OgreBulletCollisions::BoxCollisionShape(size);

    OgreBulletDynamics::RigidBody *rigidTrack = new
      OgreBulletDynamics::RigidBody ( name_element, world );

    rigidTrack->setStaticShape ( node, sceneBoxShape, 0.6f, 0.6f, Ogre::Vector3 ( initial_pos.x, initial_pos.y, initial_pos.z ),
			   Ogre::Quaternion::IDENTITY );

}

void Utilities::put_shot_in_scene ( Ogre::SceneManager* sceneMgr,
												  string name,
												  Ogre::SceneNode* nodeShot,
												  Ogre::ManualObject** shot)
  {
    *shot =  sceneMgr->createManualObject(name + "LineSHOT");
	(*shot)->begin("MaterialBlanco", Ogre::RenderOperation::OT_LINE_LIST); 
	(*shot)->position(Ogre::Vector3::ZERO);
	Ogre::Vector3 posFin(nodeShot->getOrientation().zAxis() * 1);
	(*shot)->position(posFin);
	(*shot)->end();
    (*shot)->setCastShadows(true);
    nodeShot->attachObject ( (*shot) );
}

void Utilities::put_plane_in_scene ( Ogre::SceneManager* sceneMgr,
                                                  OgreBulletDynamics::DynamicsWorld* world,
												  Ogre::StaticGeometry *staticGeometry,
                                                  string namePlane,
												  string nameMaterial,
												  float width,
												  float height,
												  Ogre::Vector3 upVector,
												  Ogre::Vector3 normal,
                                                  const Ogre::Vector3& initial_pos ) {
	// Define a floor plane mesh
	Ogre::Entity *entPlane;
	Ogre::Plane plane;
	plane.normal = normal; plane.d = 0;
	Ogre::MeshManager::getSingleton().createPlane ( namePlane + "PlaneMesh",
								Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
								plane, width, height,
								1, 1, true, 1, 1, 1,
								upVector );
	// Create an entity plane
	entPlane = sceneMgr->createEntity(namePlane  + "PlaneEntity", namePlane  + "PlaneMesh");
	entPlane->setMaterialName(nameMaterial);
	entPlane->setCastShadows(true);

	staticGeometry->addEntity(entPlane, initial_pos);

	// add collision detection to it
	OgreBulletCollisions::CollisionShape *shapePlane = new OgreBulletCollisions::StaticPlaneCollisionShape ( normal, 0 ); // (normal vector, distance)
	// a body is needed for the shape
	OgreBulletDynamics::RigidBody *defaultPlaneBody = new OgreBulletDynamics::RigidBody ( namePlane  + "PlaneBody", world );
	defaultPlaneBody->setStaticShape ( shapePlane, 0.1, 0.8, initial_pos);// (shape, restitution, friction, position)
}

 void Utilities::put_part_map_in_scene ( Ogre::SceneManager* sceneMgr,
                                                  OgreBulletDynamics::DynamicsWorld* world,
												  Ogre::StaticGeometry *staticGeometry,
                                                  string namePart,
												  string namePartMesh,
                                                  const Ogre::Vector3& initial_pos) {
	Ogre::Entity *entity = sceneMgr->createEntity(namePart, namePartMesh + ".mesh");
	entity->setCastShadows(true);
	staticGeometry->addEntity(entity, initial_pos);
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new
    OgreBulletCollisions::StaticMeshToShapeConverter(entity);
    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimesh =
    trimeshConverter->createTrimesh();
    OgreBulletDynamics::RigidBody *rigidTrack = new
    OgreBulletDynamics::RigidBody(namePart, world);
	rigidTrack->setStaticShape(trackTrimesh, 0.8, 0.95, initial_pos);
	delete trimeshConverter;
 }

void Utilities::put_character_in_scene ( Ogre::SceneManager* sceneMgr,
                                                  OgreBulletDynamics::DynamicsWorld* world,
                                                  string name_mesh,
                                                  string name_element,
                                                  const Ogre::Vector3& initial_pos,
                                                  Ogre::Entity** entity,
                                                  Ogre::SceneNode** node,
                                                  OgreBulletDynamics::RigidBody** rigidTrack,
                                                  bool visible,
                                                  string initAnimation )
  {
    *entity = sceneMgr->createEntity ( name_element, name_mesh + string ( ".mesh" ) );
    (*entity)->setCastShadows(true);
    (*entity)->setVisible(visible);

    *node = sceneMgr->createSceneNode ( name_element );
    (*node)->attachObject ( *entity );

    if (initAnimation != "") {
        Ogre::AnimationState *animation = (*entity)->getAnimationState(initAnimation);
        animation->setEnabled(true);
        animation->setLoop(true);
    }

    sceneMgr->getRootSceneNode()->addChild ( *node );

    OgreBulletCollisions::CollisionShape* sceneBoxShape = NULL;

    if (initAnimation == "") {
        OgreBulletCollisions::StaticMeshToShapeConverter* trimeshConverter = new
          OgreBulletCollisions::StaticMeshToShapeConverter ( *entity );

        sceneBoxShape = (OgreBulletCollisions::CollisionShape*) trimeshConverter->createConvex();

        delete trimeshConverter;
    } else {
        OgreBulletCollisions::AnimatedMeshToShapeConverter* trimeshConverter = new
          OgreBulletCollisions::AnimatedMeshToShapeConverter ( *entity );

        sceneBoxShape = (OgreBulletCollisions::CollisionShape*) trimeshConverter->createConvex();

        delete trimeshConverter;
    }

    *rigidTrack = new OgreBulletDynamics::RigidBody ( name_element, world );

    (*rigidTrack)->setShape ( *node, sceneBoxShape, 0.6, 0.6, 80.0f,
                            initial_pos, (*node)->_getDerivedOrientation() );
    (*rigidTrack)->getBulletRigidBody()->setAngularFactor(0.0f);
  }
