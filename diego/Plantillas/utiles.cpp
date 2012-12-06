#include "utiles.h"

//Devolverá 0 si no lo encuentra y != 0 si lo encuentra (posicion en la lista)
unsigned int find_canal ( const Lista<Canal>& lista_canales, const string& nombreCanal )
{
  unsigned int i = 0, pos = 0;
  Canal *c = 0;
  
  for ( i = 1; i <= lista_canales.getNumElems(); i++ )
    {
      lista_canales.getRefElem ( c, i );

      if ( c->getNombreCanal() == nombreCanal )
	{
          pos = i;
          break;
	}
    }
  return pos;
}

//Devolverá 0 si no lo encuentra y != 0 si lo encuentra (posicion en la lista)
unsigned int find_usuario ( const Lista<Usuario>& lista_usuarios, const string& nombre )
{
  unsigned int i = 0, pos = 0;
  Usuario *u = 0;
  
  for ( i = 1; i <= lista_usuarios.getNumElems(); i++ )
    {
      lista_usuarios.getRefElem ( u, i );

      if ( u->getNombre() == nombre )
	{
          pos = i;
          break;
	}
    }
  return pos;
}

//Devolverá 0 si no lo encuentra y != 0 si lo encuentra (posicion en la lista)
unsigned int find_admin ( const Lista<Admin>& lista_admins, const string& nombre )
{
  unsigned int i = 0, pos = 0;
  Admin *a = 0;
  
  for ( i = 1; i <= lista_admins.getNumElems(); i++ )
    {
      lista_admins.getRefElem ( a, i );

      if ( a->getNombre() == nombre )
	{
          pos = i;
          break;
	}
    }
  return pos;
}
