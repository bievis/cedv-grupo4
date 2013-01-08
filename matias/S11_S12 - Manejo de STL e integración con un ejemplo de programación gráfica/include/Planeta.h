#ifndef PLANETA_H
#define PLANETA_H

#include "Astro.h"
#include "Satelite.h"
#include <vector>

class Planeta : public Astro {
 public:
  Planeta () : Astro() { };
  Planeta (const string id, const float posicion,
        const int colorRojo, const int colorVerde, const int colorAzul,
        const double radio, const int divisiones, const float translacion, const float rotacion);
  Planeta (const Planeta &p); // Constructor copia

  // Gets y Sets
  inline std::vector<Satelite>& getSatelites () const;
  
  // Sobre carga de Operadores
  Planeta& operator= (const Planeta &p);

 private:
  std::vector<Satelite> _satelites;
  
  void copiar(const Astro &a);
};

#endif
