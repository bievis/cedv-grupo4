#ifndef CAMERAS_CONTROLLER_H
#define CAMERAS_CONTROLLER_H

#include <OGRE/Ogre.h>
#include "Hero.h"

#define HIGHT_CAMERA 25.0f
#define HIGHT_CAMERA_MINIMAP 50.0f
#define DISTANCE_Z_CAMERA 5.0f
#define DISTANCE_Z_CAMERA_MINIMAP 1.0f

class CamerasController {
    public:
        CamerasController(Ogre::SceneManager* sceneMgr, Ogre::Camera* camera, Ogre::Camera* cameraMiniMap, Hero* hero);
        void update(double timeSinceLastFrame);
		~CamerasController();
    private:
        // Referencias
        Ogre::Camera* _camera;
		Ogre::Camera* _cameraMiniMap;
        Hero* _hero;
		Ogre::SceneNode* _cameraNode;
		Ogre::SceneNode* _cameraMiniMapNode;
		Ogre::SceneNode* _targetNode;
};

#endif  // CAMERAS_CONTROLLER_H
