#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include "AdvancedOgreFramework.hpp"
#include <OgreRectangle2D.h>
#include <OgreMaterial.h>
#include <OgreAxisAlignedBox.h>
#include <OgreSceneNode.h>
#include <OgreOverlay.h>
#include <string>
//#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>
#include <OgreBulletDynamicsRigidBody.h>
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"

using namespace std;

// Mesh file to load how to a Character
#define MESH_FILE_WITHOUT_EXTENSION "SWAT"

/// \brief this class will be a set of utility methods
class Utilities {

 public:

    /// \brief method to put an overlay in the scene
    /// This method put an overlay created in ransom.overlay file, also the name should be defined in this file with the set of specifications
    /// \param overMgr reference to overlay manager (Advanced Ogre Framework)
    /// \param name this name was set in the ransom.overlay file and this name identifies the overlay to be found in the file
    /// \param visible this boolean value set if the overlay will be visible from starting or will not
    void put_overlay ( Ogre::OverlayManager *overMgr, const char* name, bool visible );
    /// \brief method to put an overlay in an object Rectangle2D
    /// This method put an overlay created in ransom.material file, also the name should be defined in this file with the set of specifications
    /// \param rect referente to the object Rectangle2D where the overlay will be put
    /// \param name this name was set in the ransom.material file and this name identifies the overlay to be found in the file
    /// \param left value left corner
    /// \param top value top corner
    /// \param right value right corner
    /// \param bottom value bottom corner
    /// \param visible this boolean value set if the overlay will be visible from starting or will not
    void put_overlay_on_rectangle ( Ogre::Rectangle2D* rect, const char* name, Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real bottom, bool visible );
    /// \brief method to create a rotate background
    /// This method uses the method put_overlay_on_rectangle() to do part of it logic
    /// \param sceneMgr reference to the scene manager
    /// \param background name for the background
    void put_background_with_rotation ( Ogre::SceneManager* sceneMgr,const char* background );
    /// \brief this method add an element (mesh name) in the scene (Ogre) and in the world (OgreBullet)
    /// This method load in the scene (and in the world of OgreBullet) the object with the name (name + ".mesh") in the media/models folder
    /// \param sceneMgr reference to the scene manager (Ogre)
    /// \param world reference to world (OgreBullet)
    /// \param name_mesh name to charge mesh file
    /// \param name_element name to set at the scene element
    /// \param initial_posX initial coordenate in the axis X
    /// \param initial_posY initial coordenate in the axis Y
    /// \param initial_posZ initial coordenate in the axis Z
    /// \param visible set entity to visible or not
    /// \return reference to scene node created
    OgreBulletDynamics::RigidBody* put_element_in_scene ( Ogre::SceneManager* sceneMgr, OgreBulletDynamics::DynamicsWorld* world, string name_mesh, string name_element, float initial_posX, float initial_posY, float initial_posZ );
    void put_character_in_scene ( Ogre::SceneManager* sceneMgr,
                                                  OgreBulletDynamics::DynamicsWorld* world,
                                                  string name_mesh,
                                                  string name_element,
                                                  float initial_posX,
                                                  float initial_posY,
                                                  float initial_posZ,
                                                  Ogre::Entity** entity,
                                                  Ogre::SceneNode** node,
                                                  OgreBulletDynamics::RigidBody** rigidTrack,
                                                  bool visible,
                                                  string initAnimation );
    /// \brief method to get the Utilities object
    /// \return static Utilities object
    static Utilities& getSingleton();
    /// \brief method to get the Utilities reference
    /// \return reference to Utilities object
    static Utilities* getSingletonPtr();

 private:
    /// \brief reference to Utitlies object
    static Utilities* msSingleton;

    /// \brief default constructor
    Utilities() {};
    /// \brief virtual destructor
    virtual ~Utilities() {};

};

#endif
