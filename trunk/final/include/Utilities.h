#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "AdvancedOgreFramework.hpp"
#include <OgreRectangle2D.h>
#include <OgreMaterial.h>
#include <OgreAxisAlignedBox.h>
#include <OgreSceneNode.h>
#include <string>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
//#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include <OgreBulletDynamicsRigidBody.h>
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"

using namespace std;

class Utilities : public Ogre::Singleton<Utilities> {

 public:
  void put_overlay ( Ogre::Rectangle2D* rect, const char* name, Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real bottom, bool visible );
  void put_background_with_rotation ( Ogre::SceneManager* m_pSceneMgr,const char* background );
  void put_element_in_scene ( Ogre::SceneManager* m_pSceneMgr, OgreBulletDynamics::DynamicsWorld* _world, string name_element );

  static Utilities& getSingleton ();
  static Utilities* getSingletonPtr ();

 private:
  Utilities();
  virtual ~Utilities();

};

#endif
