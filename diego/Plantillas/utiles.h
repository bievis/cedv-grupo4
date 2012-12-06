#ifndef __UTILES_H__
#define __UTILES_H__

#include <iostream>
#include "Canal.h"
#include "Usuario.h"
#include "Admin.h"
#include "Lista.h"

unsigned int find_canal ( const Lista<Canal>& lista_canales, const string& nombreCanal );

unsigned int find_usuario ( const Lista<Usuario>& lista_usuarios,const string& nombre );

unsigned int find_admin ( const Lista<Admin>& lista_admins,const string& nombre );

#endif
