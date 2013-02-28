#include <ColisionManager.h>

template<> ColisionManager* Ogre::Singleton<ColisionManager>::msSingleton = 0;

ColisionManager::ColisionManager()
{
  
}

ColisionManager::~ColisionManager()
{
  _personaje = NULL;
  _partesEscenario.clear();
}

void ColisionManager::setPersonaje (Personaje* personaje) {
  _personaje = personaje;
}

void ColisionManager::addParteEscenario (ParteEscenario* parte) {
  _partesEscenario.push_back (parte);
}

bool ColisionManager::hayColision () {
  bool hay = false;

  if (_personaje != NULL) {
    Ogre::Entity *entPersonaje = NULL;
    Ogre::Entity *entParte = NULL;
    
    std::vector<ParteEscenario*>::const_iterator
      mit (_partesEscenario.begin()),
      mend(_partesEscenario.end());
    for(;mit!=mend;++mit) {
      (*mit)->toString();
    }
  }
  
  return hay;
}

// Gets y Sets
std::vector<ParteEscenario*>& ColisionManager::getPartesEscenario() {
  return _partesEscenario;
}

ColisionManager& ColisionManager::getSingleton ()
{
    assert(msSingleton);
    return (*msSingleton);
}
 
ColisionManager* ColisionManager::getSingletonPtr ()
{
    assert(msSingleton);
    return msSingleton;
}


