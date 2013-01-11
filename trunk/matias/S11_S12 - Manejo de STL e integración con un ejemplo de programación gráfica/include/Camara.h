#ifndef CAMARA_H
#define CAMARA_H

#include <iostream>

using namespace std;

class Camara {
 public:
  Camara () {
    _posx = 0.0;
    _posy = 0.0;
    _posz = 0.0;
    _lookx = 0.0;
    _looky = 0.0;
    _lookz = 0.0;
    _upx = 0.0;
    _upy = 0.0;
    _upz = 0.0;
  };
  Camara (const double _posx, const double _posy, const double _posz,
         const double _lookx, const double _looky, const double _lookz,
         const double _upx, const double _upy, const double _upz);
  Camara (const Camara &c); // Constructor copia

  // Gets y Sets
  double getPosx () const;
  double getPosy () const;
  double getPosz () const;
  double getLookx () const;
  double getLooky () const;
  double getLookz () const;
  double getUpx () const;
  double getUpy () const;
  double getUpz () const;
  void setPosx(const double posx);
  void setPosy(const double posy);
  void setPosz(const double posz);
  void setLookx(const double lookx);
  void setLooky(const double looky);
  void setLookz(const double lookz);
  void setUpx(const double upx);
  void setUpy(const double upy);
  void setUpz(const double upz);
  
  // Sobre carga de Operadores
  Camara& operator= (const Camara &c);
 private:
  double _posx, _posy, _posz, _lookx, _looky, _lookz, _upx, _upy, _upz;  

  void copiar(const Camara &c);
};

#endif
