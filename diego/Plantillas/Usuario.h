#ifndef __USUARIO_H__
#define __USUARIO_H__

#include <iostream>
#include <stdio.h>

using namespace std;

class Usuario {

 protected:
  string _nombre;

 public:
  Usuario() : _nombre("") {};
  Usuario ( const string &usuario ) : _nombre(usuario) {};
  Usuario ( const Usuario& origen );
  virtual ~Usuario();

  Usuario& operator= (const Usuario& origen );

  inline string getNombre() const { return _nombre; };
  inline void setNombre( const string& nombre ) { _nombre = nombre; };

  //  inline string getValorComparacion() const { return _nombre; };

  bool operator==(const Usuario& usuario);

  void print();
};

void Usuario::print()
{
  cout << "Nombre : " << _nombre << endl;
}

Usuario::Usuario(const Usuario& origen)
{
  _nombre = origen.getNombre();
}

Usuario& Usuario::operator=(const Usuario& origen)
{
  _nombre = origen.getNombre();
  return *this;
}

Usuario::~Usuario()
{
#ifdef __DEBUG__
  cout << __FUNCTION__ << " Se destruye el usuario : '" << _nombre << "'"; 
  printf ( "[%p]\n", this );
#endif
}

bool Usuario::operator==(const Usuario& usuario)
{
  bool result = false;

  if ( _nombre == usuario.getNombre() )
    result = true;

  return result;
}

#endif
