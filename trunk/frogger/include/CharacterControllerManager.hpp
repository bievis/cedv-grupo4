//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef CHARACTERCONTROLLER_MANAGER_HPP
#define CHARACTERCONTROLLER_MANAGER_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "CharacterControllerPhysics.hpp"
#include "CharacterControllerPlayer.hpp"

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

class CharacterControllerManager
{
	private:
		static const int RUN_SPEED = 17;
		static const int TURN_SPEED = 500;
		static const int CAM_HEIGHT = 2;

		CharacterControllerPhysics * mCCPhysics;
		CharacterControllerPlayer * mCCPlayer;

		SceneManager * mSceneManager;

		Camera * mCamera;
		SceneNode * mBodyNode;
		SceneNode * mCameraPivot;
		SceneNode * mCameraGoal;
		SceneNode * mCameraNode;
		Real mPivotPitch;
		Vector3 mWalkDirection;

		bool mIsFalling;
		bool mJumped;

		Vector3 mGoalDirection;
		Vector3 mKeyDirection;


	public:
		CharacterControllerManager(SceneManager * scnMgr, Camera * cam, btPairCachingGhostObject * ghostObject,
									btConvexShape * convexShape, btScalar stepHeight, btCollisionWorld * collisionWorld,
									Vector3 & origin, int upAxis = 1
									);

		CharacterControllerManager(SceneManager * scnMgr, CharacterControllerPlayer * ccPlayer,
									CharacterControllerPhysics * ccPhysics
									);

		CharacterControllerPlayer * getCCPlayer();
		CharacterControllerPhysics * getCCPhysics();
		void injectKeyDown(const OIS::KeyEvent & evt);
		void injectKeyUp(const OIS::KeyEvent & evt);
		void injectMouseMove(const OIS::MouseEvent & evt);
		void updateCharacter(Real dt);


	private:
		void setupCamera(Camera * cam);
		void updateCamera(Real deltaTime);
		void updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom);
		Quaternion updateOrientation(Real deltaTime);
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||