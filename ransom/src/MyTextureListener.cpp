#include "MyTextureListener.h"

#define MAX_ENEMIES 20

// 64 x 64 = 4096 pixeles en la textura
#define NUM_PIXELS 4096
// si al menos hay 100 pixeles blancos significa que vemos algo
#define THRESHOLD_VIEW 100

MyTextureListener::MyTextureListener ( Ogre::SceneManager* sceneMgr, const TexturePtr& rtt )
{
  m_sceneMgr = sceneMgr;
  m_rtt = rtt;
}

MyTextureListener::~MyTextureListener()
{
}

void MyTextureListener::preRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
    Ogre::Image img;
    Ogre::PixelBox pixBox;
    string nameEntity = "";
    Ogre::Entity* ptrEntity = NULL;
    unsigned int cont = 0;

    // Ponemos material blanco a todos los enemigos

    for ( unsigned int i = 0; i < MAX_ENEMIES; i++ )
      {
        nameEntity = "Enemy" + Ogre::StringConverter::toString(i);
        if ( m_sceneMgr->hasEntity ( nameEntity ) )
          {
            ptrEntity = m_sceneMgr->getEntity ( nameEntity );
            ptrEntity->setMaterialName ( "MaterialBlanco" );
            ptrEntity->setCastShadows ( false );
          }
      }

    // Ocultamos el suelo

    nameEntity = "floor";
    if ( m_sceneMgr->hasEntity ( nameEntity ) )
      {
        ptrEntity = m_sceneMgr->getEntity ( nameEntity );
        ptrEntity->setVisible ( false );
      }

    // Cogemos la imagen del momento

    m_rtt->convertToImage ( img );

    pixBox = img.getPixelBox();

    for ( unsigned int i = 0; i < pixBox.getWidth(); i++ )
      {
        for ( unsigned int j = 0; j < pixBox.getHeight(); j++ )
          {
            Ogre::ColourValue color = img.getColourAt(i,j,0);

            float c = color.r + color.g + color.b + color.a;

            if ( c == 4.0 ) // Blanco ( 1, 1, 1, 1 ) [ r, g, b, a ]
              cont++;
          }
      }

    m_enemyViewed = false;

    if ( cont >= THRESHOLD_VIEW )
      m_enemyViewed = true;

  }

void MyTextureListener::postRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
    string nameEntity = "";
    Ogre::Entity* ptrEntity = NULL;
    // Restablecemos a material rojo a todos los enemigos

    for ( unsigned int i = 0; i < MAX_ENEMIES; i++ )
      {
        nameEntity = "Enemy" + Ogre::StringConverter::toString(i);
        if ( m_sceneMgr->hasEntity ( nameEntity ) )
          {
            ptrEntity = m_sceneMgr->getEntity ( nameEntity );
            ptrEntity->setMaterialName("MaterialRojo");
            ptrEntity->setCastShadows ( true );
          }
      }

    // Restablecemos a visible el suelo

    nameEntity = "floor";
    if ( m_sceneMgr->hasEntity ( nameEntity ) )
      {
        ptrEntity = m_sceneMgr->getEntity ( nameEntity );
        ptrEntity->setVisible ( true );
      }

    // ---------------------------------------------
  }
