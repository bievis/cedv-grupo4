#include "MyTextureListener.h"

//#define MAX_ENEMIES 20

// si al menos hay 100 pixeles blancos significa que vemos algo
#define THRESHOLD_VIEW 100

MyTextureListener::MyTextureListener ( Ogre::SceneManager* sceneMgr, const TexturePtr& rtt )
{
  _sceneMgr = sceneMgr;
  _rtt = rtt;
  _materialName = "";
  _enemyViewed = false;
}

MyTextureListener::~MyTextureListener()
{
}

void MyTextureListener::preRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
    Ogre::Image img;
    Ogre::PixelBox pixBox;
    string nameEntityDummy = "HeroDUMMY";
    string nameEntity = "Hero";
    Ogre::Entity* ptrEntity = NULL;
    unsigned int cont = 0;

    // Ponemos material blanco a todos los enemigos

    if ( _sceneMgr->hasEntity ( nameEntity ) && _sceneMgr->hasEntity ( nameEntityDummy ) )
      {
        ptrEntity = _sceneMgr->getEntity ( nameEntity );
        ptrEntity->setVisible ( false );
        ptrEntity = _sceneMgr->getEntity ( nameEntityDummy );
        ptrEntity->setVisible ( true );
      }

    // Ocultamos el suelo

    nameEntity = "floor";
    if ( _sceneMgr->hasEntity ( nameEntity ) )
      {
        ptrEntity = _sceneMgr->getEntity ( nameEntity );
        ptrEntity->setVisible ( false );
      }

    // Cogemos la imagen del momento

    _rtt->convertToImage ( img );

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

    _enemyViewed = false;

    if ( cont >= THRESHOLD_VIEW )
      _enemyViewed = true;

  }

void MyTextureListener::postRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
    string nameEntity = "Hero";
    string nameEntityDummy = "HeroDUMMY";
    Ogre::Entity* ptrEntity = NULL;

    // Restablecemos a material rojo a todos los enemigos

    if ( _sceneMgr->hasEntity ( nameEntity ) && _sceneMgr->hasEntity ( nameEntityDummy ) )
      {
        ptrEntity = _sceneMgr->getEntity ( nameEntity );
        ptrEntity->setVisible ( true );
        ptrEntity = _sceneMgr->getEntity ( nameEntityDummy );
        ptrEntity->setVisible ( false );
      }

    // Restablecemos a visible el suelo

    nameEntity = "floor";
    if ( _sceneMgr->hasEntity ( nameEntity ) )
      {
        ptrEntity = _sceneMgr->getEntity ( nameEntity );
        ptrEntity->setVisible ( true );
      }
  }
