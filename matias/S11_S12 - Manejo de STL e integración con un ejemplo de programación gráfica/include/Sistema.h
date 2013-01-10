#ifndef SISTEMA_H
#define SISTEMA_H

#include "Astro.h"
#include "Planeta.h"
#include <map>
#include <iostream>

using namespace std;

class Sistema {
 public:
  Sistema () { };
  Sistema (const string id, const Astro astroCentral);
  Sistema (const Sistema &s); // Constructor copia

  // Gets y Sets
  string getId() const;
  void setId(const string id);
  Astro& getAstroCentral();
  void setAstroCentral(const Astro astroCentral);
  std::map<string, Planeta>& getPlanetas();
  
  // Sobre carga de Operadores
  Sistema& operator= (const Sistema &s);

  string toString();
  void pintar(long tiempo) const;
 private:
  string _id;
  Astro _astroCentral;
  std::map<string, Planeta> _planetas;
  
  void copiar(const Sistema &s);
};

#endif
