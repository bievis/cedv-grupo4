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
    std::map<float,int> mColors;

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

    std::map<float,int>::iterator it;

    for ( unsigned int i = 0; i < pixBox.getWidth(); i++ )
      {
        for ( unsigned int j = 0; j < pixBox.getHeight(); j++ )
          {
            Ogre::ColourValue color = img.getColourAt(i,j,0);

            float c = color.r + color.g + color.b + color.a;

            it = mColors.find ( c );

            if ( it != mColors.end() )
              it->second=it->second+1;
            else
              mColors.insert ( std::pair<float,int>(c,1) );
          }
      }

      // Blanco [R,G,B,A] = ( 1, 1, 1, 1 )
      // Negro [R,G,B,A] = ( 0, 0, 0, 1 )
      it = mColors.find ( 1.0 );

      m_enemyViewed = false;

      if ( it != mColors.end() )
        {
 //         cout << "it->second = " << it->second << " : NUM_PIXELS - THRESHOLD_VIEW = " << NUM_PIXELS - THRESHOLD_VIEW << endl;
          if ( it->second < ( NUM_PIXELS - THRESHOLD_VIEW ) )
            m_enemyViewed = true;
        }

//if ( mColors.size() > 1 )
//{
//    cout << "=============" << endl;
//    for (it=mColors.begin(); it!=mColors.end(); ++it)
//      std::cout << it->first << " => " << it->second << std::endl;
//    cout << "*************" << endl;
//}
    mColors.clear();
    // Volcamos a disco la captura, para ver si aparecen los objetos en blanco y el suelo ha desaparecido
//    static int cont = 1;
//    img.save ( "prueba" + Ogre::StringConverter::toString(cont++) + ".png" );
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
