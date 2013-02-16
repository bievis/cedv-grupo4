#include "MotionState.hpp"

MotionState::MotionState(const btTransform &initialPos, Ogre::SceneNode *node){
	mVisibleObj = node;
	mTransform = initialPos;
	mCOM = btTransform::getIdentity();
}

MotionState::MotionState(const btTransform &initialPos){
	mVisibleObj = NULL;
	mTransform = initialPos;
}

MotionState::~MotionState(){
	
}

void MotionState::setNode(Ogre::SceneNode *node) {
	mVisibleObj = node;
}

btTransform MotionState::getWorldTransform() const {
	return mCOM.inverse() * mTransform;
}

void MotionState::getWorldTransform(btTransform &worldTrans) const {
	worldTrans = mCOM.inverse() * mTransform;
}

void MotionState::setWorldTransform(const btTransform & worldTrans){
	if (mVisibleObj == NULL)	return;
		
	mTransform = worldTrans;
	btTransform transform = mTransform * mCOM;
	btQuaternion rot = transform.getRotation();
	btVector3 pos = transform.getOrigin();
	mVisibleObj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	mVisibleObj->setPosition(pos.x(), pos.y(), pos.z());
	
}
