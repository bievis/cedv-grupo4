#ifndef MINIMAPTEXTURELISTENER_H
#define MINIMAPTEXTURELISTENER_H

#include <Ogre.h>
#include <vector>
#include "Character.h"

using namespace std;
using namespace Ogre;

/// \brief class to be used how listener in the texture for the mini map
/// This class performs a change between real objects (enemyes, heroes, hostages) and dummy objects (cubes with colors)
class MiniMapTextureListener : public RenderTargetListener
{
  private:
    std::vector<Character*> _vCharacteres;
	/// \brief reference to scene manager
    Ogre::SceneManager* _sceneMgr;
  public:
    /// \brief constructor for the texture listener
    /// \param sceneMgr reference to scene manager
    /// \param rtt texture pointer
    MiniMapTextureListener ( Ogre::SceneManager* sceneMgr, std::vector<Character*> vCharacteres);
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
