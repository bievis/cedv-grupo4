#ifndef SATELITE_H
#define SATELITE_H

#include "Planeta.h"

class Satelite : public Astro {
 public:
  Satelite () : Astro() { };
  Satelite (const string id, const float posicion, 
        const int colorRojo, const int colorVerde, const int colorAzul,
        const double radio, const int divisiones, const float translacion, const float rotacion,
        const Planeta &planeta);
  Satelite (const Satelite &s); // Constructor copia

  // Gets y Sets
  inline Planeta getPlaneta () const;
  inline void setPlaneta(const Planeta planeta);
  
  // Sobre carga de Operadores
  Satelite& operator= (const Satelite &s);
  bool operator == (const Satelite &s) const;

 private:
  Planeta _planeta;
  
  void copiar(const Astro &s);
};

#endif
