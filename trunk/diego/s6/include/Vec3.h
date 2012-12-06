#ifndef VEC3_H
#define VEC3_H

class Vec3 {
 public:
  Vec3 ();
  Vec3 (int x, int y, int z);
  ~Vec3 ();
 
  // Funciones de acceso.
  int getX () const;
  int getY () const;
  int getZ () const;
  void setX (int x);
  void setY (int y);
  void setZ (int z);

  // Sobrecarga de operadores.
  void operator+ (const Vec3 &op);
  void operator- (const Vec3 &op);

 private:
  int _x, _y, _z; // Coordenadas 3D.
};

#endif
