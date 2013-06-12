#include "Enemy.h"

Enemy::Enemy( Ogre::SceneManager* sceneMgr,
                    OgreBulletDynamics::DynamicsWorld* world,
                    const string& name,
                    float initial_pos_X,
                    float initial_pos_Y,
                    float initial_pos_Z ) : Character ( sceneMgr,
                                                        world,
                                                        name,
                                                        initial_pos_X,
                                                        initial_pos_Y,
                                                        initial_pos_Z,
                                                        true )
  {
    //Material del enemigo
    _entity->setMaterialName ( "MaterialRojo" );

    _rtt = Ogre::TextureManager::getSingleton().createManual (
            "RttT_" + name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D, 64, 64, 0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET );

    _rtex = _rtt->getBuffer()->getRenderTarget();

    _camPOV = sceneMgr->createCamera ( "cameraPOV_" + name );
    _camPOV->setPosition ( Ogre::Vector3 ( 0, 0, 0.2 ) );
    _camPOV->lookAt ( Ogre::Vector3 ( 0, 0, 5 ) );
    _camPOV->setNearClipDistance ( 0.1 );
    _camPOV->setFOVy ( Ogre::Degree ( 38 ) );
	Ogre::SceneNode *nodeCamera = _node->createChildSceneNode("cameraPOV_" + name);
	nodeCamera->attachObject(_camPOV);

    _rtex->addViewport ( _camPOV );
    _rtex->getViewport(0)->setClearEveryFrame ( true );
    _rtex->getViewport(0)->setBackgroundColour ( Ogre::ColourValue::Black );
    _rtex->getViewport(0)->setOverlaysEnabled ( false );
    _rtex->setAutoUpdated(true);

    // Creamos la barra de energia
    _bbSetLife = _sceneMgr->createBillboardSet("lifeBarBillboardSet_" + name, 1);
    _bbSetLife->setMaterialName("lifeBar");
    _bbSetLife->setBillboardType(BBT_POINT);
    _lifeBar = _bbSetLife->createBillboard(Ogre::Vector3(0,
                                           _rigidBody->getCenterOfMassPosition().y +
                                                         (_entity->getBoundingBox().getSize().y / 2)
                                            , 0));
    _lifeBar->setDimensions(SIZE_LIFE_BAR, 0.2);
    _lifeBar->setTexcoordRect(0.0f, 0.0f, 0.50f, 1.0);
    _lifeNode = _node->createChildSceneNode("lifeBar_" + name);
    _lifeNode->attachObject(_bbSetLife);

    // Para poner al personaje principal en material blanco y poder verlo el enemigo
    _textureListener = new MyTextureListener ( _sceneMgr, _rtt );
    _rtex->addListener ( _textureListener );
  }

Enemy::~Enemy()
  {
    if ( _textureListener )
      {
        _rtex->removeListener ( _textureListener );
        delete _textureListener;
      }

    if ( _camPOV )
      _sceneMgr->destroyCamera ( _camPOV );

    // Destruimos el billboardset
    _lifeNode->detachObject(_bbSetLife);
    _sceneMgr->destroyBillboardSet(_bbSetLife);
    _lifeNode->getParent()->removeChild(_lifeNode);
    _sceneMgr->destroySceneNode(_lifeNode);
  }

Enemy::Enemy(const Enemy& other) : Character ( other )
  {
    copy ( other );
  }

Enemy& Enemy::operator=(const Enemy& rhs)
  {
    if (this == &rhs) return *this; // handle self assignment

    copy ( rhs );

    return *this;
  }

void Enemy::copy ( const Enemy& source )
  {
    _rtex = source.getRenderTexture();
	#ifndef _WIN32
		_rtt = source.getTexturePtr();
	#endif
    _camPOV = source.getCameraPOV();
  }

void Enemy::print()
{
  cout << "==============" << endl;
  cout << "Enemy Info" << endl;
  cout << "==============" << endl;

  Character::print();

  printf ( "render tex ref.: %p\n", _rtex );
  printf ( "texture ref.   : %p\n", _rtt.get() );
  printf ( "camera ref.    : %p\n", _camPOV );
  cout << "==============" << endl;
}

bool Enemy::haveYouSeenAnybody() const
{
  assert ( _textureListener );
  return _textureListener->enemyViewed();
}

void Enemy::updateLifeBar() {
    Ogre::Real ratio = _health / MAX_HEALTH;

    if (ratio < 0.0f)
        ratio = 0.0f;

    _lifeBar->setTexcoordRect((1.0 - ratio) / SIZE_LIFE_BAR,
                              0.0f,
                              0.50f + (1.0 - ratio) / SIZE_LIFE_BAR,
                              1.0);
}

void Enemy::update(double timeSinceLastFrame) {
    Character::update(timeSinceLastFrame);
    updateLifeBar();
}
