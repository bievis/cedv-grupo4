#ifndef __COLISION_MANAGERH__
#define __COLISION_MANAGERH__

#include <OGRE/Ogre.h>
#include <vector>
#include "Personaje.h"
#include "ParteEscenario.h"

// Clase encargada de gestionar las colisiones
class ColisionManager: public Ogre::Singleton<ColisionManager> {
 public:
  ColisionManager();
  virtual ~ColisionManager();
  virtual void setPersonaje (Personaje* personaje);
  virtual void addParteEscenario (ParteEscenario* parte);
  virtual bool hayColision ();
  // Gets y Sets
  virtual std::vector<ParteEscenario*>& getPartesEscenario();
  
  static ColisionManager& getSingleton ();
  static ColisionManager* getSingletonPtr ();
  
 private:
  Personaje* _personaje;
  std::vector<ParteEscenario*> _partesEscenario;
};

#endif
