//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef MOTION_STATE_HPP
#define MOTION_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

class MotionState : public btDefaultMotionState
{
	protected:
		SceneNode * mVisibleObj;
		btTransform mTransform;
		btTransform mCOM;

	public:
		MotionState(const btTransform & initialPos, SceneNode * node);
		MotionState(const btTransform & initialPos);
		~MotionState();

		void setNode(SceneNode * node);
		btTransform getWorldTransform() const;

		void getWorldTransform(btTransform & worldTrans) const;

		void setWorldTransform(const btTransform & worldTrans);
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||