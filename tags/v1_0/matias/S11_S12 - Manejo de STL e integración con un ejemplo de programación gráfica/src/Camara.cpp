#include "Camara.h"

Camara::Camara (const double posx, const double posy, const double posz,
         const double lookx, const double looky, const double lookz,
         const double upx, const double upy, const double upz) {
  _posx = posx;
  _posy = posy;
  _posz = posz;
  _lookx = lookx;
  _looky = looky;
  _lookz = lookz;
  _upx = upx;
  _upy = upy;
  _upz = upz;
}

// Gets y Sets
inline double Camara::getPosx () const {
  return _posx;
}

inline double Camara::getPosy () const {
  return _posy;
}

inline double Camara::getPosz () const {
  return _posz;
}

inline double Camara::getLookx () const {
  return _lookx;
}

inline double Camara::getLooky () const {
  return _looky;
}

inline double Camara::getLookz () const {
  return _lookz;
}

inline double Camara::getUpx () const {
  return _upx;
}

inline double Camara::getUpy () const {
  return _upy;
}

inline double Camara::getUpz () const {
  return _upz;
}

inline void Camara::setPosx(const double posx) {
  _posx = posx;
}

inline void Camara::setPosy(const double posy) {
  _posy = posy;
}

inline void Camara::setPosz(const double posz) {
  _posz = posz;
}

inline void Camara::setLookx(const double lookx) {
  _lookx = lookx;
}

inline void Camara::setLooky(const double looky) {
  _looky = looky;
}

inline void Camara::setLookz(const double lookz) {
  _lookz = lookz;
}

inline void Camara::setUpx(const double upx) {
  _upx = upx;
}

inline void Camara::setUpy(const double upy) {
  _upy = upy;
}

inline void Camara::setUpz(const double upz) {
  _upz = upz;
}

Camara::Camara (const Camara &c) {
  copiar(c);
}

// Sobre carga de Operadores
Camara& Camara::operator= (const Camara &c) {
  copiar(c);
  return *this;
}

void Camara::copiar(const Camara &c) {
  _posx = c.getPosx();
  _posy = c.getPosy();
  _posz = c.getPosz();
  _lookx = c.getLookx();
  _looky = c.getLooky();
  _lookz = c.getLookz();
  _upx = c.getUpx();
  _upy = c.getUpy();
  _upz = c.getUpz();
}
