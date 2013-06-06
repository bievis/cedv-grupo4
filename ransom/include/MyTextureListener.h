#ifndef MYTEXTURELISTENER_H
#define MYTEXTURELISTENER_H

#include <Ogre.h>

using namespace std;
using namespace Ogre;

/// \brief class to be used how listener in the texture for the conversion to black & white colours
/// This class performs a setMaterialName() in the preRender to black/white colours for the object
/// int the texture, also in the postRender reset Material to origin values. In the class, there is
/// a method which checks white/black colour levels to tell us if any object exists in the scene
class MyTextureListener : public RenderTargetListener
{
  private:
    /// \brief reference to scene manager
    Ogre::SceneManager* m_sceneMgr;
    /// \brief texture pointer
    Ogre::TexturePtr m_rtt;
    /// \brief boolean value to check if there is any object in the texture
    bool m_enemyViewed;

  public:
    /// \brief constructor for the texture listener
    /// \param sceneMgr reference to scene manager
    /// \param rtt texture pointer
    MyTextureListener ( Ogre::SceneManager* sceneMgr, const Ogre::TexturePtr& rtt );
    /// \brief default destructor
    ~MyTextureListener();
    /// \brief this method is executed before the texture is put into the rectangle2D
    /// \param evt event
    virtual void preRenderTargetUpdate ( const RenderTargetEvent& evt );
    /// \brief this method is executed after the texture is put into the rectangle2D
    /// \param evt event
    virtual void postRenderTargetUpdate ( const RenderTargetEvent& evt );
    /// \brief this method returned if there is any object in the texture
    /// \return true/false according to any object is in the texture
    inline bool enemyViewed() { return m_enemyViewed; };
};

#endif // MYTEXTURELISTENER_H
