#ifndef SISTEMA_H
#define SISTEMA_H

#include "Astro.h"
#include <map>
#include <iostream>

using namespace std;

class Sistema {
 public:
  Sistema () { };
  Sistema (const string id);
  Sistema (const Sistema &s); // Constructor copia

  // Gets y Sets
  string getId() const;
  void setId(const string id);
  std::map<string, Astro>& getAstros();
  
  // Sobre carga de Operadores
  Sistema& operator= (const Sistema &s);

  string toString();
 private:
  string _id;
  std::map<string, Astro> _astros;
  
  void copiar(const Sistema &s);
};

#endif
