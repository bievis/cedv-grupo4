#include "CharacterControllerPlayer.hpp"

CharacterControllerPlayer::CharacterControllerPlayer(SceneManager * sceneMgr, Vector3 & origin)
{
	setupBody(sceneMgr, origin);
	setupAnimations();
	mMoving = false;
}

void CharacterControllerPlayer::addTime(Real deltaTime)
{
	updateAnimations(deltaTime);
}

void CharacterControllerPlayer::setupBody(SceneManager * sceneMgr, Vector3 & origin)
{
	mBodyNode = sceneMgr->getRootSceneNode()->createChildSceneNode(origin);
	mBodyEnt = sceneMgr->createEntity("SinbadNode", "Sinbad.mesh");
	mBodyNode->attachObject(mBodyEnt);
}

void CharacterControllerPlayer::setupAnimations()
{
	mBodyEnt->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);

	String animNames[] =
		{
			"IdleBase",
			"IdleTop",

			"RunBase",
			"RunTop",

			"HandsClosed",
			"HandsRelaxed",

			"DrawSwords",

			"SliceVertical",
			"SliceHorizontal",

			"Dance",

			"JumpStart",
			"JumpLoop",
			"JumpEnd"
		};

	for (int i = 0; i < NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEnt->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	setBaseAnimation(ANIM_IDLE_BASE);
	setTopAnimation(ANIM_IDLE_TOP);

	mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
}

void CharacterControllerPlayer::updateAnimations(Real deltaTime)
{
	Real baseAnimSpeed = 1;
	Real topAnimSpeed = 1;

	mTimer += deltaTime;

	if (mBaseAnimID == ANIM_JUMP_START && mTimer >= mAnims[mBaseAnimID]->getLength())
	{
		// takeoff animation finished, so time to leave the ground!
		setBaseAnimation(ANIM_JUMP_LOOP, true);
	}
	else if (mBaseAnimID == ANIM_JUMP_END)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			if (mMoving)
			{
				setBaseAnimation(ANIM_RUN_BASE, true);
				setTopAnimation(ANIM_RUN_TOP, true);
			}
			else
			{
				setBaseAnimation(ANIM_IDLE_BASE);
				setTopAnimation(ANIM_IDLE_TOP);
			}
		}
	}

	if (mBaseAnimID != ANIM_NONE)
		mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);

	if (mTopAnimID != ANIM_NONE)
		mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);

	fadeAnimations(deltaTime);
}

void CharacterControllerPlayer::fadeAnimations(Real deltaTime)
{
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		if (mFadingIn[i])
		{
			// slowly fade this animation in until it has full wieght
			Real newWeight = mAnims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));

			if (newWeight >= 1)
				mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			// slowly fade this animation out unit it has no weight, add the disable it
			Real newWeight = mAnims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));

			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

void CharacterControllerPlayer::setBaseAnimation(AnimID id, bool reset)
{
	if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mBaseAnimID] = false;
		mFadingOut[mBaseAnimID] = true;
	}

	mBaseAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;

		if (reset)
			mAnims[id]->setTimePosition(0);
	}
}

void CharacterControllerPlayer::setTopAnimation(AnimID id, bool reset)
{
	if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mTopAnimID] = false;
		mFadingOut[mTopAnimID] = true;
	}

	mTopAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;

		if (reset)
			mAnims[id]->setTimePosition(0);
	}
}

void CharacterControllerPlayer::animRunStart()
{
	setTopAnimation(ANIM_RUN_TOP);
	setBaseAnimation(ANIM_RUN_BASE);
}

void CharacterControllerPlayer::animRunEnd()
{
	setTopAnimation(ANIM_IDLE_TOP);
	setBaseAnimation(ANIM_IDLE_BASE, true);
}

void CharacterControllerPlayer::animJumpStart()
{
	setBaseAnimation(ANIM_JUMP_START, true);
	setTopAnimation(ANIM_NONE);
	mTimer = 0;
}

void CharacterControllerPlayer::animJumpLoop()
{
	setBaseAnimation(ANIM_JUMP_LOOP, true);
}

void CharacterControllerPlayer::animJumpEnd()
{
	setBaseAnimation(ANIM_JUMP_END, true);
	mTimer = 0;
}

SceneNode * CharacterControllerPlayer::getBodySceneNode()
{
	return mBodyNode;
}

Vector3 CharacterControllerPlayer::getPosition() const
{
	return mBodyNode->getPosition();
}

void CharacterControllerPlayer::setIsMoving(bool isMoving)
{
	mMoving = isMoving;
}