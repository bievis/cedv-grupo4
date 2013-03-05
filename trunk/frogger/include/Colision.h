#ifndef COLISION_H
#define COLISION_H

#include "ElementoCarril.h"

using namespace std;

enum TipoColision  { CHOQUE, SOBRE, HUNDIDO, NINGUNA };

class Colision {
 public:
  Colision (const TipoColision tipo, ElementoCarril* elementoColision);
  Colision (const Colision &c);

  TipoColision getTipo () const;
  ElementoCarril* getElementoColision() const;
  
  // Sobre carga de Operadores
  Colision& operator= (const Colision &c);

  // Destructor
  ~Colision ();
 private:
  void copiar(const Colision &c);

  TipoColision _tipo;
  ElementoCarril* _elementoColision;
};

#endif
