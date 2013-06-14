#include "MiniMapTextureListener.h"

MiniMapTextureListener::MiniMapTextureListener ( std::vector<Character*> vCharacteres, Ogre::Rectangle2D* rect )
{
  _vCharacteres = vCharacteres;
  _rect = rect;
}

MiniMapTextureListener::~MiniMapTextureListener()
{
	_vCharacteres.clear();
}

void MiniMapTextureListener::preRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
	// Ocultamos el recuadro donde pintamos el mapa para que no se repita
	 _rect->setVisible(false);

	Character *character;
	for (unsigned int i = 0; i < _vCharacteres.size(); i++) {
		character = _vCharacteres[i];

		character->showDummy(true);
	}
  }

void MiniMapTextureListener::postRenderTargetUpdate ( const RenderTargetEvent& evt )
  {
	_rect->setVisible(true);

    Character *character;
	for (unsigned int i = 0; i < _vCharacteres.size(); i++) {
		character = _vCharacteres[i];

		character->showDummy(false);
	}
  }
