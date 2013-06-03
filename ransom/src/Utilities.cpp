#include "Utilities.h"

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

    // Use infinite AAB to always stay visible
//    rect->setBoundingBox ( aabInf );

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

Ogre::SceneNode* Utilities::put_element_in_scene ( Ogre::SceneManager* sceneMgr,
                                                  OgreBulletDynamics::DynamicsWorld* world,
                                                  string name_mesh,
                                                  string name_element,
                                                  float initial_posX,
                                                  float initial_posY,
                                                  float initial_posZ )
  {
    Ogre::Vector3 size = Ogre::Vector3::ZERO;
    Ogre::Entity *entity = sceneMgr->createEntity ( name_element, name_mesh + string ( ".mesh" ) );
    Ogre::SceneNode *node = sceneMgr->createSceneNode ( name_element );
    entity->setCastShadows(true);
    node->attachObject ( entity );

    sceneMgr->getRootSceneNode()->addChild ( node );

    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new
      OgreBulletCollisions::StaticMeshToShapeConverter ( entity );

    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimesh =
      trimeshConverter->createTrimesh();

    OgreBulletDynamics::RigidBody *rigidTrack = new
      OgreBulletDynamics::RigidBody ( name_element, world );
    rigidTrack->setShape ( node, trackTrimesh, 0.6f, 0.6f, 1.0f, Ogre::Vector3 ( initial_posX, initial_posY, initial_posZ ),
			   Ogre::Quaternion::IDENTITY );

    delete trimeshConverter;

    return node;
}

Ogre::SceneNode* Utilities::put_cube_in_scene ( Ogre::SceneManager* sceneMgr,
                                                  OgreBulletDynamics::DynamicsWorld* world,
                                                  string name_mesh,
                                                  string name_element,
                                                  float initial_posX,
                                                  float initial_posY,
                                                  float initial_posZ,
                                                  eColor color )
  {
    Ogre::Vector3 size = Ogre::Vector3::ZERO;
    Ogre::Entity *entity = sceneMgr->createEntity ( name_element, name_mesh + string ( ".mesh" ) );
    Ogre::SceneNode *node = sceneMgr->createSceneNode ( name_element );
    entity->setCastShadows(true);

    if ( color == ROJO )
      entity->setMaterialName ( "MaterialRojo" );
    else
      entity->setMaterialName ( "MaterialAzul" );

    node->attachObject ( entity );

    sceneMgr->getRootSceneNode()->addChild ( node );

    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new
      OgreBulletCollisions::StaticMeshToShapeConverter ( entity );

    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimesh =
      trimeshConverter->createTrimesh();

    OgreBulletDynamics::RigidBody *rigidTrack = new
      OgreBulletDynamics::RigidBody ( name_element, world );
    rigidTrack->setShape ( node, trackTrimesh, 0.6f, 0.6f, 1.0f, Ogre::Vector3 ( initial_posX, initial_posY, initial_posZ ),
			   Ogre::Quaternion::IDENTITY );

    delete trimeshConverter;

    return node;
}
