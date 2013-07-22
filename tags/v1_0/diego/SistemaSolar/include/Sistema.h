#ifndef _SISTEMA_H_
#define _SISTEMA_H_

#include <map>
#include <list>
#include <iostream>
#include "Planeta.h"
#include "Astro.h"

using namespace std;

//El Sistema Solar mantiene una serie de planetas y el Sol (aunque es posible añadir nuevas estrellas).
class Sistema {

 public:

  map<string,Planeta> planetas;
  list<Astro> estrellas;
  
  Sistema() {};
  virtual ~Sistema();

  void print();

};

#endif  
