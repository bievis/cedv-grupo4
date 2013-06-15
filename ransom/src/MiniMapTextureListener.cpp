#include "MiniMapTextureListener.h"
#include "GameState.hpp"

MiniMapTextureListener::MiniMapTextureListener ( Ogre::SceneManager* sceneMgr, std::vector<Character*> vCharacteres, Ogre::Rectangle2D* rect )
{
  _vCharacteres = vCharacteres;
  _rect = rect;
  _sceneMgr = sceneMgr;
}

MiniMapTextureListener::~MiniMapTextureListener()
{
	_vCharacteres.clear();
}

void MiniMapTextureListener::preRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
	// Ocultamos el recuadro donde pintamos el mapa para que no se repita
	_rect->setVisible(false);
	
	_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
	_sceneMgr->getEntity(NAME_MAP)->setCastShadows(false);

	Character *character;
	for (unsigned int i = 0; i < _vCharacteres.size(); i++) {
		character = _vCharacteres[i];

		character->showDummy(true);
		character->getEntity()->setCastShadows(false);
	}
  }

void MiniMapTextureListener::postRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
	_rect->setVisible(true);

	_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	_sceneMgr->getEntity(NAME_MAP)->setCastShadows(true);

    Character *character;
	for (unsigned int i = 0; i < _vCharacteres.size(); i++) {
		character = _vCharacteres[i];

		character->showDummy(false);
		character->getEntity()->setCastShadows(true);
	}
  }
