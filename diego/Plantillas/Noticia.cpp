#include "Noticia.h"
#include <stdio.h>

Noticia::Noticia ( const Noticia& origen )
{
  Copia ( origen );
}

void Noticia::Copia ( const Noticia& origen )
{
  _mensaje = origen.getMensaje();
  _interesante = origen.getInteresante();
}

Noticia& Noticia::operator= ( const Noticia& origen )
{
  Copia ( origen );
  return *this;
}

void Noticia::print()
{
  cout << "Mensaje : " << _mensaje << flush;

  if ( _interesante )
    cout << " - Interesante : si" << endl;
  else
    cout << " - Interesante : no" << endl;
}

Noticia::~Noticia()
{   
#ifdef __DEBUG__
  cout << __FUNCTION__ << " Se destruye la noticia : '" << _mensaje << "'"; 
  printf ( "[%p]\n", this );
#endif
};

bool Noticia::operator==( const Noticia& noticia )
{
  bool result = false;

  if ( _mensaje == noticia.getMensaje() )
    result = true;

  return result;
}
