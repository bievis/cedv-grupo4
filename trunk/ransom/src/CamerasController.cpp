#include "CamerasController.h"

CamerasController::CamerasController(Ogre::SceneManager* sceneMgr, Ogre::Camera* camera, 
									Ogre::Camera* cameraMiniMap, Hero* hero): _camera(camera), 
																			  _cameraMiniMap(cameraMiniMap),
																		      _hero(hero) {
	_cameraNode = sceneMgr->getRootSceneNode()->createChildSceneNode("Camera" + hero->getName());
	_cameraMiniMapNode = sceneMgr->getRootSceneNode()->createChildSceneNode("CameraMiniMap" + hero->getName());
    _targetNode = sceneMgr->getRootSceneNode()->createChildSceneNode("Camera_target");

    // Hacemos que las camaras siempre esten mirando al objetivo
    _cameraNode->setAutoTracking(true, _targetNode);
    _cameraNode->setFixedYawAxis(true);
	_cameraMiniMapNode->setAutoTracking(true, _targetNode);
    _cameraMiniMapNode->setFixedYawAxis(true);
	
	// Asginamos propiedades a las camaras y las atachamos a sus nodos
    _camera->setNearClipDistance(0.1);
    _camera->setFarClipDistance(100);
    _cameraNode->attachObject(_camera);

	_cameraMiniMap->setNearClipDistance(0.1);
    _cameraMiniMap->setFarClipDistance(100);
    _cameraMiniMapNode->attachObject(_cameraMiniMap);
} 

void CamerasController::update(double timeSinceLastFrame) {    
	Ogre::Vector3 posHero = _hero->getRigidBody()->getCenterOfMassPosition();
	posHero.y = 1.0;

	// Situamos las camaras
	Ogre::Vector3 cameraPosition = posHero + Ogre::Vector3::UNIT_Z * DISTANCE_Z_CAMERA
										   + Ogre::Vector3::UNIT_Y * HIGHT_CAMERA;
    Ogre::Vector3 _displacement = (cameraPosition - _cameraNode->getPosition()) * timeSinceLastFrame;
    _cameraNode->translate(_displacement);

	cameraPosition = posHero + Ogre::Vector3::UNIT_Z * DISTANCE_Z_CAMERA_MINIMAP
										   + Ogre::Vector3::UNIT_Y * HIGHT_CAMERA_MINIMAP;
    _displacement = (cameraPosition - _cameraMiniMapNode->getPosition()) * timeSinceLastFrame;
    _cameraMiniMapNode->translate(_displacement);

	// Situamos el objetivo
    _displacement = (posHero - _targetNode->getPosition()) * timeSinceLastFrame;
    _targetNode->translate(_displacement);
}

CamerasController::~CamerasController() {
	_cameraNode->detachAllObjects();
	_cameraMiniMapNode->detachAllObjects();
	_cameraNode->getCreator()->destroySceneNode(_cameraNode);
	_cameraMiniMapNode->getCreator()->destroySceneNode(_cameraNode);

	_camera = NULL;
	_cameraMiniMap = NULL;
    _hero = NULL;
	_cameraNode = NULL;
	_cameraMiniMapNode = NULL;
	_targetNode = NULL;
}