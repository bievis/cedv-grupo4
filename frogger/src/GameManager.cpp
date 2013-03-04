#include <GameManager.h>

template<> GameManager* Ogre::Singleton<GameManager>::msSingleton = 0;

GameManager::GameManager()
{
  
}

GameManager::~GameManager()
{
  limpiar();
}

void GameManager::setPersonaje (Personaje* personaje) {
  _personaje = personaje;
}

void GameManager::addParteEscenario (ParteEscenario* parte) {
  _partesEscenario.push_back (parte);
}

void GameManager::limpiar () {
  _personaje = NULL;
  _partesEscenario.clear();
}

bool GameManager::hayColision () {
  bool hay = false;
  bool hayEnCarril = false;

  if (_personaje != NULL) {
    Ogre::SceneNode *entPersonaje = _personaje->getNodo();
    Ogre::SceneNode *ent = NULL;
    ParteEscenario* parte;
    Carril* carril;
    ElementoCarril* elemento;
    
    std::vector<ParteEscenario*>::const_iterator
      mit (_partesEscenario.begin()),
      mend(_partesEscenario.end());
    for(;mit!=mend && !hayEnCarril;++mit) {
      parte = (*mit);
      ent = parte->getNodo();
      hayEnCarril = ent->_getWorldAABB ().intersects (entPersonaje->_getWorldAABB());
      if (hayEnCarril) {
        hay = false;
        std::vector<Carril*>::const_iterator
          mit2 (parte->getCarriles().begin()),
          mend2(parte->getCarriles().end());
        for(;mit2!=mend2 && !hay;++mit2) {
          carril = (*mit2);
          std::vector<ElementoCarril*>::const_iterator
            mit3 (carril->getElementos().begin()),
            mend3(carril->getElementos().end());
          for(;mit3!=mend3 && !hay;++mit3) {
            elemento = (*mit3);
            ent = elemento->getNodo();
            hay = ent->_getWorldAABB ().intersects (entPersonaje->_getWorldAABB());
            // TODO BORRAR
            if (hay) {
              cout << "Ha colisionado el personaje con " << elemento->getNombre() << endl;
            }
          }
        }
      }
    }
  }
  
  return hay;
}

SceneNode* GameManager::crearNodo (SceneManager*	m_pSceneMgr, const char* nombre, const char* mesh,
            double posX, double posY, double posZ) {
  Entity* ent;
  SceneNode* node;  
  
  node = m_pSceneMgr->createSceneNode(nombre);
  ent = m_pSceneMgr->createEntity(nombre, mesh);
  node->attachObject(ent);
  node->setPosition(posX, posY, posZ);
  m_pSceneMgr->getRootSceneNode()->addChild(node);
  
  return node;
}

// Mueve los elementos de la escena
void GameManager::mover(const double deltaT, const double tiempo) {
  // Movemos los personajes
  getPersonaje()->mover(deltaT);
  // Movemos el resto de elementos
  std::vector<ParteEscenario*>::const_iterator
      mit (_partesEscenario.begin()),
      mend(_partesEscenario.end());
  for(;mit!=mend;++mit) {
    (*mit)->mover(deltaT, tiempo);
  }
  // Miramos si solisionan
  hayColision();
}

// Gets y Sets
std::vector<ParteEscenario*>& GameManager::getPartesEscenario() {
  return _partesEscenario;
}

Personaje* GameManager::getPersonaje () {
  return _personaje;
}

GameManager& GameManager::getSingleton ()
{
    assert(msSingleton);
    return (*msSingleton);
}
 
GameManager* GameManager::getSingletonPtr ()
{
    assert(msSingleton);
    return msSingleton;
}


