#ifndef __ADMIN_H__
#define __ADMIN_H__

#include <iostream>
#include "Usuario.h"
#include <stdio.h>

using namespace std;

class Admin : public Usuario {

public:
  virtual ~Admin();

  bool operator==(const Admin& usuario);

};

Admin::~Admin()
{
#ifdef __DEBUG__
  cout << __FUNCTION__ << " Se destruye el administrador : '" << _nombre << "'"; 
  printf ( "[%p]\n", this );
#endif
}

bool Admin::operator==(const Admin& usuario)
{
  bool result = false;

  if ( _nombre == usuario.getNombre() )
    result = true;

  return result;
}

#endif
