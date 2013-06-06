#ifndef MYTEXTURELISTENER_H
#define MYTEXTURELISTENER_H

#include <Ogre.h>

using namespace std;
using namespace Ogre;

class MyTextureListener : public RenderTargetListener
{
  private:
    Ogre::SceneManager* m_sceneMgr;
    Ogre::TexturePtr m_rtt;
    bool m_enemyViewed;
  public:
    MyTextureListener ( Ogre::SceneManager* sceneMgr, const Ogre::TexturePtr& rtt );
    ~MyTextureListener();
    virtual void preRenderTargetUpdate ( const RenderTargetEvent& evt );
    virtual void postRenderTargetUpdate ( const RenderTargetEvent& evt );

    inline bool enemyViewed() { return m_enemyViewed; };
};

#endif // MYTEXTURELISTENER_H
