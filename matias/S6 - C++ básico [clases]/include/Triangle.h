#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vec3.h"

class Triangle {
 public:
  Triangle () { };
  Triangle (const Vec3 &v1, const Vec3 &v2, const Vec3 &v3);
  Triangle (const Triangle &t);
  ~Triangle ();

  Vec3 getV1 () const;
  Vec3 getV2 () const;
  Vec3 getV3 () const;

  Triangle& operator= (const Triangle &t);

  // Otras funciones
  string toString ();

 private:
  Vec3 *_v1, *_v2, *_v3; // Vértices del triángulo.
};

#endif
