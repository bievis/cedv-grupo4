#include <GameManager.h>

template<> GameManager* Ogre::Singleton<GameManager>::msSingleton = 0;

GameManager::GameManager()
{
//  _tipoColisionAnterior = NINGUNA;
}

GameManager::~GameManager()
{
//  limpiar();
}

// void GameManager::setPersonaje (Personaje* personaje) {
//   _personaje = personaje;
// }

// void GameManager::addParteEscenario (ParteEscenario* parte) {
//   _partesEscenario.push_back (parte);
// }

// void GameManager::limpiar () {
//   _personaje = NULL;
//   _partesEscenario.clear();
//   _tipoColisionAnterior = NINGUNA;
// }

// void GameManager::limpiarPartes () {
//   ParteEscenario* parte = NULL;
//   std::vector<ParteEscenario*>::const_iterator
//       mit (_partesEscenario.begin()),
//       mend(_partesEscenario.end());
//   for(;mit!=mend;++mit) {
//     parte = (*mit);
//     parte->limpiarCarriles();
//   }
// }

// Colision GameManager::hayColision () {
//   TipoColision tipo = NINGUNA;
//   bool hay = false;
//   bool hayEnCarril = false;
//   double velocidad;
//   ParteEscenario* parte = NULL;
//   Carril* carril = NULL;
//   ElementoCarril* elemento = NULL;

//   // Miramos la colision cuando el personaje este parado
//   if (_personaje != NULL
//       && _personaje->getMovimiento() == NINGUNO
//       // No colisiones cuando este en el inicio
//       && getPersonaje()->getNodo()->getPosition() != getPersonaje()->getPosInicial()) {
//     Ogre::SceneNode *entPersonaje = _personaje->getNodo();
//     Ogre::SceneNode *ent = NULL;

//     std::vector<ParteEscenario*>::const_iterator
//       mit (_partesEscenario.begin()),
//       mend(_partesEscenario.end());
//     for(;mit!=mend && !hayEnCarril;++mit) {
//       parte = (*mit);
//       ent = parte->getNodo();
//       hayEnCarril = ent->_getWorldAABB ().intersects (entPersonaje->_getWorldAABB());
//       if (hayEnCarril) {
//         if (parte->getTipo() == AGUA) {
//           tipo = HUNDIDO;
//         }
//         hay = false;
//         std::vector<Carril*>::const_iterator
//           mit2 (parte->getCarriles().begin()),
//           mend2(parte->getCarriles().end());
//         for(;mit2!=mend2 && !hay;++mit2) {
//           carril = (*mit2);
//           std::vector<ElementoCarril*>::const_iterator
//             mit3 (carril->getElementos().begin()),
//             mend3(carril->getElementos().end());
//           for(;mit3!=mend3 && !hay;++mit3) {
//             elemento = (*mit3);
//             ent = elemento->getNodo();
//             hay = ent->_getWorldAABB ().intersects (entPersonaje->_getWorldAABB());
//             if (hay) {
//               if (parte->getTipo() == AGUA) {
//                 tipo = SOBRE;
//               } else if (parte->getTipo() == CARRETERA) {
//                 tipo = CHOQUE;
//               }
//               velocidad = carril->getVelocidad();
//             } else {
//               elemento = NULL;
//             }
//           }
//         }
//       }
//     }
//   }

//   return *(new Colision(tipo, elemento, velocidad));
// }

// SceneNode* GameManager::crearNodo (SceneManager*	m_pSceneMgr, const char* nombre, const char* mesh,
//             double posX, double posY, double posZ) {
//   Entity* ent;
//   SceneNode* node;

//   node = m_pSceneMgr->createSceneNode(nombre);
//   ent = m_pSceneMgr->createEntity(nombre, mesh);
//   node->attachObject(ent);
//   node->setPosition(posX, posY, posZ);
//   m_pSceneMgr->getRootSceneNode()->addChild(node);

//   return node;
// }

// Mueve los elementos de la escena
// deltaT, es el tiempo trascurrido desde el ultimo frame
// tiempo, es el tiempo total transcurrido en el juego
// void GameManager::mover(const double deltaT, const double tiempo) {
//   if (getPersonaje()->getEstado() != MUERTO && getPersonaje()->getEstado() != MUERTO_AHOGADO) {
//     // Movemos el resto de elementos
//     std::vector<ParteEscenario*>::const_iterator
//         mit (_partesEscenario.begin()),
//         mend(_partesEscenario.end());
//     for(;mit!=mend;++mit) {
//       (*mit)->mover(deltaT, tiempo);
//     }

//     // Miramos si colisionan
//     Colision colision = hayColision();
//     if (colision.getTipo() != NINGUNA) {
//       if (colision.getElementoColision() == NULL) {
//         // Si se ha hundido
//         if (colision.getTipo() == HUNDIDO) {
//           getPersonaje()->setEstado(MUERTO_AHOGADO);
//         }
//       } else {
//         if (colision.getTipo () == SOBRE
//             && _tipoColisionAnterior == SOBRE) {
//           // Si el personaje ha colisionado con algo con el que puede estar enciama
//           // Movemos el personaje con dicho elemento
//           getPersonaje()->moverConElemento(deltaT, colision.getElementoColision(),
//                                            colision.getVelocidad());
//         } else {
//           // Si se ha chocado
//           if (colision.getTipo() == CHOQUE) {
//             getPersonaje()->setEstado(MUERTO);
//           }
//         }
//       }
//     }
//     _tipoColisionAnterior = colision.getTipo();
//   }
//   // Movemos los personajes
//   getPersonaje()->mover(deltaT);
// }

// Gets y Sets
// std::vector<ParteEscenario*>& GameManager::getPartesEscenario() {
//   return _partesEscenario;
// }

// Personaje* GameManager::getPersonaje () {
//   return _personaje;
// }

// GameManager& GameManager::getSingleton ()
// {
//     assert(msSingleton);
//     return (*msSingleton);
// }

// GameManager* GameManager::getSingletonPtr ()
// {
//     assert(msSingleton);
//     return msSingleton;
// }