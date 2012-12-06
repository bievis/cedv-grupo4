#ifndef VEC3_H
#define VEC3_H

#include <fstream>

using namespace std;

class Vec3 {
 public:
  Vec3 (int x, int y, int z);
  ~Vec3 ();
 
  // Funciones de acceso.
  int getX () const;
  int getY () const;
  int getZ () const;

  // Funciones de modificacion.
  void setX (int x) { _x = x; };
  void setY (int y) { _y = y; };
  void setZ (int z) { _z = z; };

  // Otras funciones
  string toString ();

  // Sobrecarga de operadores.
  void operator+ (const Vec3 &op);
  void operator- (const Vec3 &op);
  Vec3& operator= (const Vec3 &op);

 private:
  int _x, _y, _z; // Coordenadas 3D.
};

#endif
