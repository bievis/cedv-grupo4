#include "Character.h"
#include <cassert>

// Mesh file to load how to a Character
#define MESH_FILE_WITHOUT_EXTENSION "Cube"

Character::Character ( Ogre::SceneManager* sceneMgr,
                        OgreBulletDynamics::DynamicsWorld* world,
                        const string& name,
                        float initial_pos_X,
                        float initial_pos_Y,
                        float initial_pos_Z,
                        eColor color )
{
  m_mutex_move = PTHREAD_MUTEX_INITIALIZER;
  m_mutex_turn = PTHREAD_MUTEX_INITIALIZER;
  m_mutex_walk = PTHREAD_MUTEX_INITIALIZER;

  m_node = NULL;
  m_rigidBody = NULL;
//  m_rtex = NULL;
//  m_camPOV = NULL;

  m_sceneMgr = sceneMgr;

  m_name = name;
  m_health = 100.0;
//  m_node = Utilities::getSingleton().put_element_in_scene ( sceneMgr,
//                                                           world,
//                                                           MESH_FILE_WITHOUT_EXTENSION,
//                                                           name,
//                                                           initial_pos_X,
//                                                           initial_pos_Y,
//                                                           initial_pos_Z );
  Utilities::getSingleton().put_cube_in_scene ( sceneMgr,
                                                           world,
                                                           MESH_FILE_WITHOUT_EXTENSION,
                                                           name,
                                                           initial_pos_X,
                                                           initial_pos_Y,
                                                           initial_pos_Z,
                                                           color,
                                                           &m_entity,
                                                           &m_node,
                                                           &m_rigidBody );

  assert ( m_node );
  assert ( m_rigidBody );

  m_posX = initial_pos_X;
  m_posY = initial_pos_Y;
  m_posZ = initial_pos_Z;

//  m_rtt = Ogre::TextureManager::getSingleton().createManual (
//            "RttT_" + name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
//            Ogre::TEX_TYPE_2D, 64, 64, 0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET );
//
//  m_rtex = m_rtt->getBuffer()->getRenderTarget();
//
//  m_camPOV = sceneMgr->createCamera ( "cameraPOV_" + name );
//  m_camPOV->setPosition ( Ogre::Vector3 ( 0, 2, -4 ) );
//  m_camPOV->lookAt ( Ogre::Vector3 ( 0, 2, -14 ) );
//  m_camPOV->setNearClipDistance ( 5 );
//  m_camPOV->setFOVy ( Ogre::Degree ( 38 ) );
//
//  m_rtex->addViewport ( m_camPOV );
//  m_rtex->getViewport(0)->setClearEveryFrame ( true );
//  m_rtex->getViewport(0)->setBackgroundColour ( Ogre::ColourValue::Black );
//  m_rtex->getViewport(0)->setOverlaysEnabled ( false );
//  m_rtex->setAutoUpdated(true);

}

Character::~Character()
{
  //dtor
}

Character::Character ( const Character& other )
{
  copy ( other );
}

Character& Character::operator=(const Character& rhs)
{
  if (this == &rhs) return *this; // handle self assignment

  copy ( rhs );

  //assignment operator
  return *this;
}

void Character::copy ( const Character& source )
{
  m_name = source.getName();
  m_health = source.getHealth();
  m_posX = source.getInitial_PosX();
  m_posY = source.getInitial_PosY();
  m_posZ = source.getInitial_PosZ();
  m_node = source.getSceneNode();
  m_rigidBody = source.getRigidBody();
  //m_rtex = source.getRenderTexture();
  //m_rtt = source.getTexturePtr();
  //m_camPOV = source.getCameraPOV();
}

void Character::setHealth ( float newHealth )
{
  if ( m_health < 100 )
  {
    if ( newHealth > 100 )
      m_health = 100;
    else
      m_health = newHealth;
  }
}

//void Character::walk()
//{
//  move ( )
//}

void Character::walk ( bool reverse )
  {
    pthread_mutex_lock( &m_mutex_walk );

    assert ( m_rigidBody );

    m_rigidBody->applyForce ( Ogre::Vector3 ( 0, 100, 0 ), Ogre::Vector3::ZERO );

    pthread_mutex_unlock( &m_mutex_walk );
  }

void Character::move_to ( float posX, float posY, float posZ )
  {
    pthread_mutex_lock( &m_mutex_move );

    assert ( m_node );

    m_node->translate ( posX, posY, posZ );

    pthread_mutex_unlock( &m_mutex_move );
  }

void Character::turn ( Ogre::Real angle )
  {
    pthread_mutex_lock( &m_mutex_turn );

    assert ( m_node );

    m_node->yaw ( Ogre::Radian(angle) );

    pthread_mutex_unlock( &m_mutex_turn );
  }

void Character::print()
{
  cout << "==============" << endl;
  cout << "Character Info" << endl;
  cout << "==============" << endl;
  cout << "name           : " << m_name << endl;
  cout << "health         : " << m_health << endl;
  printf ( "node ref.      : %p\n", m_node );
  printf ( "body ref.      : %p\n", m_rigidBody );
//  printf ( "render tex ref.: %p\n", m_rtex );
//  printf ( "texture ref.   : %p\n", m_rtt.get() );
//  printf ( "camera ref.    : %p\n", m_camPOV );
  cout << "initial position" << endl;
  cout << "X              : " << m_posX << endl;
  cout << "Y              : " << m_posY << endl;
  cout << "Z              : " << m_posZ << endl;
  cout << "==============" << endl;
}

//bool Character::check_vision()
//  {
//    Ogre::Image img;
//    Ogre::PixelBox pixBox;
//    string nameEntity = "";
//    Ogre::Entity* ptrEntity = NULL;
//
//    Ogre::TexturePtr rtt;
//    Ogre::RenderTexture* rtex = NULL;
//    Ogre::Camera* camPOV = NULL;
//
//    // Ponemos material blanco a todos los enemigos
//
//    for ( unsigned int i = 0; i < 20; i++ )
//      {
//        nameEntity = "Enemy" + Ogre::StringConverter::toString(i);
//        if ( m_sceneMgr->hasEntity ( nameEntity ) )
//          {
//            ptrEntity = m_sceneMgr->getEntity ( nameEntity );
//            ptrEntity->setMaterialName ( "MaterialBlanco" );
//            ptrEntity->setCastShadows ( false );
//          }
//      }
//
//    // Ocultamos el suelo
//
//    nameEntity = "floor";
//    if ( m_sceneMgr->hasEntity ( nameEntity ) )
//      {
//        ptrEntity = m_sceneMgr->getEntity ( nameEntity );
//        ptrEntity->setVisible ( false );
//      }
//
//    // Realizamos el render a textura
//
//    rtt = Ogre::TextureManager::getSingleton().createManual (
//            "RttT_" + m_name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
//            Ogre::TEX_TYPE_2D, 64, 64, 0, Ogre::PF_A8R8G8B8, Ogre::TU_RENDERTARGET );
//
//    rtex = rtt->getBuffer()->getRenderTarget();
//
//    camPOV = m_sceneMgr->createCamera ( "cameraPOV_" + m_name );
//    camPOV->setPosition ( Ogre::Vector3 ( 0, 2, -4 ) );
//    camPOV->lookAt ( Ogre::Vector3 ( 0, 2, -14 ) );
//    camPOV->setNearClipDistance ( 5 );
//    camPOV->setFOVy ( Ogre::Degree ( 38 ) );
//
//    rtex->addViewport ( camPOV );
//    rtex->getViewport(0)->setClearEveryFrame ( true );
//    rtex->getViewport(0)->setBackgroundColour ( Ogre::ColourValue::Black );
//    rtex->getViewport(0)->setOverlaysEnabled ( false );
//    rtex->setAutoUpdated(true);
//
//    // Cogemos la imagen del momento
//
//    rtt->convertToImage ( img );
//
////    pixBox = img.getPixelBox();
//
////    for ( unsigned int i = 0; i < pixBox.getWidth(); i++ )
////      {
////        for ( unsigned int j = 0; j < pixBox.getHeight(); j++ )
////          {
////            printf ( "%02x ", img.getColourAt(i,j,0) );
////          }
////          printf ( "\n" );
////      }
//
//    // Volcamos a disco la captura, para ver si aparecen los objetos en blanco y el suelo ha desaparecido
//    static int cont = 1;
//    img.save ( "prueba" + Ogre::StringConverter::toString(cont++) + ".png" );
//
//    // Restablecemos a material rojo a todos los enemigos
//
//    for ( unsigned int i = 0; i < 20; i++ )
//      {
//        nameEntity = "Enemy" + Ogre::StringConverter::toString(i);
//        if ( m_sceneMgr->hasEntity ( nameEntity ) )
//          {
//            ptrEntity = m_sceneMgr->getEntity ( nameEntity );
//            ptrEntity->setMaterialName("MaterialRojo");
//            ptrEntity->setCastShadows ( true );
//          }
//      }
//
//    // Ponemos material negro al suelo
//
//    nameEntity = "floor";
//    if ( m_sceneMgr->hasEntity ( nameEntity ) )
//      {
//        ptrEntity = m_sceneMgr->getEntity ( nameEntity );
//        ptrEntity->setVisible ( true );
//      }
//
//    // ---------------------------------------------
//
//    //delete rtex;
//    m_sceneMgr->destroyCamera ( camPOV );
//
//}
