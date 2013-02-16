//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.hpp"

#include "DotSceneLoader.hpp"
#include "GlowMaterialListener.hpp"

#include "PhysicsManager.hpp"
#include "CharacterControllerManager.hpp"

#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

//|||||||||||||||||||||||||||||||||||||||||||||||

class GameState : public AppState
{
public:
	GameState();

	DECLARE_APPSTATE_CLASS(GameState)

	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();

	void moveCamera();
	void getInput();
    void buildGUI();
    void hideGUI();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void onLeftPressed(const OIS::MouseEvent &evt);
    void itemSelected(OgreBites::SelectMenu* menu);

	void update(double timeSinceLastFrame);
	
	void addPlatform(Vector3 position);

private:
    OgreBites::ParamsPanel*		m_pDetailsPanel;
	bool						m_bQuit;

	Ogre::Vector3				m_TranslateVector;
	Ogre::Real					m_MoveSpeed;
	Ogre::Degree				m_RotateSpeed;
	float						m_MoveScale;
	Ogre::Degree				m_RotScale;
	Ogre::Real					farDistance;

	bool						m_bCursor;
	bool						m_bLMouseDown;
	bool						m_bRMouseDown;
		
	PhysicsManager* 			mPhysics;
	DebugDrawer*				mDebugDrawer;
	CharacterControllerManager* mCharacter;
	
	bool mEnabledPhysicsDebugDraw;
	
	btRigidBody*				ball;
	btRigidBody*				cube1;
	btRigidBody*				cube2;
	btRigidBody*				cube3;	
	btRigidBody*				cube4;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||
