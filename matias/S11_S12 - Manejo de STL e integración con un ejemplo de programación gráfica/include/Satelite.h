#ifndef SATELITE_H
#define SATELITE_H

#include "Astro.h"

class Satelite : public Astro {
 public:
  Satelite () : Astro() { };
  Satelite (const string id, const float posicion, 
        const int colorRojo, const int colorVerde, const int colorAzul,
        const double radio, const int divisiones, const float translacion, const float rotacion,
        const string idPlaneta);
  Satelite (const Satelite &s); // Constructor copia

  // Gets y Sets
  string getIdPlaneta () const;
  void setIdPlaneta(const string idPlaneta);
  
  // Sobre carga de Operadores
  Satelite& operator= (const Satelite &s);
  bool operator == (const Satelite &s) const;

  // Otras funciones
  string toString() const;
  void pintar(long tiempo) const;
 private:
  string _idPlaneta;
  
  void copiar(const Satelite &s);
};

#endif
