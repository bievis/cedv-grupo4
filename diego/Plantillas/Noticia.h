#ifndef __NOTICIA_H__
#define __NOTICIA_H__

#include <iostream>
#include <string>

using namespace std;

class Noticia {
 private:
  bool _interesante;
  string _mensaje;

  void Copia ( const Noticia& origen );

 public:
 Noticia() : _interesante(false), _mensaje("")  {};
  Noticia ( const string &mensaje, bool interesante ) : _interesante(interesante), _mensaje(mensaje) {};
  Noticia ( const Noticia& origen );
  virtual ~Noticia();

  Noticia& operator= ( const Noticia& origen );

  inline string getMensaje() const { return _mensaje; };
  inline void setMensaje ( const string& mensaje ) { _mensaje = mensaje; };

  inline bool getInteresante() const { return _interesante; };
  inline void setInteresante ( bool interesante ) { _interesante = interesante; };

  void print();

  bool operator==( const Noticia& noticia );
};

#endif
