#ifndef MINIMAPTEXTURELISTENER_H
#define MINIMAPTEXTURELISTENER_H

#include <Ogre.h>
#include <vector>
#include "Character.h"

using namespace std;
using namespace Ogre;

/// \brief class to be used how listener in the texture for the conversion to black & white colours
/// This class performs a setMaterialName() in the preRender to black/white colours for the object
/// int the texture, also in the postRender reset Material to origin values. In the class, there is
/// a method which checks white/black colour levels to tell us if any object exists in the scene
class MiniMapTextureListener : public RenderTargetListener
{
  private:
    std::vector<Character*> _vCharacteres;
	Ogre::Rectangle2D* _rect;
	/// \brief reference to scene manager
    Ogre::SceneManager* _sceneMgr;
  public:
    /// \brief constructor for the texture listener
    /// \param sceneMgr reference to scene manager
    /// \param rtt texture pointer
    MiniMapTextureListener ( Ogre::SceneManager* sceneMgr, std::vector<Character*> vCharacteres, Ogre::Rectangle2D* rect );
    /// \brief default destructor
    ~MiniMapTextureListener();
    /// \brief this method is executed before the texture is put into the rectangle2D
    /// \param evt event
    virtual void preRenderTargetUpdate ( const RenderTargetEvent& evt );
    /// \brief this method is executed after the texture is put into the rectangle2D
    /// \param evt event
    virtual void postRenderTargetUpdate ( const RenderTargetEvent& evt );
};

#endif // MINIMAPTEXTURELISTENER_H
