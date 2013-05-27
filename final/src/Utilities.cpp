#include "Utilities.h"

template<> Utilities* Ogre::Singleton<Utilities>::msSingleton = 0;

Utilities& Utilities::getSingleton ()
{
    assert(msSingleton);
    return (*msSingleton);
}

Utilities* Utilities::getSingletonPtr ()
{
    assert(msSingleton);
    return msSingleton;
}

void Utilities::put_overlay ( Ogre::Rectangle2D* rect, const char* name, Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real bottom, bool visible )
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

void Utilities::put_background_with_rotation ( Ogre::SceneManager* m_pSceneMgr, const char* background )
  {
    // Create background material
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create ( "Background", "General" );
    material->getTechnique(0)->getPass(0)->createTextureUnitState ( background );
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    // Create background rectangle covering the whole screen
    Ogre::Rectangle2D* rect = new Ogre::Rectangle2D ( true );
    rect->setCorners ( -2.0, 1.0, 2.0, -1.0 );
    rect->setMaterial ( "Background" );

    // Render the background before everything else
    rect->setRenderQueueGroup ( Ogre::RENDER_QUEUE_BACKGROUND );

    // Use infinite AAB to always stay visible
    Ogre::AxisAlignedBox aabInf;
    aabInf.setInfinite();
    rect->setBoundingBox(aabInf);

    // Attach background to the scene
    Ogre::SceneNode* node = m_pSceneMgr->getRootSceneNode()->createChildSceneNode ( "Background" );
    node->attachObject ( rect );

    // Example of background scrolling
    //    material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation ( -0.015, 0.0 );
  }

void Utilities::put_element_in_scene ( Ogre::SceneManager* m_pSceneMgr, OgreBulletDynamics::DynamicsWorld* _world, string name_element )
  {
    Ogre::Entity *entity = m_pSceneMgr->createEntity ( name_element, name_element + string ( ".mesh" ) );
    Ogre::SceneNode *node = m_pSceneMgr->createSceneNode ( name_element );
    node->attachObject ( entity );

    m_pSceneMgr->getRootSceneNode()->addChild ( node );
    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = new
      OgreBulletCollisions::StaticMeshToShapeConverter ( entity );

    OgreBulletCollisions::TriangleMeshCollisionShape *trackTrimesh =
      trimeshConverter->createTrimesh();

    OgreBulletDynamics::RigidBody *rigidTrack = new
      OgreBulletDynamics::RigidBody ( name_element, _world );
    rigidTrack->setShape ( node, trackTrimesh, 0.8, 0.95, 0, Ogre::Vector3::ZERO,
			   Ogre::Quaternion::IDENTITY );

    delete trimeshConverter;
}
