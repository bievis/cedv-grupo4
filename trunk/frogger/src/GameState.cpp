//|||||||||||||||||||||||||||||||||||||||||||||||

#include "GameState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

GameState::GameState()
{
    m_MoveSpeed			= 0.1f;
    m_RotateSpeed		= 0.3f;

    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;

    m_pDetailsPanel		= 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering GameState...");

    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pCamera->setPosition(Vector3(0, 30, 30));
    m_pCamera->lookAt(Vector3(0, 15, 0));
    m_pCamera->setNearClipDistance(5);

    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

    mPhysics = new PhysicsManager();
    mDebugDrawer = new DebugDrawer(m_pSceneMgr->getRootSceneNode(), mPhysics->getDynamicsWorld());
    mPhysics->getDynamicsWorld()->setDebugDrawer(mDebugDrawer);

    m_bCursor = false;

    mEnabledPhysicsDebugDraw = false;
    mDebugDrawer->setDebugMode(mEnabledPhysicsDebugDraw);

    // GLOW
/*    CompositorManager::getSingleton().addCompositor(m_pCamera->getViewport(), "Glow");
    CompositorManager::getSingleton().setCompositorEnabled(m_pCamera->getViewport(), "Glow", true);
    GlowMaterialListener *gml = new GlowMaterialListener();
    Ogre::MaterialManager::getSingleton().addListener(gml);
*/

    farDistance = -180.0f;
    
    buildGUI();

    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing GameState...");

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming GameState...");

    buildGUI();

    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_bQuit = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::exit()
{        
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving GameState...");
    
    
    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::createScene()
{

    m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);	
    m_pSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5) );
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));

    m_pSceneMgr->setShadowTextureCount(2);
    m_pSceneMgr->setShadowTextureSize(512);

    m_pSceneMgr->createLight("Light")->setPosition(75,75,75);

    m_pSceneMgr->setSkyBox(true, "Examples/Sky");


    // PLANE
    Plane plane(Ogre::Vector3(0,1,0), 0);
    MeshManager::getSingleton().createPlane("plane",
    ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
    2000, 2000, 1, 1, true, 1, 100, 100, Vector3::UNIT_Z);
    SceneNode* nodePlane = m_pSceneMgr->createSceneNode("ground");
    Entity* groundEnt = m_pSceneMgr->createEntity("planeEnt", "plane");
    groundEnt->setMaterialName("Ground");
    groundEnt->setCastShadows(false);
    nodePlane->attachObject(groundEnt);
    m_pSceneMgr->getRootSceneNode()->addChild(nodePlane); 
    mPhysics->addStaticPlane(nodePlane);

       // SPHERE
    SceneNode *ballNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(10,10,-150));
    Entity *ballEnt = m_pSceneMgr->createEntity("ball", "ball.mesh");
    ballEnt->setCastShadows(true);
    //ballEnt->setMaterialName("Shader/Glass");
    ballNode->attachObject(ballEnt);

    btTransform ballTrans = btTransform(btQuaternion(0,0,0,1),btVector3(10,10,-150));
    btCollisionShape* ballShape = new btSphereShape(1);
    btScalar ballMass = 10;

    ball = mPhysics->addRigidBody(ballTrans, ballShape, ballMass,ballNode);

    // Cube
    SceneNode *Node1 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(10,1,-150));
    Entity *Ent1 = m_pSceneMgr->createEntity("cube1", "Cube.mesh");
    //Ent1->setMaterialName("Shader/Velvet");
    Node1->scale(4,4,4);
    Node1->yaw(Degree(90));
    Ent1->setCastShadows(true);
    Node1->attachObject(Ent1);

    btTransform Trans1 = btTransform(btQuaternion(0,1,0,1),btVector3(10,1,-150));
    btCollisionShape* Shape1 = new btBoxShape(btVector3(3.273*4, 0.087*4, 2.496*4));
    btScalar Mass1 = 0;

    cube1 = mPhysics->addRigidBody(Trans1, Shape1, Mass1, Node1);

    // Cube
    SceneNode *Node2 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,1,-40));
    Entity *Ent2 = m_pSceneMgr->createEntity("cube2", "Cube.002.mesh");
    Node2->scale(4,4,4);
    Node2->yaw(Degree(90));
    Ent2->setCastShadows(true);
    Node2->attachObject(Ent2);

    btTransform Trans2 = btTransform(btQuaternion(0,1,0,1),btVector3(0,1,-40));
    btCollisionShape* Shape2 = new btBoxShape(btVector3(11.289*4, 0.087*4, 2.804*4));
    btScalar Mass2 = 0;

    cube2 = mPhysics->addRigidBody(Trans2, Shape2, Mass2, Node2);

    // Cube
    SceneNode *Node3 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,1,-125));
    Entity *Ent3 = m_pSceneMgr->createEntity("cube3", "Cube.001.mesh");
    Node3->scale(4,4,4);
    Node3->yaw(Degree(90));
    Ent3->setCastShadows(true);
    Node3->attachObject(Ent3);

    btTransform Trans3 = btTransform(btQuaternion(0,1,0,1),btVector3(0,1,-125));
    btCollisionShape* Shape3 = new btBoxShape(btVector3(1.999*4, 0.087*4, 1.3465*4));
    btScalar Mass3 = 0;

    cube3 = mPhysics->addRigidBody(Trans3, Shape3, Mass3, Node3);

    // Cube
    SceneNode *Node4 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(5,2,-100));
    Entity *Ent4 = m_pSceneMgr->createEntity("cube4", "Cube.003.mesh");
    Node4->scale(4,10,4);
    Ent4->setCastShadows(true);
    Node4->attachObject(Ent4);

    btTransform Trans4 = btTransform(btQuaternion(0,0,0,1),btVector3(5,2,-100));
    btCollisionShape* Shape4 = new btBoxShape(btVector3(5.295*4, 0.087*10, 2.1375*4));
    btScalar Mass4 = 0;

    cube4 = mPhysics->addRigidBody(Trans4, Shape4, Mass4, Node4);

    // CHARACTER CONTROLLER
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(0, 10, 0));
    Vector3 origin(0, 10, 0);

    btPairCachingGhostObject * characterGhostObject = new btPairCachingGhostObject();
    characterGhostObject->setWorldTransform(startTransform);

    btScalar characterHeight = 8.f;
    btScalar characterWidth = 1.f;

    btConvexShape * capsule = new btCapsuleShape(characterWidth, characterHeight);

    mPhysics->addCollisionShape(capsule);
    characterGhostObject->setCollisionShape(capsule);
    characterGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

    // CHARACTER DUCK
    btConvexShape * duck = new btCapsuleShape(characterWidth, characterHeight / 3);
    mPhysics->addCollisionShape(duck);

    btScalar stepHeight = 0.55f;
    mCharacter = new CharacterControllerManager(m_pSceneMgr, m_pCamera, characterGhostObject, capsule, stepHeight, mPhysics->getCollisionWorld(), origin);
    mCharacter->getCCPhysics()->setDuckingConvexShape(duck);

    mPhysics->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

    mPhysics->getDynamicsWorld()->addCollisionObject(characterGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
    mPhysics->getDynamicsWorld()->addAction(mCharacter->getCCPhysics());

    mPhysics->setRootSceneNode(m_pSceneMgr->getRootSceneNode());
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{   
    mCharacter->injectKeyDown(keyEventRef);


    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        pushAppState(findByName("PauseState"));
        return true;
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_I))
    {
        OgreBites::SelectMenu* pCamMode = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("ChatModeSelMenu");

        if(m_pDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            OgreFramework::getSingletonPtr()->m_pTrayMgr->moveWidgetToTray(m_pDetailsPanel, OgreBites::TL_TOPLEFT, 0);
            OgreFramework::getSingletonPtr()->m_pTrayMgr->moveWidgetToTray(pCamMode, OgreBites::TL_TOPRIGHT, 0);
            pCamMode->show();
            m_pDetailsPanel->show();
        }
        else
        {
            OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(m_pDetailsPanel);
            OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(pCamMode);
            pCamMode->hide();
            m_pDetailsPanel->hide();
        }
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_T)){
        mEnabledPhysicsDebugDraw = !mEnabledPhysicsDebugDraw;
        mDebugDrawer->setDebugMode(mEnabledPhysicsDebugDraw);
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_H))
    {
        OgreBites::SelectMenu* pInfo = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("InfoPanel");

        if(pInfo->getTrayLocation() == OgreBites::TL_NONE)
        {
            OgreFramework::getSingletonPtr()->m_pTrayMgr->moveWidgetToTray(pInfo, OgreBites::TL_RIGHT, 0);
            pInfo->show();
        }
        else
        {
            OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(pInfo);
            pInfo->hide();
        }
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_TAB)){
        if(m_bCursor)
            OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
        else
            OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();

        m_bCursor = !m_bCursor;
    }


    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_O))
        OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    mCharacter->injectKeyUp(keyEventRef);

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseMoved(const OIS::MouseEvent &evt)
{
    mCharacter->injectMouseMove(evt);

    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;

    if(id == OIS::MB_Left)
    {
        onLeftPressed(evt);
        m_bLMouseDown = true;
    }
    else if(id == OIS::MB_Right)
    {
        m_bRMouseDown = true;
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;

    if(id == OIS::MB_Left)
    {
        m_bLMouseDown = false;
    }
    else if(id == OIS::MB_Right)
    {
        m_bRMouseDown = false;
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::onLeftPressed(const OIS::MouseEvent &evt)
{
    mPhysics->shootBox(Convert::toOgre(mCharacter->getCCPhysics()->getPosition()));
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::moveCamera()
{
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
        m_pCamera->moveRelative(m_TranslateVector*10);

    m_pCamera->moveRelative(m_TranslateVector);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::getInput()
{
    if(!ball->isActive())
        ball->activate(true);

    btVector3 impulse = btVector3(0,0,0);
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_SPACE))
    {
      impulse += btVector3(0,10,0);
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_UP))
    {
      impulse += btVector3(0,0,-10);
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_DOWN))
    {
      impulse += btVector3(0,0,10);
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
    {
      impulse += btVector3(10,0,0);
    }

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LEFT))
    {
      impulse += btVector3(-10,0,0);
    }

    ball->applyCentralImpulse(impulse);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

    mPhysics->getDynamicsWorld()->stepSimulation(timeSinceLastFrame, 10);

    mDebugDrawer->step();

    mCharacter->updateCharacter(timeSinceLastFrame);

    if(m_bQuit == true)
    {
        popAppState();
        return;
    }

    if(!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible())
    {
        if(m_pDetailsPanel->isVisible())
        {
            m_pDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().x));
            m_pDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().y));
            m_pDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().z));
            m_pDetailsPanel->setParamValue(3, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().w));
            m_pDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().x));
            m_pDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().y));
            m_pDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().z));
        }
    }

    m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
    m_RotScale  = m_RotateSpeed * timeSinceLastFrame;

    m_TranslateVector = Vector3::ZERO;

    getInput();

    addPlatform(mCharacter->getCCPlayer()->getPosition());

}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::buildGUI()
{
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();

    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");

    m_pDetailsPanel = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "DetailsPanel", 200, items);
    m_pDetailsPanel->show();

    Ogre::String infoText = "Help\n\n[W] - Forward\n[S] - Backwards\n[A] - Left\n";
    infoText.append("[D] - Right\n\n[T] - Show Physics Debug\n[I] - Toggle Cam Position\n[O] - Toggle FPS / Logo\n[H] - Toggle Help\n");
    infoText.append("[Print] - Take screenshot\n\n[ESC] - Exit");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createTextBox(OgreBites::TL_RIGHT, "InfoPanel", infoText, 300, 280);
    hideGUI();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::hideGUI(){
    Ogre::StringVector chatModes;
    chatModes.push_back("Solid");
    chatModes.push_back("Wireframe");
    chatModes.push_back("Point");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "ChatModeSelMenu", "Camera Mode", 200, 3, chatModes);

    OgreBites::SelectMenu* pCamMode = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("ChatModeSelMenu");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(m_pDetailsPanel);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(pCamMode);
    pCamMode->hide();
    m_pDetailsPanel->hide();

    OgreBites::SelectMenu* pInfo = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("InfoPanel");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(pInfo);
    pInfo->hide();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->hideLogo();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->hideFrameStats();

}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::itemSelected(OgreBites::SelectMenu* menu)
{
    switch(menu->getSelectionIndex())
    {
    case 0:
        m_pCamera->setPolygonMode(Ogre::PM_SOLID);break;
    case 1:
        m_pCamera->setPolygonMode(Ogre::PM_WIREFRAME);break;
    case 2:
        m_pCamera->setPolygonMode(Ogre::PM_POINTS);break;
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::addPlatform(Vector3 position){
    //std::cout<<" "<<position.z<<std::cin;
    
    if(position.z < farDistance) {

        std::ostringstream s;
        s << position.z;
        SceneNode *ballNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(10,10,farDistance));
        Entity *ballEnt = m_pSceneMgr->createEntity("ball"+s.str(), "ball.mesh");
        ballEnt->setCastShadows(true);
        //ballEnt->setMaterialName("Shader/Glass");
        ballNode->attachObject(ballEnt);
    
        btTransform ballTrans = btTransform(btQuaternion(0,0,0,1),btVector3(10,10,farDistance));
        btCollisionShape* ballShape = new btSphereShape(1);
        btScalar ballMass = 10;
        ball = mPhysics->addRigidBody(ballTrans, ballShape, ballMass,ballNode);
        
        // Cube
        SceneNode *Node1 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(10,1,farDistance-150));
        Entity *Ent1 = m_pSceneMgr->createEntity("cube1"+s.str(), "Cube.mesh");
        //Ent1->setMaterialName("Shader/Velvet");
        Node1->scale(4,4,4);
        Node1->yaw(Degree(90));
        Ent1->setCastShadows(true);
        Node1->attachObject(Ent1);
        
        btTransform Trans1 = btTransform(btQuaternion(0,1,0,1),btVector3(10,1,farDistance-150));
        btCollisionShape* Shape1 = new btBoxShape(btVector3(3.273*4, 0.087*4, 2.496*4));
        btScalar Mass1 = 0;
        
        cube1 = mPhysics->addRigidBody(Trans1, Shape1, Mass1, Node1);
        
        // Cube
        SceneNode *Node2 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,1,farDistance-40));
        Entity *Ent2 = m_pSceneMgr->createEntity("cube2"+s.str(), "Cube.002.mesh");
        Node2->scale(4,4,4);
        Node2->yaw(Degree(90));
        Ent2->setCastShadows(true);
        Node2->attachObject(Ent2);
        
        btTransform Trans2 = btTransform(btQuaternion(0,1,0,1),btVector3(0,1,farDistance-40));
        btCollisionShape* Shape2 = new btBoxShape(btVector3(11.289*4, 0.087*4, 2.804*4));
        btScalar Mass2 = 0;
        
        cube2 = mPhysics->addRigidBody(Trans2, Shape2, Mass2, Node2);
        
        // Cube
        SceneNode *Node3 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(0,1,farDistance-125));
        Entity *Ent3 = m_pSceneMgr->createEntity("cube3"+s.str(), "Cube.001.mesh");
        Node3->scale(4,4,4);
        Node3->yaw(Degree(90));
        Ent3->setCastShadows(true);
        Node3->attachObject(Ent3);
        
        btTransform Trans3 = btTransform(btQuaternion(0,1,0,1),btVector3(0,1,farDistance-125));
        btCollisionShape* Shape3 = new btBoxShape(btVector3(1.999*4, 0.087*4, 1.3465*4));
        btScalar Mass3 = 0;
        
        cube3 = mPhysics->addRigidBody(Trans3, Shape3, Mass3, Node3);
        
        // Cube
        SceneNode *Node4 = m_pSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(5,2,farDistance-100));
        Entity *Ent4 = m_pSceneMgr->createEntity("cube4"+s.str(), "Cube.003.mesh");
        Node4->scale(4,10,4);
        Ent4->setCastShadows(true);
        Node4->attachObject(Ent4);
        
        btTransform Trans4 = btTransform(btQuaternion(0,0,0,1),btVector3(5,2,farDistance-100));
        btCollisionShape* Shape4 = new btBoxShape(btVector3(5.295*4, 0.087*10, 2.1375*4));
        btScalar Mass4 = 0;
        
        cube4 = mPhysics->addRigidBody(Trans4, Shape4, Mass4, Node4);

        farDistance += -180.f;

    }
}

