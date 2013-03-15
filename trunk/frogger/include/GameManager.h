#ifndef __GAME_MANAGERH__
#define __GAME_MANAGERH__

#include <vector>
#include "Personaje.h"
#include "ParteEscenario.h"
#include "Colision.h"

// Limites del escenario
#define LIMITE_DER 9
#define LIMITE_IZQ -9
#define LIMITE_ARRIBA -7
#define LIMITE_ABAJO 6.5

// Clase encargada de gestionar las colisiones
class GameManager: public Ogre::Singleton<GameManager> {
 public:
  GameManager();
  virtual ~GameManager();
  virtual void setPersonaje (Personaje* personaje);
  virtual void addParteEscenario (ParteEscenario* parte);
  virtual void mover(const double deltaT, const double tiempo);
  virtual Colision hayColision ();
  virtual void limpiar ();
  virtual SceneNode* crearNodo (SceneManager*	m_pSceneMgr, const char* nombre, const char* mesh,
            double posX, double posY, double posZ);
  
  // Gets y Sets
  virtual std::vector<ParteEscenario*>& getPartesEscenario();
  virtual Personaje* getPersonaje ();
  
  static GameManager& getSingleton ();
  static GameManager* getSingletonPtr ();
  
 private:
  Personaje* _personaje;
  std::vector<ParteEscenario*> _partesEscenario;
};

#endif
