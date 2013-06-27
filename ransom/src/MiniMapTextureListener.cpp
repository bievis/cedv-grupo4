#include "MiniMapTextureListener.h"
#include "GameState.hpp"

MiniMapTextureListener::MiniMapTextureListener ( Ogre::SceneManager* sceneMgr, std::vector<Character*> vCharacteres)
{
  _vCharacteres = vCharacteres;
  _sceneMgr = sceneMgr;
}

MiniMapTextureListener::~MiniMapTextureListener()
{
	_vCharacteres.clear();
}

void MiniMapTextureListener::preRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
	_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);

	Character *character;
	for (unsigned int i = 0; i < _vCharacteres.size(); i++) {
		character = _vCharacteres[i];

		if (character->isVisible()) {
			character->showDummy(true);
			character->getEntity()->setCastShadows(false);
		}
	}
  }

void MiniMapTextureListener::postRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
	_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    Character *character;
	for (unsigned int i = 0; i < _vCharacteres.size(); i++) {
		character = _vCharacteres[i];

		if (character->isVisible()) {
			character->showDummy(false);
			character->getEntity()->setCastShadows(true);
		}
	}
  }
