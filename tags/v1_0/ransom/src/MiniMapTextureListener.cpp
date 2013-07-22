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
  _sceneMgr = NULL;
}

void MiniMapTextureListener::preRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
	// Desactivamos las sombras en el Minimapa
	_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);

	// Recorremos todos los personajes y los mostramos como cubos en el mapa
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
	// Activamos las sombras
	_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// Recorremos todos los personajes y los mostramos con su entidad original
    Character *character;
	for (unsigned int i = 0; i < _vCharacteres.size(); i++) {
		character = _vCharacteres[i];

		if (character->isVisible()) {
			character->showDummy(false);
			character->getEntity()->setCastShadows(true);
		}
	}
  }
