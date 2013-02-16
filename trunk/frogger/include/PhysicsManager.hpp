//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef PHYSICS_MANAGER_HPP
#define PHYSICS_MANAGER_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "MotionState.hpp"
#include "PhysicsDebug.hpp"

#include <btBulletDynamicsCommon.h>
#include <OgreSceneNode.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

class PhysicsManager {

	public:
		PhysicsManager();
		~PhysicsManager();

		void addStaticPlane(SceneNode * node);

		btRigidBody * addDynamicBox(SceneNode * node, float m = 1.0f);
		btRigidBody * addRigidBody(btTransform transform, btCollisionShape * shape, btScalar mass, SceneNode * node = NULL);

		void addCollisionShape(btCollisionShape * colShape);

		btDiscreteDynamicsWorld * getDynamicsWorld();
		btCollisionWorld * getCollisionWorld();
		btBroadphaseInterface * getBroadphase();

		void setRootSceneNode(SceneNode * node);
		btVector3 toBullet(const Vector3 & vec) const;
		void shootBox(const Vector3 & camPosition);

		static void debugBtVector3(const btVector3 & vec, const char * str = 0){
			std::cout << str << " x: " << vec.x() << "; y: " << vec.y() << "; z: " << vec.z() << std::endl;
		}

	private:
		btAlignedObjectArray<btCollisionShape*> 	mCollisionShapes;
		btBroadphaseInterface* 						mBroadphase;
		btCollisionDispatcher* 						mDispatcher;
		btConstraintSolver* 						mSolver;
		btDefaultCollisionConfiguration* 			mColConfig;
		btDiscreteDynamicsWorld* 					mWorld;
		SceneNode* 									mRootSceneNode;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||